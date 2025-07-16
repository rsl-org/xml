#pragma once

namespace rsl::xml {
namespace annotations {
struct Attribute {};
struct Raw {};
struct Node {};
}  // namespace annotations
constexpr inline annotations::Attribute attribute{};
constexpr inline annotations::Raw raw{};
constexpr inline annotations::Node node{};
}  // namespace rsl::xml