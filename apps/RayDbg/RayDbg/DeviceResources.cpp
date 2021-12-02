#include "pch.h"
#include "DeviceResources.h"
#include "DirectXHelper.h"

namespace dx {
	DeviceResources::DeviceResources()
		: _screenViewport()
		, _d3dfeaturelevel(D3D_FEATURE_LEVEL_11_1) {
		_create_dires();
		_create_ddres();
	}

	void DeviceResources::_create_dires() {
	}

	void DeviceResources::_create_ddres() {
		// This flag adds support for surfaces with a different color channel ordering
		// than the API default. It is required for compatibility with Direct2D.
		uint32_t creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
		if (sdk_layers_avaliable()) {
			creation_flags = creation_flags | D3D11_CREATE_DEVICE_DEBUG;
		}
#endif

		D3D_FEATURE_LEVEL featureLevels[] = 
		{
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		winrt::com_ptr<ID3D11Device> dev;
		winrt::com_ptr<ID3D11DeviceContext> ctx;

		const auto hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			creation_flags,
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			dev.put(),
			&_d3dfeaturelevel,
			ctx.put());

		if (FAILED(hr)) { // fall back to the WARP device
			throw_if_failed(
				D3D11CreateDevice(
					nullptr,
					D3D_DRIVER_TYPE_WARP,
					0,
					creation_flags,
					featureLevels,
					ARRAYSIZE(featureLevels),
					D3D11_SDK_VERSION,
					dev.put(),
					&_d3dfeaturelevel,
					ctx.put()));
		}
		_d3ddevice = dev.as<ID3D11Device3>();
		_d3dcontext = ctx.as<ID3D11DeviceContext4>();
	}

	void DeviceResources::_create_wsdres() {

	}
}
