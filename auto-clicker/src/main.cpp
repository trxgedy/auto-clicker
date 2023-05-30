#include "stdafx.hpp"
#include "menu\menu.hpp"

auto wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow ) -> int __stdcall
{
	const auto clicker_ { std::make_unique<clicker::c_clicker>( ) };
	const auto window_ { std::make_unique<menu::c_window>( ) };	
	const auto render_ { std::make_unique<menu::c_render>( ) };

	std::thread( &clicker::c_clicker::click_thread, clicker_.get( ) ).detach( );

	menu::context_t ctx {};
	if ( window_->create( " ", { 650, 400 }, ctx, render_->menu_style ) )
	{
		window_->render( ctx, { 650, 400 }, render_->menu_panel );
	}

	return EXIT_SUCCESS;
}