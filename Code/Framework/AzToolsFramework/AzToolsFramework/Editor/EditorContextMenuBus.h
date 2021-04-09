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

#include <AzCore/EBus/EBus.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/std/any.h>
#include <AzCore/std/string/string.h>

class QMenu;

namespace AzToolsFramework
{
    enum class EditorContextMenuOrdering
    {
        TOP = 0,
        MIDDLE = 500,
        BOTTOM = 1000
    };

    //! Editor Settings API Bus
    class EditorContextMenuEvents
        : public AZ::EBusTraits
    {
    public:
        virtual ~EditorContextMenuEvents() = default;

        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::MultipleAndOrdered;
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        
        /**
         * Determines the order in which handlers populate the context menus.
         */
        struct BusHandlerOrderCompare
        {
            AZ_FORCE_INLINE bool operator()(EditorContextMenuEvents* left, EditorContextMenuEvents* right) const
            {
                if (left->GetMenuPosition() == right->GetMenuPosition())
                {
                    return left->GetMenuIdentifier() < right->GetMenuIdentifier();
                }

                return left->GetMenuPosition() < right->GetMenuPosition();
            }
        };

        /**
         * Specifies the order in which a handler receives context menu events relative to other handlers.
         * This value should not be changed while the handler is connected.
         * Use the EditorContextMenuOrdering enum values as a baseline.
         * @return A value specifying this handler's relative order.
         */
        virtual int GetMenuPosition() const
        {
            return aznumeric_cast<int>(EditorContextMenuOrdering::BOTTOM);
        }

        /**
         * Returns the identifier for this handler.
         * Used to break ties in the order comparison.
         * @return A string containing the identifier for this handler.
         */
        virtual AZStd::string GetMenuIdentifier() const
        {
            return "";
        }

        /**
         * Appends menu items to the global editor context menu.
         * This is the menu that appears when right clicking the main editor window,
         * including the Entity Outliner and the Viewport.
         */
        virtual void PopulateEditorGlobalContextMenu(QMenu* menu) const = 0;
    };

    using EditorContextMenuBus = AZ::EBus<EditorContextMenuEvents>;
}