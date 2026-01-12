#include "d3d_helpers.h"

ComPtr<ID3D11Device>           g_Device;
ComPtr<ID3D11DeviceContext>    g_Context;
ComPtr<IDXGISwapChain>         g_SwapChain;
ComPtr<ID3D11RenderTargetView> g_RTV;

void CreateRTV() {
    ComPtr<ID3D11Texture2D> bb;
    if (SUCCEEDED(g_SwapChain->GetBuffer(0, IID_PPV_ARGS(&bb))))
        g_Device->CreateRenderTargetView(bb.Get(), nullptr, g_RTV.GetAddressOf());
}

void CleanupD3D() {
    g_RTV.Reset();
    g_SwapChain.Reset();
    g_Context.Reset();
    g_Device.Reset();
}