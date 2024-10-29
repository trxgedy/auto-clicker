module;

import <Windows.h>;
import std;

export module Clicker;

export namespace clicker
{
	class c_clicker
	{
	public:
		inline static bool left_active = false;
		inline static bool right_active = false;
		inline static bool jitter_active = true;
		inline static bool active_in_inventory = false;

		inline static int cps = 15;
		inline static int jitter_intensity = 5;

		inline static int left_bind = 'F';
		inline static int right_bind = 0;

		inline static std::string left_bind_msg { "bind" };
		inline static std::string right_bind_msg { "bind" };

		inline static char win_title_buffer[ 128 ] { };

		c_clicker( ) = default;
		~c_clicker( ) = default;

		void click_thread( );

	private:
		POINT cursor_pos {};

		void send_click( int mouse_button, int press, int release );
		void shake_cursor( );
	};
}