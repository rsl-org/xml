#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <sstream>

namespace rsl::serializer::_xml_impl {
 struct XmlNode {
  std::string tag;
  std::unordered_map<std::string, std::string> attributes;
  std::vector<XmlNode> children;
  std::string raw_content;
  XmlNode* parent = nullptr;

  XmlNode() = default;
  XmlNode(std::string tag, std::unordered_map<std::string, std::string> attributes = {}, std::vector<XmlNode> children = {})
    : tag(std::move(tag)), attributes(std::move(attributes)), children(std::move(children)) {
    for (auto& child : this->children)
      child.parent = this;
  }

  XmlNode* up() {
    return parent;
  }

  XmlNode* add(XmlNode child) {
    child.parent = this;
    children.push_back(std::move(child));
    return &children.back();
  }

  [[nodiscard]] XmlNode* find(std::function<bool(const XmlNode&)> pred) {
    if (pred(*this)) return this;
    for (auto& child : children)
      if (auto* found = child.find(pred))
        return found;
    return nullptr;
  }

  [[nodiscard]] bool has_attribute(std::string_view key, std::string_view value = {}) const {
    auto it = attributes.find(std::string(key));
    if (it == attributes.end()) {
      return false;
    }
    return value.empty() ? true : it->second == value;
  }

  [[nodiscard]] std::string stringify(std::size_t indent_level = 0) const {
    if (tag.empty()) {
      return "";
    }
    std::ostringstream result;
    auto indent = std::string(indent_level * 2, ' ');
    result << indent << '<' << tag;
    for (auto const& [name, value] : attributes) {
      result << ' ' << name << "=\"" << value << '"';
    }

    if (children.empty() && raw_content.empty()) {
      result << "/>";
      return result.str();
    }

    result << '>';
    if (raw_content.empty()) {
      for (auto const& child : children) {
        result << '\n' << child.stringify(indent_level + 1);
      }
      result << '\n' << indent;
    } else {
      result << raw_content;
      if (!children.empty()) {
        throw std::runtime_error(
            "Cannot have raw_content and child nodes at the same time");
      }

      if (raw_content.back() == '\n') {
        result << indent;
      }
    }

    result << "</" << tag << '>';
    return result.str();
  }
}; 
}