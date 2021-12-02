#pragma once
namespace dx {
	class DeviceResources final
	{
	public:
		DeviceResources();

	private:
		void _create_dires(); // create device independent resources
		void _create_ddres(); // create device dependent resources
		void _create_wsdres(); // create window size dependent resources

	private:
		winrt::com_ptr<ID3D11Device3> _d3ddevice;
		winrt::com_ptr<ID3D11DeviceContext4> _d3dcontext;
		winrt::com_ptr<IDXGISwapChain3> _swapchain;
		D3D11_VIEWPORT _screenViewport;
		winrt::Windows::UI::Xaml::Controls::SwapChainPanel _swapchain_panel;
		D3D_FEATURE_LEVEL _d3dfeaturelevel;
	};
}


