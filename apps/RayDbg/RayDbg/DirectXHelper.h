#pragma once
#include "pch.h"

namespace dx {
#if defined(_DEBUG)
    inline bool sdk_layers_avaliable() {
        const auto hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_NULL,
            0,
            D3D11_CREATE_DEVICE_DEBUG,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            nullptr,
            nullptr,
            nullptr);
        return SUCCEEDED(hr);
    }
#endif

    inline void throw_if_failed(HRESULT hr) {
        if (FAILED(hr)) {
            winrt::throw_hresult(hr);
        }
    }

    inline float convert_dips_to_pixels(float dips, float dpi) {
        constexpr float dips_per_inch = 96.0f;
        return std::floorf(dips * dpi / dips_per_inch + 0.5f); // round to the nearest integer
    }

    template<class T>
    inline bool equals(T a, T b) {
        return std::islessequal(std::abs(a - b), std::numeric_limits<T>::epsilon());
    }

    inline winrt::com_ptr<ID3DBlob> compile_shader (
        std::string_view const& shader_source,
        std::string_view const& source_name,
        std::string_view const& entry_point,
        std::string_view const& shader_target) {
#if defined(DEBUG) || defined (_DEBUG)
        const UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
#else
        const UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#endif
        winrt::com_ptr<ID3DBlob> code_blob, error_blob;
        const auto hr = D3DCompile(
            shader_source.data(),
            shader_source.length(),
            source_name.length() > 0 ? source_name.data() : nullptr,
            nullptr,
            nullptr,
            entry_point.data(),
            shader_target.data(),
            flags,
            0,
            code_blob.put(),
            error_blob.put());
#if defined(DEBUG) || defined (_DEBUG)
        if (FAILED(hr) && error_blob != nullptr) {
            auto msg = static_cast<const char*>(error_blob->GetBufferPointer());
            OutputDebugStringA(std::format("!!!! SHADER COMPILE ERROR: {0}", msg).c_str());
        }
#endif
        throw_if_failed(hr);
        return code_blob;
    }
}

