#pragma once
#include <d3d11.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

extern ComPtr<ID3D11Device>           g_Device;
extern ComPtr<ID3D11DeviceContext>    g_Context;
extern ComPtr<IDXGISwapChain>         g_SwapChain;
extern ComPtr<ID3D11RenderTargetView> g_RTV;

void CreateRTV();
void CleanupD3D();