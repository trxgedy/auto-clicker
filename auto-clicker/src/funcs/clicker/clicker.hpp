#ifndef clicker_hpp
#define clicker_hpp

namespace clicker
{
	class c_clicker
	{
	public:
		inline static int cps_rate { 1 };

		inline static bool left_toggle { false };
		inline static bool right_toggle { false };
		inline static bool jitter_toggle { false };
		inline static bool inventory_left { false };
		inline static bool inventory_right { false };

	public:
		c_clicker( ) = default;
		~c_clicker( ) = default;

		auto click_thread( ) -> void;
		auto toggle_bind( bool &toggle, int key ) -> void;
		auto send_click( int key, int press, int release, bool jitter ) -> void;
	};
}

#endif