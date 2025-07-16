#include <print>
#include <rsl/repr>

struct Test {
  int foo = 3;
  std::vector<int> bar {1, 5, 8, 29, 3};
};

int main() {
  auto obj = Test();
  std::println("{}", rsl::repr(obj));
}