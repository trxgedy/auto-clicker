#ifndef stdafx_hpp
#define stdafx_hpp

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <thread>
#include <unordered_map>
#include <functional>
#include <algorithm>

#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

#include "utils\utils.hpp"
#include "funcs\clicker\clicker.hpp"
#include "funcs\bind\bind.hpp"

#include <d3d9.h>                          
#include <d3dx9.h>                        

#include "menu\imgui\imgui.h"
#include "menu\imgui\imgui_stdlib.h"
#include "menu\imgui\imgui_impl_dx9.h"
#include "menu\imgui\imgui_impl_win32.h"

#include "resources.hpp"

#endif