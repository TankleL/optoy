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
        point_t<T> o;
        vector_t<T> d;
    };

    template<class T>
    struct line_segment_t {
        point_t<T> o;
        vector_t<T> d;
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

    struct vertex_t {
        float x, y, z, w;
    };
}

