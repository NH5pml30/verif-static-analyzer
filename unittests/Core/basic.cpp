#include <gtest/gtest.h>

TEST(BasicTest, BasicAssertions) {
  EXPECT_STRNE("hello", "world");
  EXPECT_EQ(7 * 6, 42);
}

bool valid_age(int age) {
  if (age >= 21) {
    return true;
  }
  /*
  if (age >= 16) {
    return false;
  }
  if (age >= 10) {
    return true;
  }
  */
  return false;
}

TEST(BasicTest, ValidAgeAssertions) {
  EXPECT_TRUE(valid_age(25));
  EXPECT_FALSE(valid_age(20));
  // EXPECT_FALSE(valid_age(16));
  // EXPECT_TRUE(valid_age(15));
  // EXPECT_TRUE(valid_age(10));
  // EXPECT_FALSE(valid_age(9));
}
