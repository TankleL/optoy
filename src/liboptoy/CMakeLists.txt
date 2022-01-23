project(liboptoy)

set(LIBOPTOY_HEADER_FILES
    includes/optoy.hpp
    includes/predef.hpp
    includes/mathutils.hpp
    includes/core/geo.hpp
    includes/core/interaction.hpp
    includes/core/pathint.hpp
    includes/core/scenenode.hpp
    includes/core/ray.hpp
    includes/core/simplecam.hpp
    includes/core/simplerenderer.hpp
    includes/core/sphere.hpp
    includes/core/texture2d.hpp
    includes/core/threadpool.hpp
    includes/interfaces/icamera.hpp
    includes/interfaces/iimage.hpp
    includes/interfaces/iscene.hpp
    includes/interfaces/ishape.hpp
    includes/io/pngwriter.hpp
)

set(LIBOPTOY_SOURCE_FILES
    sources/optoy.cpp
)

add_library(optoy STATIC
    ${LIBOPTOY_HEADER_FILES} # for IDE searching convienence
    ${LIBOPTOY_SOURCE_FILES}
)

add_library(optoy::liboptoy ALIAS optoy)

target_include_directories(optoy PUBLIC includes)

find_package(Eigen3 CONFIG REQUIRED)
target_link_libraries(optoy PUBLIC Eigen3::Eigen)

find_package(lodepng CONFIG REQUIRED)
target_link_libraries(optoy PUBLIC lodepng)

target_precompile_headers(optoy
    PUBLIC
        <algorithm>
        <array>
        <cmath>
        <cassert>
        <format>
        <regex>
        <sstream>
        <stdexcept>
        <string>
        <string_view>
        <unordered_map>
        <vector>
        <Eigen/Dense>
)

if(MSVC)
    target_compile_options(optoy PRIVATE /W4 /WX /Wv:18)
else()
    target_compile_options(optoy PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

set_property(TARGET optoy PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" )

