module;

import <windows.h>;
import std;

export module Utils;

namespace string
{
	std::string to_lower( std::string str )
	{
		std::transform( str.begin( ), str.end( ), str.begin( ), ::tolower );
		return str;
	}
}

export namespace utils
{
	bool is_minecraft_foreground( )
	{
		return GetForegroundWindow( ) == FindWindow( "LWJGL", nullptr );
	}

	bool is_specific_window_foreground( std::string win_title )
	{
		if( win_title.empty() )
			return false;

		char foreground_window_title[128];
		GetWindowTextA( GetForegroundWindow( ), foreground_window_title, 128 );

		return ( string::to_lower( foreground_window_title ).find( string::to_lower( win_title ) ) != std::string_view::npos );
	}

	bool is_cursor_visible( )
	{
		CURSORINFO cursor_info { sizeof( CURSORINFO ) };
		GetCursorInfo( &cursor_info );

		return( cursor_info.hCursor > ( HCURSOR ) 0xC350 ) && ( cursor_info.hCursor < ( HCURSOR ) 0x186A0 );
	}

	int random_int( int min, int max)
	{
		std::random_device rd;
		std::mt19937 gen( rd( ) );

		std::uniform_int_distribution<int> dist( min, max );

		return dist( gen );
	}
}