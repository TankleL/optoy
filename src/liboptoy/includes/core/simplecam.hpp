#ifndef OPTOY_SIMPLECAM_HPP
#define OPTOY_SIMPLECAM_HPP

#include "scenenode.hpp"

namespace optoy {
class simeple_camera_t : public position_scene_node_t {
public:
    transformation_t view_transformation() const;
};
} // namespace optoy

#endif // !OPTOY_SIMPLECAM_HPP
