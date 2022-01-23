#ifndef OPTOY_SIMPLE_RENDERER_HPP
#define OPTOY_SIMPLE_RENDERER_HPP

#include "interfaces/iimage.hpp"
#include "interfaces/icamera.hpp"

namespace optoy::simple_renderer_t {
    template<class TScalar>
    void render(
        std::proxy<image2d::interface_t>& film,
        const std::proxy<scene::interface_t<TScalar>>& scene,
        const std::proxy<camera::interface_t<TScalar>>& camera,
        uint32_t hops = 0) {

        const auto w = film.invoke<image2d::width_f>();
        const auto h = film.invoke<image2d::height_f>();

        for (image2d::coord_t iy = 0; iy < h; ++iy) {
            const TScalar y = (TScalar)iy / (TScalar)h - (TScalar)0.5;
            for (image2d::coord_t ix = 0; ix < w; ++ix) {
                const TScalar x = (TScalar)ix / (TScalar)w - (TScalar)0.5;

            }
        }
        camera.invoke<camera::generate_ray_f>();
    }
} // namespace optoy

#endif // !OPTOY_SIMPLE_RENDERER_HPP
