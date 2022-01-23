#ifndef OPTOY_SIMPLECAM_HPP
#define OPTOY_SIMPLECAM_HPP

#include "mathutils.hpp"
#include "predef.hpp"
#include "scenenode.hpp"
#include "ray.hpp"

namespace optoy {

template<class TScalar>
class basic_camera_t : public movable_scene_node_t<TScalar> {
public:
    void look_at(const vector3<TScalar>& direction, const vector3<TScalar>& up) NOIMPL_ERROR;

private:
    vector4<TScalar> _lookat;
    vector4<TScalar> _up;
};

template<class TScalar>
class simple_cam_t : public basic_camera_t<TScalar> {
public:
    simple_cam_t(TScalar aspect_ration, TScalar film_depth, TScalar aperture_radius);
    ray_t<TScalar> generate_ray(TScalar x, TScalar y) const;
    ray_t<TScalar> generate_ray_without_bias() const;

private:
    TScalar _aspect_ratio;
    TScalar _film_depth;
    TScalar _aperture_radius;
};

} // namespace optoy

#endif // !OPTOY_SIMPLECAM_HPP
