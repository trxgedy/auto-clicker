#ifndef utils_hpp
#define utils_hpp

namespace utils
{
	class c_utils
	{
	public:
		inline static int pixels { 1 };
		inline static std::string window_title_buffer {};
		POINT point;

	public:
		c_utils( ) = default;
		~c_utils( ) = default;

		auto shake_cursor( ) -> void;
		auto is_cursor_visible( ) -> bool;
		auto is_javaw_foreground( ) -> bool;
		auto random_int( int min, int max ) -> int;
		auto to_lower( std::string ) -> std::string;
		auto foreground_win_title( ) -> std::string;
	};
}

#endif