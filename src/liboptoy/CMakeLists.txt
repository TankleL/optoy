project(liboptoy)

set(LIBOPTOY_HEADER_FILES
    includes/optoy.hpp
    includes/mathdef.hpp
    includes/integrators/pathint.hpp
    includes/interfaces/icamera.hpp
    includes/interfaces/iscene.hpp
    includes/primitives/geo.hpp
    includes/primitives/intersec.hpp
    includes/primitives/ray.hpp
    includes/primitives/sphere.hpp
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

