#include <rsl/xml>
#include <print>
#include "rsl/serializer/xml/annotations.hpp"
#include <vector>


struct Foo{};

struct EmptyClass{
  struct { struct {
  std::vector<Foo> elts{{}, {}, {}};

  } bar; } foo;
  struct KK {} zoinks;
  std::vector<Foo> elts;
  [[=rsl::xml::attribute]] bool passed = true;
  [[=rsl::xml::attribute]] std::optional<int> bar;
};

int main() {
  auto result = rsl::to_xml(EmptyClass{.elts={{}, {}}});
  std::println("{}", result);
}