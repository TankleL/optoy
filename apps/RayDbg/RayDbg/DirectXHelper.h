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
}

