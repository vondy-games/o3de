"""
All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
its licensors.

For complete copyright and license terms please see the LICENSE at the root of this
distribution (the "License"). All use of this software is governed by the License,
or, if provided, by the license below or the license accompanying this file. Do not
remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

Test case ID : C19578018
Test Case Title : Verify that a shape collider component with no shape component indicates a missing service
URL of the test case : https://testrail.agscollab.com/index.php?/cases/view/19578018
"""

# fmt: off
class Tests():
    create_collider_entity      = ("Created Collider Entity",        "Failed to create Collider Entity")
    add_physx_shape_collider    = ("PhysX Shape Collider added",     "Failed to add PhysX Shape Collider")
    collider_component_inactive = ("Collider component is Inactive", "Collider component is Active")
    add_shape_component         = ("Shape component added",          "Failed to add Shape component")
    collider_component_active   = ("Collider component is active",   "Collider component is inactive")
# fmt: on


def C19578018_ShapeColliderWithNoShapeComponent():
    """
    Summary:
     Create an Entity with PhysX Shape Collider component and verify that PhysX Shape Collider Component
     is inactive without shape component.

    Expected Behavior:
     The PhysX Shape Collider component should be inactive.
     Verify that after a shape component is added, the warning goes away.

    Test Steps:
     1) Load the level
     2) Add an entity with a PhysX Shape Collider component.
     3) Validate Collider Entity
     4) Validate PhysX Shape Collider component is inactive.
     5) Add Shape component to Entity
     6) Validate PhysX Shape Collider component is Active.
     7) Close the editor

    Note:
    - This test file must be called from the Lumberyard Editor command terminal
    - Any passed and failed tests are written to the Editor.log file.
        Parsing the file or running a log_monitor are required to observe the test results.

    :return: None
    """

    # Built-in Imports
    import ImportPathHelper as imports

    imports.init()

    # Helper Imports
    from utils import Report
    from utils import TestHelper as helper
    from editor_entity_utils import EditorEntity

    # Lumberyard Imports
    import azlmbr.bus as bus
    import azlmbr.editor as editor

    def is_component_active(component_id) -> bool:
        """
        Used to check if component is Active
        :return: boolean, True if component is active, else False
        """
        return editor.EditorComponentAPIBus(bus.Broadcast, "IsComponentEnabled", component_id)

    helper.init_idle()
    # 1) Load the level
    helper.open_level("Physics", "Base")

    # 2) Add an entity with a PhysX Shape Collider component.
    collider = EditorEntity.create_editor_entity("Collider")
    physx_component = collider.add_component("PhysX Shape Collider")

    # 3) Validate Collider Entity
    Report.result(Tests.create_collider_entity, collider.id.IsValid())

    # 4) Validate PhysX Shape Collider component is inactive.
    Report.result(Tests.add_physx_shape_collider, collider.has_component("PhysX Shape Collider"))
    Report.result(Tests.collider_component_inactive, not is_component_active(physx_component.id))

    # 5) Add Shape component to Entity
    collider.add_component("Box Shape")
    Report.result(Tests.add_shape_component, collider.has_component("Box Shape"))

    # 6) Validate PhysX Shape Collider component is Active.
    Report.result(Tests.collider_component_active, is_component_active(physx_component.id))



if __name__ == "__main__":
    import ImportPathHelper as imports
    imports.init()

    from utils import Report
    Report.start_test(C19578018_ShapeColliderWithNoShapeComponent)