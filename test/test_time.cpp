#include <unity.h>
void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_timer() {
    unsigned int allPeriod = 150;
    unsigned int start = 0 - 50;
    unsigned int end = 100;
    TEST_ASSERT_EQUAL_UINT32(150, end - start);
}

int main( int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_timer);
    UNITY_END();
}