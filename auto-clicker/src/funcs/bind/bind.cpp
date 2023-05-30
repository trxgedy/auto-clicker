#include "stdafx.hpp"
#include "bind.hpp"

namespace bind
{
	static std::unordered_map<int, std::string> key_map
	{
		{ 0x30, "0" }, { 0x31, "1" }, { 0x32, "2" },
		{ 0x33, "3" }, { 0x34, "4" }, { 0x35, "5" },
		{ 0x36, "6" }, { 0x37, "7" }, { 0x38, "8" },
		{ 0x39, "9" }, { 0x41, "A" }, { 0x42, "B" },
		{ 0x43, "C" }, { 0x44, "D" }, { 0x45, "E" },
		{ 0x46, "F" }, { 0x47, "G" }, { 0x48, "H" },
		{ 0x49, "I" }, { 0x4A, "J" }, { 0x4B, "K" },
		{ 0x4C, "L" }, { 0x4D, "M" }, { 0x4E, "N" },
		{ 0x4F, "O" }, { 0x50, "P" }, { 0x51, "Q" },
		{ 0x52, "R" }, { 0x53, "S" }, { 0x54, "T" },
		{ 0x55, "U" }, { 0x56, "V" }, { 0x57, "W" },
		{ 0x58, "X" }, { 0x59, "Y" }, { 0x5A, "Z" },

		{ 0x60,	"Numpad 0" }, { 0x61,	"Numpad 1" },
		{ 0x62,	"Numpad 2" }, { 0x63,	"Numpad 3" },
		{ 0x64,	"Numpad 4" }, { 0x65,	"Numpad 5" }, 
		{ 0x66,	"Numpad 6" }, { 0x67,	"Numpad 7" },
		{ 0x68,	"Numpad 8" }, { 0x69,	"Numpad 9" },

		{ VK_MBUTTON, "Mouse 3" },     { VK_XBUTTON1, "Mouse 4" },
		{ VK_XBUTTON2, "Mouse 5" },    { VK_BACK, "Back" },
		{ VK_TAB, "Tab" },             { VK_CLEAR, "Clear" },
		{ VK_RETURN, "Enter" },        { VK_SHIFT, "Shift" },
		{ VK_CONTROL, "Ctrl" },        { VK_MENU, "Alt" },
		{ VK_PAUSE, "Pause" },         { VK_CAPITAL, "Caps Lock" },
		{ VK_ESCAPE, "Escape" },       { VK_SPACE, "Space" },
		{ VK_PRIOR, "Page Up" },       { VK_NEXT, "Page Down" },
		{ VK_END, "End" },             { VK_HOME, "Home" },
		{ VK_LEFT, "Left Key" },       { VK_UP, "Up Key" },
		{ VK_RIGHT, "Right Key" },     { VK_DOWN, "Down Key" },
		{ VK_SELECT, "Select" },       { VK_PRINT, "Print Screen" },
		{ VK_INSERT, "Insert" },       { VK_DELETE, "Delete" },
		{ VK_HELP, "Help" },           { VK_SLEEP, "Sleep" },
		{ VK_MULTIPLY, "*" },          { VK_ADD, "+" },
		{ VK_SUBTRACT, "-" },          { VK_DECIMAL, "." },
		{ VK_DIVIDE, "/" },            { VK_NUMLOCK, "Num Lock" },
		{ VK_SCROLL, "Scroll" },       { VK_LSHIFT, "Left Shift" },
		{ VK_RSHIFT, "Right Shift" },  { VK_LCONTROL, "Left Ctrl" },
		{ VK_RCONTROL, "Right Ctrl" }, { VK_LMENU, "Left Alt" },
		{ VK_RMENU, "Right Alt" },	   { VK_OEM_3, "Grave" },
	};

	auto c_bind::keybinder(int &side, std::string &msg ) -> void
	{
		side = 0;
		msg = "None";

		while ( !side )
		{
			for ( auto key : bind::key_map )
			{
				for ( int i { 0 }; i < 256; i++ )
				{
					if ( GetAsyncKeyState( i ) && key.first == i )
					{
						msg = key.second;
						side = key.first;
					}
				}
			}
			std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) );
		}
	}

};