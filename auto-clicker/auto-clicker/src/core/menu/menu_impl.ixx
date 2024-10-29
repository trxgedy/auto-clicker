#include "ext\imgui\imgui.h"
#include "ext\imgui\imgui_impl_win32.h"
#include "ext\imgui\imgui_impl_dx11.h"
#include <d3d11.h>

import <Windows.h>;

import std;

import Widgets;
import Fonts;
import Clicker;
import Keybind;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

export import Menu;

namespace menu
{
    LRESULT WINAPI wnd_proc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
    {
        if ( ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam ) )
            return true;

        switch ( msg )
        {
            case WM_DESTROY:
                ::PostQuitMessage( 0 );
                return 0;
        }
        return ::DefWindowProcA( hWnd, msg, wParam, lParam );
    }

    bool c_window::create_device( )
    {
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory( &sd, sizeof( sd ) );
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = w_context.hwnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = 0;
        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[ 2 ] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
        HRESULT res = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &w_context.swap_chain, &w_context.g_pd3dDevice, &featureLevel, &w_context.device );
        if ( res == DXGI_ERROR_UNSUPPORTED )
            res = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &w_context.swap_chain, &w_context.g_pd3dDevice, &featureLevel, &w_context.device );
        if ( res != S_OK )
            return false;

        ID3D11Texture2D *pBackBuffer;
        w_context.swap_chain->GetBuffer( 0, IID_PPV_ARGS( &pBackBuffer ) );
        w_context.g_pd3dDevice->CreateRenderTargetView( pBackBuffer, nullptr, &w_context.target_view );
        pBackBuffer->Release( );

        return true;
    }

    void c_window::cleanup_device( )
    {
        if ( w_context.target_view )
        {
            w_context.target_view->Release( );
            w_context.target_view = nullptr;
        }

        if ( w_context.swap_chain )
        {
            w_context.swap_chain->Release( );
            w_context.swap_chain = nullptr;
        }
        if ( w_context.device )
        {
            w_context.device->Release( );
            w_context.device = nullptr;
        }
        if ( w_context.g_pd3dDevice )
        {
            w_context.g_pd3dDevice->Release( );
            w_context.g_pd3dDevice = nullptr;
        }
    }

    bool c_window::create_window( )
    {
        try
        {
            w_context.wc = { sizeof( w_context.wc ), CS_CLASSDC, wnd_proc, 0L, 0L, GetModuleHandle( nullptr ), nullptr, nullptr, nullptr, nullptr, "window_class", nullptr };
            RegisterClassExA( &w_context.wc );

            w_context.hwnd = CreateWindowA( w_context.wc.lpszClassName, window_name, WS_POPUP, 100, 100, w_size.x, w_size.y, nullptr, nullptr, w_context.wc.hInstance, nullptr );

            if ( !create_device( ) )
            {
                cleanup_device( );
                UnregisterClassA( w_context.wc.lpszClassName, w_context.wc.hInstance );
                return true;
            }

            const auto init_centered = [ ]( HWND hwnd )
            {
                RECT rc;
                GetWindowRect( hwnd, &rc );

                auto size = std::make_pair( GetSystemMetrics( SM_CXSCREEN ) - rc.right, GetSystemMetrics( SM_CYSCREEN ) - rc.bottom );

                SetWindowPos( hwnd, nullptr, size.first / 2, size.second / 2, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
            };

            init_centered( w_context.hwnd );

            ShowWindow( w_context.hwnd, SW_SHOWDEFAULT );
            UpdateWindow( w_context.hwnd );

            ImGui::CreateContext( );

            ImGui::StyleColorsDark( );

            ImGui_ImplWin32_Init( w_context.hwnd );
            ImGui_ImplDX11_Init( w_context.g_pd3dDevice, w_context.device );
        }
        catch ( std::exception &e )
        {
            cleanup_device( );

            MessageBoxA( nullptr, e.what( ), nullptr, MB_OK | MB_ICONERROR );
            return false;
        }

        return false;
    }

    void c_window::render_window( char *name, float width, float height )
    {
        this->window_name = name;
        this->w_size = { width, height};

        create_window( );
        menu_style( );

        MSG msg;
        std::memset( &msg, 0, sizeof( msg ) );

        while ( msg.message != WM_QUIT )
        {
            if ( !w_context.context_state )
                msg.message = WM_QUIT;

            while ( ::PeekMessageA( &msg, nullptr, 0U, 0U, PM_REMOVE ) )
            {
                ::TranslateMessage( &msg );
                ::DispatchMessageA( &msg );
                continue;
            }

            if ( w_context.swap_chain_oc && w_context.swap_chain->Present( 0, DXGI_PRESENT_TEST ) == DXGI_STATUS_OCCLUDED )
            {
                ::Sleep( 10 );
                continue;
            }

            w_context.swap_chain_oc = false;

            if ( w_context.g_ResizeWidth != 0 && w_context.g_ResizeHeight != 0 )
            {
                if ( w_context.target_view )
                {
                    w_context.target_view->Release( );
                    w_context.target_view = nullptr;
                }

                w_context.swap_chain->ResizeBuffers( 0, w_context.g_ResizeWidth, w_context.g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0 );
                w_context.g_ResizeWidth = w_context.g_ResizeHeight = 0;

                ID3D11Texture2D *pBackBuffer;
                w_context.swap_chain->GetBuffer( 0, IID_PPV_ARGS( &pBackBuffer ) );
                w_context.g_pd3dDevice->CreateRenderTargetView( pBackBuffer, nullptr, &w_context.target_view );
                pBackBuffer->Release( );
            }

            SetWindowLongA( w_context.hwnd, GWL_EXSTYLE, WS_EX_LAYERED );
            SetLayeredWindowAttributes( w_context.hwnd, RGB( 0, 0, 0 ), 255, LWA_COLORKEY | LWA_ALPHA );

            ImGui_ImplDX11_NewFrame( );
            ImGui_ImplWin32_NewFrame( );
            ImGui::NewFrame( );
            {
                menu_design( );
            }
            ImGui::Render( );

            const float clear_color_with_alpha[ 4 ] = { 0,0,0,0 };
            w_context.device->OMSetRenderTargets( 1, &w_context.target_view, nullptr );
            w_context.device->ClearRenderTargetView( w_context.target_view, clear_color_with_alpha );
            ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

            HRESULT hr = w_context.swap_chain->Present( 1, 0 );
            w_context.swap_chain_oc = ( hr == DXGI_STATUS_OCCLUDED );
        }

        ImGui_ImplDX11_Shutdown( );
        ImGui_ImplWin32_Shutdown( );
        ImGui::DestroyContext( );

        cleanup_device( );
        DestroyWindow( w_context.hwnd );
        UnregisterClassA( w_context.wc.lpszClassName, w_context.wc.hInstance );
    }

    void c_window::menu_style( )
    {
        static auto &io = ImGui::GetIO( );
        static auto &style = ImGui::GetStyle( );

        lexend = io.Fonts->AddFontFromMemoryCompressedBase85TTF( fonts::lexend_font.data( ), 20 );

        io.LogFilename = nullptr;
        io.IniFilename = nullptr;

        style.Colors[ ImGuiCol_WindowBg ] = ImColor( 12, 12, 12 );
        style.Colors[ ImGuiCol_ChildBg ] = ImColor( 15,15,15 );
        
        style.Colors[ ImGuiCol_Button ] = ImColor( 61,133,224 );

        style.Colors[ ImGuiCol_FrameBg ] = ImColor( 23,23,23 );
        style.Colors[ ImGuiCol_FrameBgHovered ] = ImColor( 25,25,25 );
        style.Colors[ ImGuiCol_FrameBgActive ] = ImColor( 25,25,25 );

        style.WindowPadding = { 0, 0 };
        style.WindowBorderSize = 0;
        style.WindowRounding = 5;
        style.ChildRounding = 5;
        style.FrameRounding = 5;
        style.GrabRounding = 5;
    }

    void c_window::menu_design( )
    {
        constexpr auto flags { ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize };

        ImGui::SetNextWindowPos( { 0, 0 }, ImGuiCond_::ImGuiCond_Always );
        ImGui::SetNextWindowSize( w_size, ImGuiCond_::ImGuiCond_Always );

        ImGui::Begin( "###main_painel", nullptr, flags );
        {
            ImGui::InvisibleButton( "##move", { w_size.x - 21, 30 } );
            if ( ImGui::IsItemActive( ) )
            {
                RECT rect;
                GetWindowRect( GetActiveWindow( ), &rect );
                SetWindowPos( GetActiveWindow( ), nullptr, rect.left + ImGui::GetMouseDragDelta( ).x, rect.top + ImGui::GetMouseDragDelta( ).y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
            }

            custom::rainbow_rect( { 0, 30 }, w_size.x, 5 );

            ImGui::PushFont( lexend );
            {
                ImGui::SetCursorPos( { 20,5 } );
                ImGui::Text( "CLICKER" );

                ImGui::SetCursorPos( { w_size.x - 20, 5 } );
                ImGui::Text( "X" );
                if ( ImGui::IsItemClicked( ) )
                    w_context.context_state = false;

                ImGui::SetCursorPos( { 20, 50 } );
                ImGui::BeginChild( "left_child", { 370, 430 } );
                {
                    ImGui::SetCursorPos( { 10, 40 } );
                    ImGui::SetNextItemWidth( 350 );
                    ImGui::SliderInt( "CPS", &clicker::c_clicker::cps, 0, 20 );

                    ImGui::SetCursorPos( { 10, 110 } );
                    ImGui::SetNextItemWidth( 350 );
                    ImGui::SliderInt( "Jitter Intensity", &clicker::c_clicker::jitter_intensity, 1, 20 );

                    ImGui::SetCursorPos( { 10, 200 } );
                    ImGui::SetNextItemWidth( 350 );
                    ImGui::InputTextWithHint( "Specific Window", "window title", clicker::c_clicker::win_title_buffer, sizeof( clicker::c_clicker::win_title_buffer ) );

                    ImGui::SetCursorPos( { 10, 310 } );
                    ImGui::SetNextItemWidth( 350 );
                    if ( ImGui::Button( ( clicker::c_clicker::left_active ? "On" : "Off" ), { 165, 35 } ) )
                        clicker::c_clicker::left_active = !clicker::c_clicker::left_active;

                    ImGui::SetCursorPos( { 195, 310 } );
                    ImGui::SetNextItemWidth( 350 );
                    if ( ImGui::Button( clicker::c_clicker::left_bind_msg.c_str( ), { 165, 35 } ) )
                        keybind::keybinder( clicker::c_clicker::left_bind, clicker::c_clicker::left_bind_msg );
                }
                ImGui::EndChild( );

                ImGui::SetCursorPos( { 410, 50 } );
                ImGui::BeginChild( "right_child", { 370, 430 } );
                {
                    ImGui::SetCursorPos( { 10, 40 } );
                    ImGui::BeginGroup( );
                    {
                        ImGui::Checkbox( "Jitter", &clicker::c_clicker::jitter_active );
                        ImGui::Checkbox( "Active in Inventory", &clicker::c_clicker::active_in_inventory );
                        ImGui::Checkbox( "Right Click", &clicker::c_clicker::right_active );

                        ImGui::SetCursorPos( { 240, 100 } );
                        if ( ImGui::Button( clicker::c_clicker::right_bind_msg.c_str( ), {110,26} ) )
                            keybind::keybinder( clicker::c_clicker::right_bind, clicker::c_clicker::right_bind_msg );
                    }
                    ImGui::EndGroup( );

                    ImGui::EndChild( );
                }
                ImGui::PopFont( );
            }
            ImGui::End( );
        }
    }
}