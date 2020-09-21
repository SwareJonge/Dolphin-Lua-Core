// Copyright 2020 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "DolphinWX/LUA_API.h"
#include "DolphinWX/LuaScripting.h"

#include "Core/Core.h"
#include "VideoCommon/Statistics.h"

namespace Lua
{
	namespace gui
	{
    int DrawText(lua_State *L)
    {
	    int argc = lua_gettop(L);
	    // check if there are less than 3 arguments
	    if (argc < 1)
	    {
		    return 0; // if so don't execute further
	    }

	    std::string text;
	    text = StringFromFormat("%s", lua_tostring(L, 1));
	    text.append("\n");
	    Statistics::SetString(text);

	    return 0;
    }

    int Msg(lua_State *L)
    {
	    int argc = lua_gettop(L);

	    if (argc < 1)
		    return 0;

	    const char *text = lua_tostring(L, 1);

	    int delay = 5000; // Default: 5 seconds
	    // if there is a second argument let user define delay
	    if (argc == 2)
	    {
		    delay = lua_tointeger(L, 2);
	    }

	    std::string message = StringFromFormat("Lua Msg: %s", text);

	    Core::DisplayMessage(message, delay);

	    return 0; // number of return values
    }
	}
}