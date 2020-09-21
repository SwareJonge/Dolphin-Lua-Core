// Copyright 2020 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "DolphinWX/LUA_API.h"
#include "DolphinWX/LuaScripting.h"
#include "Core/State.h"

namespace Lua
{
	namespace savestate
	{
        int load(lua_State *L)
		{
	        std::string filename = lua_tostring(L, 1);
	        State::LoadAs(filename);
	        return 0;
		}

		int loadSlot(lua_State *L)
		{
	        int slot = lua_tointeger(L, 1);
	        State::Load(slot);
	        return 0;
		}

		int save(lua_State *L)
		{
	        std::string filename = lua_tostring(L, 1);
	        State::SaveAs(filename);
	        return 0;
		}

        int saveSlot(lua_State *L)
        {
	        int slot = lua_tointeger(L, 1);
	        State::Save(slot);
	        return 0;
        }
	}
}