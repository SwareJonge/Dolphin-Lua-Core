// Copyright 2020 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "DolphinWX/LUA_API.h"

namespace Lua
{
	namespace bit
	{
        int band(lua_State *L)        
		{	   
			lua_pushinteger(L, lua_tointeger(L, 1) & lua_tointeger(L, 2));	    
			return 1;    
		}
	}
}