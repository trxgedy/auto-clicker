import <Windows.h>;
import std;

export import Keybind;

namespace keybind
{
	std::unordered_map<int, std::string> key_names =
	{
		{ VK_RBUTTON, "Mouse 2" },
		{ VK_MBUTTON, "Mouse 3" },
		{ VK_XBUTTON1, "Mouse 4" },
		{ VK_XBUTTON2, "Mouse 5" },
		{ VK_BACK, "Back" },
		{ VK_TAB, "Tab" },
		{ VK_CLEAR, "Clear" },
		{ VK_RETURN, "Enter" },
		{ VK_SHIFT, "Shift" },
		{ VK_CONTROL, "Ctrl" },
		{ VK_MENU, "Alt" },
		{ VK_PAUSE, "Pause" },
		{ VK_CAPITAL, "Caps Lock" },
		{ VK_ESCAPE, "Escape" },
		{ VK_SPACE, "Space" },
		{ VK_PRIOR, "Page Up" },
		{ VK_NEXT, "Page Down" },
		{ VK_END, "End" },
		{ VK_HOME, "Home" },
		{ VK_LEFT, "Left Key" },
		{ VK_UP, "Up Key" },
		{ VK_RIGHT, "Right Key" },
		{ VK_DOWN, "Down Key" },
		{ VK_SELECT, "Select" },
		{ VK_PRINT, "Print Screen" },
		{ VK_INSERT, "Insert" },
		{ VK_DELETE, "Delete" },
		{ VK_HELP, "Help" },
		{ VK_SLEEP, "Sleep" },
		{ VK_MULTIPLY, "*" },
		{ VK_ADD, "+" },
		{ VK_SUBTRACT, "-" },
		{ VK_DECIMAL, "." },
		{ VK_DIVIDE, "/" },
		{ VK_NUMLOCK, "Num Lock" },
		{ VK_SCROLL, "Scroll" },
		{ VK_LSHIFT, "Left Shift" },
		{ VK_RSHIFT, "Right Shift" },
		{ VK_LCONTROL, "Left Ctrl" },
		{ VK_RCONTROL, "Right Ctrl" },
		{ VK_LMENU, "Left Alt" },
		{ VK_RMENU, "Right Alt" },
	};

	void keybinder( int &bind, std::string &msg )
	{
		bool done = false;

		while( !done )
		{
			for ( int i = 0; i <= 165; ++i )
			{
				if ( GetAsyncKeyState( i ) & 1 )
				{
					if ( i == VK_LBUTTON && bind != 0 ) // unbinds the key
					{
						bind = 0;
						msg = "bind";
						done = true;
						break;
					}

					done = true;
					bind = i;

					if( i >= 0x30 && i <= 0x5A ) // 0 - Z
						msg = ( char )i;

					if ( i >= 0x70 && i <= 0x7B ) // F1 - F12;
						msg = "F" + std::to_string( i - 0x70 + 1 );

					for ( const auto &pair : key_names )
					{
						if ( i == pair.first )
						{
							bind = pair.first;
							msg = pair.second;

							std::cout  << std::hex  << i << " " << pair.first << "\n";
						}
					}
				}
			}

			std::this_thread::sleep_for( std::chrono::milliseconds( 3 ) );
		}
	}

}