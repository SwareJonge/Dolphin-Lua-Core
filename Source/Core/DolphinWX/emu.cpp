// Copyright 2020 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "DolphinWX/LUA_API.h"
#include "DolphinWX/LuaScripting.h"

#include "Core/Movie.h"
#include "Core/HW/ProcessorInterface.h"
#include "Core/ConfigManager.h"

namespace Lua
{
	namespace emu
	{
		int frameAdvance(lua_State *L)
		{
			u64 current_frame = Movie::GetCurrentFrame();
			Movie::DoFrameStep();

			// Block until a frame has actually processed
			// Prevents a script from executing it's main loop more than once per frame.
			while (current_frame == Movie::GetCurrentFrame());
			return 0;
		}

		int getFrameCount(lua_State *L)
        {
	        lua_pushinteger(L, Movie::GetCurrentFrame());
	        return 1;
        }

		int getLagCount(lua_State *L)
        {
	        lua_pushinteger(L, Movie::g_currentLagCount);
	        return 1;
        }

        int softReset(lua_State *L)
        {
	        ProcessorInterface::ResetButton_Tap();
	        return 0;
        }

		int setEmulatorSpeed(lua_State *L)
        {
	        float spd = (lua_tonumber(L, 1) * 0.01f);
	        SConfig::GetInstance().m_EmulationSpeed = spd;
	        return 0;
        }
	}
}