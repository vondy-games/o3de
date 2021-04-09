"""
All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
its licensors.

For complete copyright and license terms please see the LICENSE at the root of this
distribution (the "License"). All use of this software is governed by the License,
or, if provided, by the license below or the license accompanying this file. Do not
remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
"""

# Test case ID : C18243585
# Test Case Title : Check that hinge joint allows no limit constraints on 2 bodies
# URL of the test case :https://testrail.agscollab.com/index.php?/cases/view/18243585

# fmt: off
class Tests:
    enter_game_mode                 = ("Entered game mode",                         "Failed to enter game mode")
    exit_game_mode                  = ("Exited game mode",                          "Couldn't exit game mode")
    lead_found                      = ("Found lead",                                "Did not find lead")
    follower_found                  = ("Found follower",                            "Did not find follower")
    check_lead_position             = ("Lead stays still",                          "Lead moved")
    check_follower_position         = ("Follower moved higher than lead, and swinged over it",        "Follower did not move higher than lead, or swing over it")
# fmt: on


def C18243585_Joints_HingeNoLimitsConstrained():
    """
    Summary: Check that hinge joint allows no limit constraints on 2 bodies

    Level Description:
    lead - Starts above follower entity
    follower - Starts below lead entity. Constrained to lead entity with a hinge joint. Starts with initial velocity of (5, 1, 0).
    forceRegion - Contains suction and damping force to hold follower position when it enters the region that will pass the test.

    Expected Behavior: 
    Lead entity remains still.
    Follower entity's Z position exceeds lead entity's Z position and swings past above the lead entity.
    The hinge joint constraint is not limited, the follower can swing to a position higher than the lead, and over it

    Test Steps:
    1) Open Level
    2) Enter Game Mode
    3) Create and Validate Entities
    4) Wait for several seconds
    5) Check to see if lead and follower behaved as expected
    6) Exit Game Mode
    7) Close Editor

    Note:
    - This test file must be called from the Lumberyard Editor command terminal
    - Any passed and failed tests are written to the Editor.log file.
            Parsing the file or running a log_monitor are required to observe the test results.

    :return: None
    """
    import os
    import sys

    import ImportPathHelper as imports

    imports.init()


    from utils import Report
    from utils import TestHelper as helper

    import azlmbr.legacy.general as general
    import azlmbr.bus

    import JointsHelper
    from JointsHelper import JointEntity

    # Constants
    FLOAT_EPSILON = 0.2

    # Helper Entity class
    class Entity(JointEntity):
        def criticalEntityFound(self): # Override function to use local Test dictionary
            Report.critical_result(Tests.__dict__[self.name + "_found"], self.id.isValid())

    # Main Script
    helper.init_idle()

    # 1) Open Level
    helper.open_level("Physics", "C18243585_Joints_HingeNoLimitsConstrained")

    # 2) Enter Game Mode
    helper.enter_game_mode(Tests.enter_game_mode)

    # 3) Create and Validate Entities
    lead = Entity("lead")
    follower = Entity("follower")
    Report.info_vector3(lead.position, "lead initial position:")
    Report.info_vector3(follower.position, "follower initial position:")
    leadInitialPosition = lead.position
    followerInitialPosition = follower.position

    # 4) Wait for several seconds
    general.idle_wait(4.0) # wait for lead and follower to move

    # 5) Check to see if lead and follower behaved as expected
    Report.info_vector3(lead.position, "lead position after 1 second:")
    Report.info_vector3(follower.position, "follower position after 1 second:")

    leadPositionDelta = lead.position.Subtract(leadInitialPosition)
    leadRemainedStill = JointsHelper.vector3SmallerThanScalar(leadPositionDelta, FLOAT_EPSILON)
    Report.critical_result(Tests.check_lead_position, leadRemainedStill)

    followerSwingedOverLead = (follower.position.x < leadInitialPosition.x  and
                        follower.position.z > leadInitialPosition.z)
    Report.critical_result(Tests.check_follower_position, followerSwingedOverLead)

    # 6) Exit Game Mode
    helper.exit_game_mode(Tests.exit_game_mode)



if __name__ == "__main__":
    import ImportPathHelper as imports
    imports.init()

    from utils import Report
    Report.start_test(C18243585_Joints_HingeNoLimitsConstrained)