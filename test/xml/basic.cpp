#include <string_view>
#include <string>

#include <gtest/gtest.h>
#include <rsl/xml>

static constexpr std::string_view xml_head = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

using namespace std::string_literals;

TEST(Xml, Empty) {
  struct EmptyClass{};

  auto result = rsl::to_xml(EmptyClass{});
  ASSERT_EQ(result, xml_head + "<EmptyClass/>"s);
}

TEST(Xml, Nested) {
  struct Inner {};
  struct Outer {
    Inner foo;
  };

  auto result = rsl::to_xml(Outer{.foo={}});
  ASSERT_EQ(result, xml_head + "<Outer>\n  <foo/>\n</Outer>"s);
}