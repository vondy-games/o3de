/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#include "precompiled.h"

#include <QTableView>

#include <GraphCanvas/Components/SceneBus.h>
#include <GraphCanvas/Components/Slots/SlotBus.h>
#include <GraphCanvas/Components/Slots/Data/DataSlotBus.h>
#include <GraphCanvas/Components/ViewBus.h>
#include <GraphCanvas/Components/VisualBus.h>
#include <GraphCanvas/Editor/AssetEditorBus.h>

#include <GraphCanvas/Utils/ConversionUtils.h>

#include <Editor/GraphCanvas/GraphCanvasEditorNotificationBusId.h>

#include <ScriptCanvasDeveloperEditor/EditorAutomation/EditorAutomationActions/WidgetActions.h>

#include <ScriptCanvasDeveloperEditor/EditorAutomation/EditorAutomationActions/EditorMouseActions.h>
#include <ScriptCanvasDeveloperEditor/EditorAutomation/EditorAutomationActions/EditorKeyActions.h>

namespace ScriptCanvasDeveloper
{
    //////////////////////////
    // WriteToLineEditAction
    //////////////////////////

    WriteToLineEditAction::WriteToLineEditAction(QLineEdit* targetEdit, QString targetText)
        : m_targetEdit(targetEdit)
        , m_targetText(targetText)
    {
    }

    void WriteToLineEditAction::SetupAction()
    {
        ClearActionQueue();

        QPoint targetPoint = m_targetEdit->mapToGlobal(QPoint(5, m_targetEdit->height() * 0.5f));

        // Cheaty clear for right now.
        m_targetEdit->clear();

        AddAction(aznew ScriptCanvasDeveloper::MouseClickAction(Qt::MouseButton::LeftButton, targetPoint));
        AddAction(aznew ScriptCanvasDeveloper::TypeStringAction(m_targetText));

        CompoundAction::SetupAction();
    }

    /////////////////////////////
    // MoveMouseToViewRowAction
    /////////////////////////////

    MoveMouseToViewRowAction::MoveMouseToViewRowAction(QAbstractItemView* itemView, int row, QModelIndex parentIndex)
        : m_itemView(itemView)
        , m_row(row)
        , m_parentIndex(parentIndex)
    {
    }

    void MoveMouseToViewRowAction::SetupAction()
    {
        ClearActionQueue();

        QModelIndex index = m_itemView->model()->index(m_row, 0, m_parentIndex);

        if (index.isValid())
        {
            QRect targetRect = m_itemView->visualRect(index);

            int column = 1;

            while (column < m_itemView->model()->columnCount())
            {
                targetRect |= m_itemView->visualRect(m_itemView->model()->index(m_row, column, m_parentIndex));
                ++column;
            }

            AddAction(aznew MouseMoveAction(m_itemView->mapToGlobal(targetRect.center())));
        }

        CompoundAction::SetupAction();
    }
}