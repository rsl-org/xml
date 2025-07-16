#pragma once

namespace rsl::xml {
namespace annotations {
struct Attribute {};
struct Raw {};
}  // namespace annotations
constexpr inline annotations::Attribute attribute{};
constexpr inline annotations::Raw raw{};
}  // namespace rsl::xml