import <windows.h>;

import std;
import Utils;

export import Clicker;

namespace clicker
{
	void c_clicker::click_thread( )
	{
		while ( true )
		{
			if ( GetAsyncKeyState( left_bind ) & 1 )
				left_active = !left_active;

			if ( GetAsyncKeyState( right_bind ) & 1 )
				right_active = !right_active;

			if ( utils::is_minecraft_foreground( ) || utils::is_specific_window_foreground( std::string( win_title_buffer ) ) )
			{
				if ( !this->active_in_inventory && this->is_cursor_visible( ) ) // if its not supposed to work in the inventory and the cursor is visible: skip
					continue;

				if ( left_active )
					send_click( VK_LBUTTON, WM_LBUTTONDOWN, WM_LBUTTONUP );

				if ( right_active )
					send_click( VK_RBUTTON, WM_RBUTTONDOWN, WM_RBUTTONUP );
			}

			std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
		}
	}

	void c_clicker::send_click( int mouse_button, int press, int release )
	{
		if ( GetAsyncKeyState( mouse_button ) & 0x8000 )
		{
			if ( this->jitter_active )
				this->shake_cursor( );

			SendMessageA( GetForegroundWindow( ), press, press == WM_LBUTTONDOWN ? MK_LBUTTON : MK_RBUTTON, 0 );
			SendMessageA( GetForegroundWindow( ), release, release == WM_LBUTTONDOWN ? MK_LBUTTON : MK_RBUTTON, 0 );

			std::this_thread::sleep_for( std::chrono::milliseconds( 1000 / ( ( this->cps * 2 ) + utils::random_int( -3, 5 ) ) ) );
		}
	}

	bool c_clicker::is_cursor_visible( )
	{
		CURSORINFO cursor_info { sizeof( CURSORINFO ) };
		GetCursorInfo( &cursor_info );

		return( cursor_info.hCursor > ( HCURSOR ) 0xC350 ) && ( cursor_info.hCursor < ( HCURSOR ) 0x186A0 );
	}

	void c_clicker::shake_cursor( )
	{
		GetCursorPos( &this->cursor_pos );
		SetCursorPos( this->cursor_pos.x - this->jitter_intensity, this->cursor_pos.y - this->jitter_intensity );

		this->jitter_intensity *= -1;
	}

}