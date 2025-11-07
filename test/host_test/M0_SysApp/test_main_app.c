#include "unity.h"
#include "main_app.h"

void test_main_app_dummy(void)
{
    uint32_t input = 5;
    uint32_t expected_output = 6;
    uint32_t actual_output = main_app_dummy(input);
    TEST_ASSERT_EQUAL_UINT32(expected_output, actual_output);
}
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_main_app_dummy);
    return UNITY_END();
}


