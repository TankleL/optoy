#include <gtest/gtest.h>
#include <optoy.hpp>

namespace ot = optoy;

void draw_test_image(std::proxy<ot::image2d::interface_t> image) {
    const auto w = image.invoke<ot::image2d::width_f>();
    const auto h = image.invoke<ot::image2d::height_f>();
    const double diagonal = std::sqrt((double)(w*w + h*h));
    for (ot::image2d::coord_t y = 0; y < h; ++y) {
        ot::image2d::channel_t r = (double)y / (double)h * 0xff;
        for (ot::image2d::coord_t x = 0; x < w; ++x) {
            ot::image2d::channel_t g = (double)x / (double)w * 0xff;
            ot::image2d::channel_t b = std::sqrt((double)(x * x + y * y)) / diagonal * 256.0;
            image.invoke<ot::image2d::pixel_set_f>(x, y, ot::image2d::pixel_t{r, g, b, 0xff});
        }
    }
}

TEST(IOTests, PngWriter) {
    ot::texture2d_t tex{200, 160};
    draw_test_image(&tex);
    ot::png_writer_t::write_file("test_png_writer0.png", &tex);
}
