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

#pragma once

#include <AzCore/Component/EntityId.h>

#include <Editor/Nodes/NodeUtils.h>

#include <GraphCanvas/Components/Slots/SlotBus.h>

#include <ScriptCanvas/Bus/NodeIdPair.h>

namespace ScriptCanvas
{
    class Slot;
}
namespace ScriptCanvas::Nodes::Core
{
    class AzEventHandler;
    class EBusEventHandler;
    class FunctionCallNode;
    class GetVariableNode;
    class Method;
    class ReceiveScriptEvent;
    class SetVariableNode;
    class SendScriptEvent;
}
namespace ScriptCanvas::Nodes::Entity
{
    class EntityRef;
}
namespace ScriptEvents
{
    class Method;
}

namespace ScriptCanvasEditor::Nodes
{
    // Generic method of displaying a node.
    AZ::EntityId DisplayScriptCanvasNode(AZ::EntityId graphCanvasGraphId, const ScriptCanvas::Node* node);

    // Core Node
    AZ::EntityId DisplayEntityNode(AZ::EntityId graphCanvasGraphId, const ScriptCanvas::Nodes::Entity::EntityRef* entityNode);
    AZ::EntityId DisplayEbusEventNode(AZ::EntityId graphCanvasGraphId, const AZStd::string& busName, const AZStd::string& eventName, const ScriptCanvas::EBusEventId& eventId);
    AZ::EntityId DisplayEbusWrapperNode(AZ::EntityId graphCanvasGraphId, const ScriptCanvas::Nodes::Core::EBusEventHandler* busNode);
    AZ::EntityId DisplayGetVariableNode(AZ::EntityId graphCanvasGraphId, const ScriptCanvas::Nodes::Core::GetVariableNode* variableNode);
    AZ::EntityId DisplayMethodNode(AZ::EntityId graphCanvasGraphId, const ScriptCanvas::Nodes::Core::Method* methodNode);
    AZ::EntityId DisplaySetVariableNode(AZ::EntityId graphCanvasGraphId, const ScriptCanvas::Nodes::Core::SetVariableNode* variableNode);

    // AZ Event
    AZ::EntityId DisplayAzEventHandlerNode(AZ::EntityId, const ScriptCanvas::Nodes::Core::AzEventHandler* azEventNode);

    // Script Events
    AZ::EntityId DisplayScriptEventNode(AZ::EntityId graphCanvasGraphId, const AZ::Data::AssetId assetId, const ScriptEvents::Method& methodDefinition);
    AZ::EntityId DisplayScriptEventSenderNode(AZ::EntityId graphCanvasGraphId, const ScriptCanvas::Nodes::Core::SendScriptEvent* senderNode);
    AZ::EntityId DisplayScriptEventWrapperNode(AZ::EntityId graphCanvasGraphId, const ScriptCanvas::Nodes::Core::ReceiveScriptEvent* busNode);

    // Functions
    AZ::EntityId DisplayFunctionNode(AZ::EntityId graphCanvasGraphId, const ScriptCanvas::Nodes::Core::FunctionCallNode* functionNode);
    AZ::EntityId DisplayFunctionNode(AZ::EntityId graphCanvasGraphId, ScriptCanvas::Nodes::Core::FunctionCallNode* functionNode);


    // SlotGroup will control how elements are grouped.
    // Invalid will cause the slots to put themselves into whatever category they belong to by default.
    AZ::EntityId DisplayScriptCanvasSlot(AZ::EntityId graphCanvasNodeId, const ScriptCanvas::Slot& slot, GraphCanvas::SlotGroup group = GraphCanvas::SlotGroups::Invalid);
}