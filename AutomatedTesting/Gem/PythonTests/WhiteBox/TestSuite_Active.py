"""
All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
its licensors.

For complete copyright and license terms please see the LICENSE at the root of this
distribution (the "License"). All use of this software is governed by the License,
or, if provided, by the license below or the license accompanying this file. Do not
remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

"""

# This suite consists of all test cases that are passing and have been verified.

import pytest
import sys
import os

from .FileManagement import FileManagement as fm
from ly_test_tools import LAUNCHERS

sys.path.append(os.path.dirname(os.path.abspath(__file__)) + '/../automatedtesting_shared')

from base import TestAutomationBase

@pytest.mark.SUITE_main
@pytest.mark.parametrize("launcher_platform", ['windows_editor'])
@pytest.mark.parametrize("project", ["AutomatedTesting"])
class TestAutomation(TestAutomationBase):
    def test_C28798177_WhiteBox_AddComponentToEntity(self, request, workspace, editor, launcher_platform):
        from . import C28798177_WhiteBox_AddComponentToEntity as test_module
        self._run_test(request, workspace, editor, test_module)

    def test_C29279329_WhiteBox_SetDefaultShape(self, request, workspace, editor, launcher_platform):
        from . import C29279329_WhiteBox_SetDefaultShape as test_module
        self._run_test(request, workspace, editor, test_module)

    def test_C28798205_WhiteBox_SetInvisible(self, request, workspace, editor, launcher_platform):
        from . import C28798205_WhiteBox_SetInvisible as test_module
        self._run_test(request, workspace, editor, test_module)