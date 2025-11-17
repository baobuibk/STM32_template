#include "unity.h"
#include "unity_fixture.h"  // For TEST_GROUP, RUN_TEST_CASE, RUN_TEST_GROUP, UnityMain, etc.
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "unity_internals.h"

TEST_GROUP(SprintfTests);  // Group name to avoid collision with stdio's 'sprintf'
TEST_GROUP_RUNNER(Test_Sysapp);    // thêm dòng này


static char output[100];
static const char * expected;

TEST_SETUP(SprintfTests)
{
    memset(output, 0xaa, sizeof output);
    expected = "";
}

TEST_TEAR_DOWN(SprintfTests)
{
}

static void expect(const char * s)
{
    expected = s;
}

static void given(int charsWritten)
{
    TEST_ASSERT_EQUAL(strlen(expected), charsWritten);
    TEST_ASSERT_EQUAL_STRING(expected, output);
    TEST_ASSERT_EQUAL_CHAR(0xaa, output[strlen(expected) + 1]);
}

#if 1 
TEST(SprintfTests, NoFormatOperations)
{
    expect("hey");
    given(sprintf(output, "hey"));
}

TEST(SprintfTests, InsertString)
{
    expect("Hello World\n");
    given(sprintf(output, "Hello %s\n", "World"));
}
#endif  

// This expands to a function that runs the tests in the group (uses RUN_TEST_CASE, not RUN_TEST)
TEST_GROUP_RUNNER(SprintfTests) {
    RUN_TEST_CASE(SprintfTests, NoFormatOperations);
    RUN_TEST_CASE(SprintfTests, InsertString);
}

// Wrapper to run all test groups (add more RUN_TEST_GROUP calls for additional groups)
void RunAllTests(void) {
    RUN_TEST_GROUP(SprintfTests);  // Fixed: Use RUN_TEST_GROUP, not RUN_GROUP_RUNNER
    RUN_TEST_GROUP(Test_Sysapp);    // thêm nhóm test sysapp
}

int main(int argc, const char *argv[]) {  // argv as const char *argv[]
    return UnityMain(argc, argv, RunAllTests);
}