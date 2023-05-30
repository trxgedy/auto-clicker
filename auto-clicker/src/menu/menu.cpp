#include "stdafx.hpp"
#include "menu.hpp"

namespace menu
{
	auto c_window::wnd_proc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam ) -> LRESULT WINAPI
	{
		if ( ImGui_ImplWin32_WndProcHandler( hwnd, message, wparam, lparam ) )
			return true;

		switch ( message )
		{
			case WM_QUIT:
				PostQuitMessage( 0 );
				break;

			case WM_NCHITTEST:
				RECT rect {};
				GetWindowRect( hwnd, &rect );

				POINT current_pos {};
				GetCursorPos( &current_pos );

				auto hit = DefWindowProcA( hwnd, message, wparam, lparam );
				if ( hit == HTCLIENT && ( current_pos.y < ( rect.top + 15 ) ) )
					return HTCAPTION;

				break;
		}

		return DefWindowProcA( hwnd, message, wparam, lparam );
	}

	auto c_window::create( const std::string w_name, const ImVec2 w_size, context_t &w_context, const std::function<void( context_t & )> w_styles ) -> bool
	{
		try
		{
			const auto init_centered = [ ]( HWND hwnd )
			{
				RECT rc;
				GetWindowRect( hwnd, &rc );

				auto size = std::make_pair( GetSystemMetrics( SM_CXSCREEN ) - rc.right, GetSystemMetrics( SM_CYSCREEN ) - rc.bottom );

				SetWindowPos( hwnd, nullptr, size.first / 2, size.second / 2, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
			};

			const auto create_device = [ & ]( )
			{
				w_context.d3d = Direct3DCreate9( D3D_SDK_VERSION );
				if ( w_context.d3d == nullptr )
					return false;

				std::memset( &w_context.params, 0, sizeof( w_context.params ) );

				w_context.params.Windowed = true;
				w_context.params.SwapEffect = D3DSWAPEFFECT_DISCARD;
				w_context.params.BackBufferFormat = D3DFMT_UNKNOWN;
				w_context.params.EnableAutoDepthStencil = true;
				w_context.params.AutoDepthStencilFormat = D3DFMT_D16;
				w_context.params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

				if ( w_context.d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, w_context.hwnd, 0x00000040L, &w_context.params, &w_context.device ) < 0 )
					return false;

				return true;
			};

			w_context.wc =
			{
				sizeof( WNDCLASSEXA ),
				CS_CLASSDC,
				( WNDPROC ) wnd_proc,
				0L,
				0L,
				GetModuleHandleA( nullptr ),
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				w_name.data( ),
				nullptr
			};

			RegisterClassExA( &w_context.wc );

			w_context.hwnd = CreateWindowExA( 0, w_context.wc.lpszClassName, w_context.wc.lpszClassName,
											  WS_POPUP, 100, 100, w_size.x, w_size.y,
											  nullptr, nullptr, w_context.wc.hInstance, nullptr );

			if ( w_context.hwnd == nullptr )
				throw std::exception( "error while creating the window" );

			if ( !create_device( ) )
				throw std::exception( "cannot create device!" );

			init_centered( w_context.hwnd );

			ShowWindow( w_context.hwnd, SW_SHOWDEFAULT );
			UpdateWindow( w_context.hwnd );

			ImGui::CreateContext( );

			w_styles( w_context );

			ImGui_ImplWin32_Init( w_context.hwnd );
			ImGui_ImplDX9_Init( w_context.device );

			return true;
		}
		catch ( std::exception &e )
		{
			cleanup_device( w_context );

			MessageBoxA( nullptr, e.what( ), nullptr, MB_OK | MB_ICONERROR );
			return false;
		}

		return false;
	}

	auto c_window::render( context_t &w_context, const ImVec2 w_size, const std::function<void( context_t &, const ImVec2 )> menu ) -> void
	{
		MSG msg;
		std::memset( &msg, 0, sizeof( msg ) );

		while ( msg.message != WM_QUIT )
		{
			if ( PeekMessageA( &msg, nullptr, 0U, 0U, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessageA( &msg );
				continue;
			}

			ImGui_ImplDX9_NewFrame( );
			ImGui_ImplWin32_NewFrame( );
			ImGui::NewFrame( );

			menu( w_context, w_size );

			ImGui::EndFrame( );

			w_context.device->Clear( 0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );

			if ( !w_context.context_state )
			{
				msg.message = WM_QUIT;
			}

			if ( w_context.device->BeginScene( ) >= 0 )
			{
				ImGui::Render( );
				ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
				w_context.device->EndScene( );
			}

			if ( w_context.device->Present( nullptr, nullptr, nullptr, nullptr ) == D3DERR_DEVICELOST && w_context.device->TestCooperativeLevel( ) == D3DERR_DEVICENOTRESET )
			{
				ImGui_ImplDX9_InvalidateDeviceObjects( );
				w_context.device->Reset( &w_context.params );
				ImGui_ImplDX9_CreateDeviceObjects( );
			}
		}

		ImGui_ImplDX9_Shutdown( );
		ImGui_ImplWin32_Shutdown( );
		ImGui::DestroyContext( );

		cleanup_device( w_context );

		DestroyWindow( w_context.hwnd );
		UnregisterClassA( w_context.wc.lpszClassName, w_context.wc.hInstance );
	}

	auto c_render::menu_style( context_t &w_context ) -> void
	{
		auto &io = ImGui::GetIO( );
		auto &style = ImGui::GetStyle( );

		io.IniFilename = nullptr;
		io.LogFilename = nullptr;

		roboto_bd = io.Fonts->AddFontFromMemoryCompressedBase85TTF( fonts::roboto_bold.data( ), 18 );
		roboto = io.Fonts->AddFontFromMemoryCompressedBase85TTF( fonts::roboto_black.data( ), 16 );

		style.WindowPadding = { 0,0 };
		style.WindowBorderSize = 0;

		style.Colors[ ImGuiCol_WindowBg ] = ImColor( 30, 30, 30 );
		style.Colors[ ImGuiCol_Border ] = ImColor( 0, 0, 0 );

		style.Colors[ ImGuiCol_FrameBg ] = ImColor( 45, 45, 45  );
		style.Colors[ ImGuiCol_FrameBgActive ] = ImColor( 38, 38, 38 );
		style.Colors[ ImGuiCol_FrameBgHovered ] = ImColor( 45, 45, 45 );

		style.Colors[ ImGuiCol_Button ] = ImColor( 32, 82, 149  );
		style.Colors[ ImGuiCol_ButtonHovered ] = ImColor( 20, 66, 114 );
		style.Colors[ ImGuiCol_ButtonActive ] = ImColor( 20, 66, 114 );

		style.Colors[ ImGuiCol_SliderGrab ] = ImColor( 20, 66, 114 );
		style.Colors[ ImGuiCol_SliderGrabActive ] = ImColor( 20, 66, 114 );

		style.Colors[ ImGuiCol_CheckMark ] = ImColor( 32, 82, 149 );
	}

	auto c_render::menu_panel( context_t &w_context, const ImVec2 w_size ) -> void
	{
		static auto binds_ { std::make_unique<bind::c_bind>( ) };
		static auto clicker_ { std::make_unique<clicker::c_clicker>( ) };
		static auto utils_ { std::make_unique<utils::c_utils>( ) };

		static auto flags { ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize };

		ImGui::SetNextWindowPos( { 0, 0 }, ImGuiCond_::ImGuiCond_Always );
		ImGui::SetNextWindowSize( w_size, ImGuiCond_::ImGuiCond_Always );

		ImGui::Begin( "###main_painel", nullptr, flags );
		{
			static auto *draw = ImGui::GetWindowDrawList( );

			ImGui::PushFont( roboto_bd );
			{
				// Top
				draw->AddLine( { 0, 25 }, { 650, 25 }, ImColor( 32, 82, 149 ), 2.0f );

				ImGui::SetCursorPos( { 8, 4 } );
				ImGui::Text( "Auto-Clicker" );
				ImGui::SetCursorPos( { 650 - 25, 0 } );
				if ( ImGui::Button( "X", { 25, 25 } ) )
					w_context.context_state = false;
			}
			ImGui::PopFont( );

			ImGui::PushFont( roboto );
			{
				// Left Side
				draw->AddRectFilled( { 10, 70 }, { 350, 340 }, ImColor( 35, 35, 35 ), 20.0f );

				ImGui::PushItemWidth( 315 );
				{
					// CPS Slider
					ImGui::SetCursorPos( { 20, 100 } );
					ImGui::Text( "CPS" );
					ImGui::SetCursorPos( { 20, 120 } );
					ImGui::SliderInt( "###slider_cps", &clicker_->cps_rate, 1, 20, "%d" );

					// Jitter Slider
					ImGui::SetCursorPos( { 20, 170 } );
					ImGui::Text( "Jitter Intensity" );
					ImGui::SetCursorPos( { 20, 190 } );
					ImGui::SliderInt( "###slider_jitter", &utils_->pixels, 1, 20, "%d" );

					// Custom Window
					ImGui::SetCursorPos( { 20, 240 } );
					ImGui::Text( "Specific Window" );

					ImGui::SetCursorPos( { 130, 240 } );
					ImGui::TextColored( { 0.45f, 0.45f, 0.45f, 1.0f }, "(?)" );
					if ( ImGui::IsItemHovered ( 0 ) )
						ImGui::SetTooltip( "use this if your minecraft window wasn't found" );

					ImGui::SetCursorPos( { 20, 260 } );
					ImGui::InputTextWithHint( "###specific_window", "window title", &utils_->window_title_buffer);
				}
				ImGui::PopItemWidth( );

				// Right Side
				draw->AddRectFilled( { 375, 70 }, { 635, 340 }, ImColor( 35, 35, 35 ), 20.0f );

				ImGui::SetCursorPos( { 390, 100 } );
				ImGui::Text( "Left Keybind" );

				ImGui::SetCursorPos( { 390, 120 } );
				if ( ImGui::Button( binds_->left_bind_msg.c_str( ), { 90, 20 } ) )
					binds_->keybinder( binds_->left_bind, binds_->left_bind_msg );

				ImGui::SetCursorPos( { 520, 100 } );
				ImGui::Text( "Right Keybind" );

				ImGui::SetCursorPos( { 520, 120 } );
				if ( ImGui::Button( binds_->right_bind_msg.c_str( ), { 90, 20 } ) )
					binds_->keybinder( binds_->right_bind, binds_->right_bind_msg );

				ImGui::SetCursorPos( { 390, 170 } );
				ImGui::Checkbox( "Use same bind for both", &binds_->same_bind );

				ImGui::SetCursorPos( { 390, 200 } );
				ImGui::Checkbox( "Jitter", &clicker_->jitter_toggle );

				ImGui::SetCursorPos( { 390, 230 } );
				ImGui::Checkbox( "Turn left-click on in inventory", &clicker_->inventory_left );

				ImGui::SetCursorPos( { 390, 260 } );
				ImGui::Checkbox( "Turn right-click on in inventory", &clicker_->inventory_right );
			}
			ImGui::PopFont( );
		}
		ImGui::End( );
	}
}