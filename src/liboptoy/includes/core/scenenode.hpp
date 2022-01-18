#ifndef OPTOY_SCENENODE_HPP
#define OPTOY_SCENENODE_HPP

#include "interfaces/iscene.hpp"

namespace optoy {
// >>> proxy note: scene_node_t meets the requirements of scene_node::interface_t
class scene_node_t {
public:
    std::string_view name() const { return _name; }
    const std::vector<std::proxy<scene_node::interface_t>>& children() const { return _children; }
    void add(std::proxy<scene_node::interface_t>&& node) { _children.emplace_back(std::move(node)); }

protected:
    std::string _name;
    std::vector<std::proxy<scene_node::interface_t>> _children;
};

// >>> proxy note: position_scene_node_t meets the requirements of position_scene_node::interface_t
class position_scene_node_t : scene_node_t {
public:
    transformation_t& model_transformation() { return _model; }

private:
    transformation_t _model;
};
} // namespace optoy

#endif // !OPTOY_SCENENODE_HPP
