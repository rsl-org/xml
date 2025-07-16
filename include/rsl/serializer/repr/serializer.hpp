#pragma once
#include <meta>
#include <string>
#include <sstream>
#include <type_traits>

#include <rsl/serialize>

namespace rsl::serializer::_repr_impl {

struct SerializerBase {
  template <typename T, typename V>
  void descend(this T&& self, V&& value){
    Meta<std::remove_cvref_t<T>>::descend(std::forward<T>(self), std::forward<V>(value));
  }
};

class Serializer : SerializerBase{
  std::size_t nesting_level = 0;
  std::ostringstream out;
public:
  Serializer() = default;

  template <has_members R, typename T>
  void operator()(R, T&& value) {
    out << '{';
    ++nesting_level;
    descend(std::forward<T>(value));
    --nesting_level;
    out << '}';
  }

  template <typename R, typename T>
    requires (std::is_scalar_v<T>)
  void operator()(R, T&& value) {
    out << value;
  }
};

}