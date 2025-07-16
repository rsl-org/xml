#include <string_view>
#include <string>

#include <gtest/gtest.h>
#include <rsl/repr>

TEST(Repr, EmptyStruct) {
  struct Test{};
  auto result = rsl::repr(Test{});
  ASSERT_EQ(result, "Test{}");
}