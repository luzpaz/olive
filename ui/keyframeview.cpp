#include "keyframeview.h"

#include "effects/effect.h"
#include "ui/collapsiblewidget.h"
#include "panels/panels.h"
#include "panels/effectcontrols.h"
#include "project/clip.h"
#include "panels/timeline.h"

#include <QLabel>
#include <QMouseEvent>

#define KEYFRAME_SIZE 5
#define KEYFRAME_POINT_COUNT 4

KeyframeView::KeyframeView(QWidget *parent) : QWidget(parent), mouseover(false), visible_in(0), visible_out(0) {
	setFocusPolicy(Qt::ClickFocus);
	setMouseTracking(true);
	reload();
}

void KeyframeView::reload() {
	enable_reload = true;
	update();
}

void KeyframeView::paintEvent(QPaintEvent*) {
	QPainter p(this);

	setMinimumWidth(getScreenPointFromFrame(panel_effect_controls->zoom, visible_out - visible_in));

	rowY.clear();
	for (int i=0;i<effects.size();i++) {
		Effect* e = effects.at(i);
		if (e->container->is_expanded()) {
			for (int j=0;j<e->row_count();j++) {
				QLabel* label = e->row(j)->label;
				QWidget* contents = e->container->contents;
				long frame = 20;
				int keyframe_y = label->y() + (label->height()>>1) + mapFrom(panel_effect_controls, contents->mapTo(panel_effect_controls, contents->pos())).y() - e->container->title_bar->height();
				draw_keyframe(p, getScreenPointFromFrame(panel_effect_controls->zoom, frame), keyframe_y, false);
				rowY.append(keyframe_y);
			}
		}
	}

	if (rowY.size() > 0) {
		int playhead_x = getScreenPointFromFrame(panel_effect_controls->zoom, panel_timeline->playhead-visible_in);
		p.setPen(Qt::red);
		p.drawLine(playhead_x, 0, playhead_x, height());
	}

	if (mouseover && mouseover_row < rowY.size()) {
		draw_keyframe(p, getScreenPointFromFrame(panel_effect_controls->zoom, mouseover_frame) - visible_in, rowY.at(mouseover_row), true);
	}
}

void KeyframeView::draw_keyframe(QPainter &p, int x, int y, bool semiTransparent) {
	QPoint points[KEYFRAME_POINT_COUNT] = {QPoint(x-KEYFRAME_SIZE, y), QPoint(x, y-KEYFRAME_SIZE), QPoint(x+KEYFRAME_SIZE, y), QPoint(x, y+KEYFRAME_SIZE)};
	int alpha = (semiTransparent) ? 128 : 255;
	p.setPen(QColor(0, 0, 0, alpha));
	p.setBrush(QColor(160, 160, 160, alpha));
	p.drawPolygon(points, KEYFRAME_POINT_COUNT);
}

void KeyframeView::mouseMoveEvent(QMouseEvent* event) {
	unsetCursor();
	bool new_mo = false;
	for (int i=0;i<rowY.size();i++) {
		if (event->y() > rowY.at(i)-KEYFRAME_SIZE-KEYFRAME_SIZE && event->y() < rowY.at(i)+KEYFRAME_SIZE+KEYFRAME_SIZE) {
			setCursor(Qt::CrossCursor);
			mouseover_frame = getFrameFromScreenPoint(panel_effect_controls->zoom, event->x()) + visible_in;
			mouseover_row = i;
			new_mo = true;
			break;
		}
	}
	if (new_mo || (new_mo != mouseover)) {
		update();
	}
	mouseover = new_mo;
}
