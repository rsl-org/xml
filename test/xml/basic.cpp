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
  ASSERT_EQ(result, xml_head + "<Outer>\n  <Inner/>\n</Outer>"s);
}

TEST(Xml, Attributes) {
  struct Test{
    [[=rsl::xml::attribute]] std::string name;
    [[=rsl::xml::attribute]] unsigned test = 123;
  };
  auto obj = Test("foo");
  auto result = rsl::to_xml(obj);
  ASSERT_EQ(result, xml_head + R"(<Test test="123" name="foo"/>)"s);
}

TEST(Xml, OptionalAttributes) {
  struct Test{
    [[=rsl::xml::attribute]] std::string name;
    [[=rsl::xml::attribute]] std::optional<unsigned> test;
  };
  auto obj = Test("foo");
  auto result = rsl::to_xml(obj);
  ASSERT_EQ(result, xml_head + R"(<Test name="foo"/>)"s);
}

TEST(Xml, RawNodes) {
  struct Test{
    [[=rsl::xml::raw]] std::string foo;    
  };
  auto obj = Test("zoinks");
  auto result = rsl::to_xml(obj);
  ASSERT_EQ(result, xml_head + "<Test>\n  <foo>zoinks</foo>\n</Test>"s);
}