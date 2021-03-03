#include <gtest/gtest.h>


int add(int x, int y) {
    return x + y;
}

TEST(AddTest, TwoAndThree) {
    EXPECT_EQ(add(2, 3), 5);
}
