#pragma once

namespace winrt::raydbg::entities {
    template<class T>
    struct point_t {
        T x, y, z;
    };

    template<class T>
    struct vector_t {
        T x, y, z;
    };

    template<class T>
    struct ray_t {
        point_t o;
        vector_t d;
    };

    template<class T>
    struct line_segment_t {
        point_t o;
        vector_t d;
        T tmin, tmax;
    };

    template<class T>
    struct sphere_t {
        T thetamin, thetamax;
        T phimin, phimax;
    };

    template<class T>
    struct transformation {
        T m[4][4];
    };
}

