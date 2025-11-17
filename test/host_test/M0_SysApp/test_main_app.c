#include "unity.h"
#include "unity_fixture.h"  // For TEST_GROUP, RUN_TEST_CASE, RUN_TEST_GROUP, UnityMain, etc.
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "unity_internals.h"

TEST_GROUP(Test_Sysapp);  // Group name to avoid collision with stdio's 'sprintf'




TEST_SETUP(Test_Sysapp)
{

}

TEST_TEAR_DOWN(Test_Sysapp)
{
}

TEST(Test_Sysapp, NoFormatOperations)
{
    TEST_ASSERT_EQUAL(1, 1);
}


// This expands to a function that runs the tests in the group (uses RUN_TEST_CASE, not RUN_TEST)
TEST_GROUP_RUNNER(Test_Sysapp) {
    RUN_TEST_CASE(Test_Sysapp, NoFormatOperations);
}

