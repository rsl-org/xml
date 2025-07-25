#pragma once
#include "document.hpp"
#include "rsl/serializer/xml/annotations.hpp"

#include <format>

#include <rsl/meta_traits>
#include <rsl/serializer/machinery.hpp>
#include <stdexcept>
#include <type_traits>

namespace rsl::serializer::_xml_impl {

class Serializer {
  XmlNode document;
  XmlNode* cursor = nullptr;

  static std::string stringify_value(bool value) { return value ? "true" : "false"; }

  template <typename T>
    requires(std::is_arithmetic_v<T>)
  static std::string stringify_value(T value) {
    return std::to_string(value);
  }

  static std::string stringify_value(std::string_view value) {
    std::string output;
    output.reserve(value.size());

    for (char ch : value) {
      switch (ch) {
        case '&': output.append("&amp;"); break;
        case '<': output.append("&lt;"); break;
        case '>': output.append("&gt;"); break;
        case '"': output.append("&quot;"); break;
        case '\'': output.append("&#39;"); break;
        default: output.push_back(ch); break;
      }
    }
    return output;
  }

  [[nodiscard]] std::string get_header() const {
    return R"(<?xml version="1.0" encoding="UTF-8"?>)"
           "\n";
  }

public:
  Serializer() = default;

  // template <is_optional R, typename T>
  // void operator()(R meta, T&& value) {
  //   // check for annotation?
  //   meta.descend(*this, std::forward<T>(value));
  // }

  template <has_members R, typename T>
  void operator()(R meta, T&& value) {
    if constexpr (!has_identifier(meta.info)) {
      throw std::runtime_error("Child nodes with members need identifiers.");
    } else {
      static constexpr std::string_view name = define_static_string(identifier_of(
          is_type(meta.info) || !has_identifier(type_of(meta.info)) ? meta.info
                                                                    : type_of(meta.info)));

      if (cursor == nullptr) {
        // root node
        document.tag = name;
        // make it self-referential to prevent another root node from being added.
        // this greatly simplifies the logic to walk up, `parent` is never nullptr
        document.parent = &document;
        cursor          = &document;
      } else {
        cursor = cursor->add({std::string(name)});
      }
      meta.descend(*this, std::forward<T>(value));
      cursor = cursor->parent;
    }
  }

  template <is_iterable R, typename T>
  void operator()(R meta, T&& value) {
    if (cursor == nullptr) {
      throw std::runtime_error("Root node must be a structure");
    }

    meta.descend(*this, value);
  }

  template <typename R, typename T>
  void operator()(R meta, T&& value) {
    if (cursor == nullptr) {
      throw std::runtime_error("Root node must be a structure");
    }

    if constexpr (meta::has_annotation(meta.info, ^^xml::annotations::Attribute)) {
      if constexpr (!has_identifier(meta.info)) {
        throw std::runtime_error("Attributes must have identifiers.");
      } else {
        if constexpr (is_optional<R>) {
          if (!value.has_value()) {
            return;
          }
          cursor->attributes[std::string(identifier_of(meta.info))] = stringify_value(*value);
        } else {
          cursor->attributes[std::string(identifier_of(meta.info))] = stringify_value(value);
        }
      }
    } else if constexpr (meta::has_annotation(meta.info, ^^xml::annotations::Raw)) {
      if constexpr (is_optional<R>) {
        if (!value.has_value()) {
          return;
        }
        cursor->raw_content = stringify_value(*value);
      } else {
        cursor->raw_content = stringify_value(value);
      }
    } else if constexpr (is_optional<R> &&
                         meta::has_annotation(meta.info, ^^xml::annotations::Node)) {
      if (!value.has_value()) {
        return;
      }
      (*this)(rsl::serializer::Meta<typename std::remove_cvref_t<T>::value_type>(), *value);
    }
  }

  [[nodiscard]] std::string finalize() const { return get_header() + document.stringify(); }
};
}  // namespace rsl::serializer::_xml_impl