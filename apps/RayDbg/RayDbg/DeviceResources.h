#pragma once
#include "pch.h" 

namespace dx {
	class DeviceResources final
	{
	public:
		DeviceResources();
		void set_swapchainpanel(winrt::Windows::UI::Xaml::Controls::SwapChainPanel panel);

	private:
		void _create_dires(); // create device independent resources
		void _create_ddres(); // create device dependent resources
		void _ensure_wsdres(); // create window size dependent resources
		void _update_rtsize(); // update render target size

	private:
		winrt::com_ptr<ID3D11Device3> _d3ddevice;
		winrt::com_ptr<ID3D11DeviceContext4> _d3dcontext;
		winrt::com_ptr<ID3D11RenderTargetView1> _d3drtview;
		winrt::com_ptr<ID3D11DepthStencilView> _d3ddsview;
		winrt::com_ptr<IDXGISwapChain3> _swapchain;
		D3D11_VIEWPORT _screenviewport;
		winrt::Windows::UI::Xaml::Controls::SwapChainPanel _swapchain_panel;
		D3D_FEATURE_LEVEL _d3dfeaturelevel;
		winrt::Windows::Foundation::Size _d3drt_size;
		winrt::Windows::Foundation::Size _output_size;
		winrt::Windows::Foundation::Size _logical_size;
		float _dpi;
		float _composition_scale_x;
		float _composition_scale_y;
		float _effective_composition_scale_x;
		float _effective_composition_scale_y;
		float _effective_dpi;
	};
}


