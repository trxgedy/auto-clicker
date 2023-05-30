#include "stdafx.hpp"
#include "clicker.hpp"

using namespace std::chrono;

namespace clicker
{
	const auto binds = std::make_unique<bind::c_bind>( );
	const auto utils = std::make_unique<utils::c_utils>( );

	auto c_clicker::toggle_bind( bool &toggle, int key ) -> void
	{
		if ( GetAsyncKeyState( key ) )
		{
			toggle = !toggle;
			std::this_thread::sleep_for( milliseconds( 100 ) );
		}
	}

	auto c_clicker::send_click( int key, int press, int release, bool jitter ) -> void
	{
		if ( GetAsyncKeyState( key ) )
		{
			SendMessageW( GetForegroundWindow( ), press, press == WM_LBUTTONDOWN ? MK_LBUTTON : MK_RBUTTON, MAKELPARAM( 0, 0 ) );
			SendMessageW( GetForegroundWindow( ), release, release == WM_LBUTTONDOWN ? MK_LBUTTON : MK_RBUTTON, MAKELPARAM( 0, 0 ) );

			if ( jitter )
				utils->shake_cursor( );
		}		
	}

	auto c_clicker::click_thread( ) -> void
	{
		while ( true )
		{	
			if ( utils->is_javaw_foreground( ) )
			{
				if ( binds->same_bind )
					this->right_toggle = this->left_toggle;

				if ( utils->is_cursor_visible( ) )
				{
					if ( ( !inventory_left && left_toggle ) )
						left_toggle = false;

					if ( GetAsyncKeyState( VK_ESCAPE ) || GetAsyncKeyState( 0x45 ) )
					{
						if ( !left_toggle )
						{
							left_toggle = !left_toggle;
							std::this_thread::sleep_for( milliseconds( 300 ) );
						}

						if ( !right_toggle )
						{
							right_toggle = !right_toggle;
							std::this_thread::sleep_for( milliseconds( 300 ) );
						}
					}
				}

				toggle_bind( left_toggle, binds->left_bind );
				toggle_bind( right_toggle, binds->right_bind );

				if( left_toggle )
					send_click( VK_LBUTTON, WM_LBUTTONDOWN, WM_LBUTTONUP, jitter_toggle );

				if( right_toggle )
					send_click( VK_RBUTTON, WM_RBUTTONDOWN, WM_RBUTTONUP, jitter_toggle );

				if ( this->cps_rate > 10 )
					std::this_thread::sleep_for( milliseconds( 1000 / ( this->cps_rate + utils->random_int( 0, 7) ) ) );
				else
					std::this_thread::sleep_for( milliseconds( 1000 / ( this->cps_rate + utils->random_int( 0, 3 ) ) ) );
			}
			std::this_thread::sleep_for( milliseconds( 1 ) );
		}
	}
}