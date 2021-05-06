"""
All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
its licensors.

For complete copyright and license terms please see the LICENSE at the root of this
distribution (the "License"). All use of this software is governed by the License,
or, if provided, by the license below or the license accompanying this file. Do not
remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.


Test case ID: T92569049
Test Case Title: Edit > Undo undoes the last action
URL of the test case: https://testrail.agscollab.com/index.php?/tests/view/92569049
Test case ID: T92569051
Test Case Title: Edit > Redo redoes the last undone action
URL of the test case: https://testrail.agscollab.com/index.php?/tests/view/92569051
"""


# fmt: off
class Tests():
    variable_created = ("New variable created", "New variable not created")
    undo_worked      = ("Undo action working",  "Undo action did not work")
    redo_worked      = ("Redo action working",  "Redo action did not work")
# fmt: on


def EditMenu_UndoRedo():
    """
    Summary:
     Edit > Undo undoes the last action
     Edit > Redo redoes the last undone action
     We create a new variable in variable manager, undo and verify if variable is removed,
     redo it and verify if the variable is created again.

    Expected Behavior:
     The last action is undone.
     The last undone action is redone.

    Test Steps:
     1) Open Script Canvas window (Tools > Script Canvas)
     2) Get the SC window object
     3) Open Variable Manager if not opened already
     4) Create Graph
     5) Create new variable
     6) Verify if the variable is created initially
     7) Trigger Undo action and verify if variable is removed in Variable Manager
     8) Trigger Redo action and verify if variable is readded in Variable Manager
     9) Close SC window

    Note:
     - This test file must be called from the Open 3D Engine Editor command terminal
     - Any passed and failed tests are written to the Editor.log file.
            Parsing the file or running a log_monitor are required to observe the test results.

    :return: None
    """

    from PySide2 import QtWidgets, QtCore

    import azlmbr.legacy.general as general

    import pyside_utils

    # 1) Open Script Canvas window
    general.idle_enable(True)
    general.open_pane("Script Canvas")

    # 2) Get the SC window object
    editor_window = pyside_utils.get_editor_main_window()
    sc = editor_window.findChild(QtWidgets.QDockWidget, "Script Canvas")

    # 3) Open Variable Manager if not opened already
    if sc.findChild(QtWidgets.QDockWidget, "VariableManager") is None:
        action = pyside_utils.find_child_by_pattern(sc, {"text": "Variable Manager", "type": QtWidgets.QAction})
        action.trigger()
    variable_manager = sc.findChild(QtWidgets.QDockWidget, "VariableManager")

    # 4) Create Graph
    action = pyside_utils.find_child_by_pattern(sc, {"objectName": "action_New_Script", "type": QtWidgets.QAction})
    action.trigger()

    # 5) Create new variable
    add_button = variable_manager.findChild(QtWidgets.QPushButton, "addButton")
    add_button.click()  # Click on Create Variable button
    # Select variable type
    table_view = variable_manager.findChild(QtWidgets.QTableView, "variablePalette")
    model_index = pyside_utils.find_child_by_pattern(table_view, "Boolean")
    # Click on it to create variable
    pyside_utils.item_view_index_mouse_click(table_view, model_index)

    # 6) Verify if the variable is created initially
    graph_vars = variable_manager.findChild(QtWidgets.QTableView, "graphVariables")
    result = graph_vars.model().rowCount(QtCore.QModelIndex()) == 1  # since we added 1 variable, rowcount=1
    Report.result(Tests.variable_created, result)

    # 7) Trigger Undo action and verify if variable is removed in Variable Manager
    action = sc.findChild(QtWidgets.QAction, "action_Undo")
    action.trigger()
    result = graph_vars.model().rowCount(QtCore.QModelIndex()) == 0  # since we triggered undo, rowcount=0
    Report.result(Tests.undo_worked, result)

    # 8) Trigger Redo action and verify if variable is readded in Variable Manager
    action = sc.findChild(QtWidgets.QAction, "action_Redo")
    action.trigger()
    result = (
        graph_vars.model().rowCount(QtCore.QModelIndex()) == 1
    )  # since action is redone 1 variable is readded, rowcount=1
    Report.result(Tests.redo_worked, result)

    # 9) Close SC window
    general.close_pane("Script Canvas")


if __name__ == "__main__":
    import ImportPathHelper as imports

    imports.init()

    from utils import Report

    Report.start_test(EditMenu_UndoRedo)