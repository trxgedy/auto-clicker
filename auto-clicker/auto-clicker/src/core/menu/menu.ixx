module;

#include "ext\imgui\imgui.h"
#include "ext\imgui\imgui_impl_win32.h"
#include "ext\imgui\imgui_impl_dx11.h"
#include <d3d11.h>

import <Windows.h>;
import std;

export module Menu;

export namespace menu
{
    struct context_t
    {
        HWND hwnd {};
        WNDCLASSEX wc {};
        bool context_state = { true };

        ID3D11Device *g_pd3dDevice = nullptr;
        ID3D11DeviceContext *device = nullptr;
        IDXGISwapChain *swap_chain = nullptr;
        ID3D11RenderTargetView *target_view = nullptr;

        bool                     swap_chain_oc = { false };
        UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
    };

    class c_window
    {
    public:
        c_window( ) = default;
        ~c_window( ) = default;

        void render_window( char *name, float width, float height );

    private:
        char *window_name { };
        ImVec2 w_size {};
        context_t w_context {};

        ImFont *lexend = nullptr;

        bool create_device( );
        void cleanup_device( );
        bool create_window( );
        void menu_style( );
        void menu_design( );
    };
}