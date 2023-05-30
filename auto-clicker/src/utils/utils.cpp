#include "stdafx.hpp"

namespace utils
{
	auto c_utils::shake_cursor( ) -> void
	{
		if ( GetPhysicalCursorPos( &point ) )
		{
			SetPhysicalCursorPos( point.x + ( pixels ), point.y - ( pixels ) );
			pixels = pixels * -1;
		}
	}

	auto c_utils::is_cursor_visible( ) -> bool
	{
		CURSORINFO ci { sizeof( CURSORINFO ) }; GetCursorInfo( &ci );
		const auto handle = ci.hCursor;

		return ( handle > ( HCURSOR ) 0xC350 ) && ( handle < ( HCURSOR ) 0x186A0 );
	}

	auto c_utils::random_int( int min, int max ) -> int
	{
		std::srand( std::time( nullptr ) );
		return std::rand( ) % max + min;
	}

	auto c_utils::to_lower( std::string str ) -> std::string
	{
		std::transform( str.begin( ), str.end( ), str.begin( ), ::tolower );
		return str;
	}

	auto c_utils::foreground_win_title( ) -> std::string
	{
		char win_title[ 128 ] {}; 
		GetWindowTextA ( GetForegroundWindow( ), win_title, 128 );

		return to_lower(win_title); 
	}

	auto c_utils::is_javaw_foreground( ) -> bool
	{
		if ( window_title_buffer.empty( ) )
			return GetForegroundWindow( ) == FindWindow( "LWJGL", nullptr ) || FindWindow( "GLFW30", nullptr );
		else
			return foreground_win_title( ).find( to_lower( window_title_buffer ) ) != std::string::npos;
	}
}