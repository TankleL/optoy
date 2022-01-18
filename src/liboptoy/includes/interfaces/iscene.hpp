#ifndef OPTOY_ISCENE_HPP
#define OPTOY_ISCENE_HPP

#include <string_view>
#include <vector>
#include <stl-preview/proxy>
//#include "predef.hpp"
#include "core/transformation.hpp"

namespace optoy{
namespace scene_node {
struct interface_t;

struct name_f : std::facade_operation<
    std::string_view(),
    [](const auto& self) { return self.name(); }> {};

struct children_f : std::facade_operation<
    const std::vector<std::proxy<interface_t>>&(),
    [](const auto& self) { return self.children(); }> {};

struct add_f : std::facade_operation<
    void(std::proxy<interface_t>&&),
    [](auto& self, std::proxy<interface_t>&& node) { self.add(node); }> {};

struct interface_t : std::facade<
    name_f,
    add_f,
    children_f>{};
} // namespace scene_node

namespace position_scene_node {
struct model_transformation_f : std::facade_operation <
    transformation_t& (),
    [](auto& self) { return self.model_transformation(); }> {};

struct interface_t : std::facade<
    model_transformation_f> {};
} // namespace position_scene_node
} // namespace optoy

#endif