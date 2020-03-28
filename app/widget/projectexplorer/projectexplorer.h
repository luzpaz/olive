/***

  Olive - Non-Linear Video Editor
  Copyright (C) 2019 Olive Team

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

***/

#ifndef PROJECTEXPLORER_H
#define PROJECTEXPLORER_H

#include <QStackedWidget>
#include <QTimer>
#include <QTreeView>

#include "project/project.h"
#include "project/projectviewmodel.h"
#include "widget/projectexplorer/projectexplorericonview.h"
#include "widget/projectexplorer/projectexplorerlistview.h"
#include "widget/projectexplorer/projectexplorertreeview.h"
#include "widget/projectexplorer/projectexplorernavigation.h"
#include "widget/projecttoolbar/projecttoolbar.h"

/**
 * @brief A widget for browsing through a Project structure.
 *
 * ProjectExplorer automatically handles the view<->model system using a ProjectViewModel. Therefore, all that needs to
 * be provided is the Project structure itself.
 *
 * This widget contains three views, tree view, list view, and icon view. These can be switched at any time.
 */
class ProjectExplorer : public QWidget
{
  Q_OBJECT
public:
  ProjectExplorer(QWidget* parent);

  const ProjectToolbar::ViewType& view_type();

  Project* project();
  void set_project(Project* p);

  QList<Item*> SelectedItems();

  /**
   * @brief Use a heuristic to determine which (if any) folder is selected
   *
   * Generally for some import/adding processes, we assume that if a folder is selected, the user probably wants to
   * create the new object in it rather than in the root. If, however, more than one folder is selected, we can't
   * truly determine any folder from this and just return the root instead.
   *
   * @return
   *
   * A folder that's heuristically been determined as "selected", or the root directory if none, or nullptr if no
   * project is open.
   */
  Folder* GetSelectedFolder();

  /**
   * @brief Access the ViewModel model of the project
   */
  ProjectViewModel* model();

  void SelectAll();

  void DeselectAll();

public slots:
  void set_view_type(ProjectToolbar::ViewType type);

  void Edit(Item* item);

signals:
  /**
   * @brief Emitted when an Item is double clicked
   *
   * @param item
   *
   * The Item that was double clicked, or nullptr if empty area was double clicked
   */
  void DoubleClickedItem(Item* item);

private:
  /**
   * @brief Simple convenience function for adding a view to this stacked widget
   *
   * Mainly for use in the constructor. Adds the view, connects its signals/slots, and sets the model.
   *
   * @param view
   *
   * View to add to the stack
   */
  void AddView(QAbstractItemView* view);

  /**
   * @brief Browse to a specific folder index in the model
   *
   * Only affects list_view_ and icon_view_.
   *
   * @param index
   *
   * Either an invalid index to return to the project root, or an index to a valid Folder object.
   */
  void BrowseToFolder(const QModelIndex& index);

  /**
   * @brief Get the currently active QAbstractItemView
   */
  QAbstractItemView* CurrentView();

  QStackedWidget* stacked_widget_;

  ProjectExplorerNavigation* nav_bar_;

  ProjectExplorerIconView* icon_view_;
  ProjectExplorerListView* list_view_;
  ProjectExplorerTreeView* tree_view_;

  ProjectToolbar::ViewType view_type_;

  ProjectViewModel model_;

  QModelIndex clicked_index_;

  QTimer rename_timer_;

  Item* context_menu_item_;

private slots:
  void ItemClickedSlot(const QModelIndex& index);

  void ViewEmptyAreaDoubleClickedSlot();

  void ItemDoubleClickedSlot(const QModelIndex& index);

  void SizeChangedSlot(int s);

  void DirUpSlot();

  void RenameTimerSlot();

  void ShowContextMenu();

  void ShowFootagePropertiesDialog();

  void ShowSequencePropertiesDialog();

  void RevealSelectedFootage();

};

#endif // PROJECTEXPLORER_H
