#ifndef TIMELINEHEADER_H
#define TIMELINEHEADER_H

#include <QWidget>

class TimelineHeader : public QWidget
{
    Q_OBJECT
public:
	explicit TimelineHeader(QWidget *parent = 0);
    void set_in_point(long p);
    void set_out_point(long p);
	void set_visible_in(long i);

	bool snapping;

	void update_header(double z);

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

private:
    bool dragging;

    bool resizing_workarea;
    bool resizing_workarea_in;
    long temp_workarea_in;
    long temp_workarea_out;
    long sequence_end;

	long in_visible;

	double zoom;

	void set_playhead(int mouse_x);

signals:

public slots:
};

#endif // TIMELINEHEADER_H
