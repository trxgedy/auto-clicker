import <Windows.h>;

import std;
import Clicker;
import Menu;

int __stdcall WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    const auto clicker_ { std::make_unique<clicker::c_clicker>( ) };
    const auto window_ { std::make_unique<menu::c_window>( ) };

	std::thread( &clicker::c_clicker::click_thread, clicker_.get() ).detach( );

    window_->render_window( "clicker", 800, 500 );

    return EXIT_SUCCESS;
}