#include "unity.h"


void setUp(void) { /* Setup trước mỗi test */ }
void tearDown(void) { /* Cleanup sau mỗi test */ }

void test_temp_read(void) {
    float result = (25.0f);  // Mock input
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 25.0f, result);
}

int main(void) {
    UNITY_BEGIN();  // Khởi động Unity
    RUN_TEST(test_temp_read);
    return UNITY_END();  // Kết thúc và báo cáo
}