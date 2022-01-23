#ifndef OPTOY_ISCENE_HPP
#define OPTOY_ISCENE_HPP

#include <string_view>
#include <vector>
#include <stl-preview/proxy>
#include "ishape.hpp"
#include "core/transformation.hpp"

namespace optoy{
namespace scene_node {
struct name_f;
struct add_f;
struct children_f;
struct interface_t : std::facade<
    name_f,
    add_f,
    children_f>{};

struct name_f : std::facade_operation<
    std::string_view(),
    [](const auto& self) { return self.name(); }> {};

struct add_f : std::facade_operation<
    void(std::proxy<interface_t>&&),
    [](auto& self, std::proxy<interface_t>&& node) { self.add(std::move(node)); }> {};

struct children_f : std::facade_operation<
    const std::vector<std::proxy<interface_t>>&(),
    [](const auto& self) { return self.children(); }> {};
} // namespace scene_node

namespace position_scene_node {
struct model_transformation_f : std::facade_operation <
    transformation_t& (),
    [](auto& self) { return self.model_transformation(); }> {};

struct interface_t : std::facade<
    scene_node::interface_t::operations,
    model_transformation_f> {};
} // namespace position_scene_node

namespace scene {
template<class TScalar>
struct interface_t : std::facade<shape::intersect_f<TScalar>>{};
} // namespace scene
} // namespace optoy

#endif