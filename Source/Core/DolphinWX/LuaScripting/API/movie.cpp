// Copyright 2020 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "DolphinWX/LUA_API.h"
#include "DolphinWX/LuaScripting.h"

#include "Core/Movie.h"

namespace Lua
{
	namespace movie
	{
		int startsFromSaveState(lua_State *L)
		{
	        lua_pushboolean(L, Movie::IsRecordingInputFromSaveState());
	        return 1;
		}

        int startsFromClearSave(lua_State *L)
        {
	        lua_pushboolean(L, Movie::IsStartingFromClearSave());
	        return 1;
        }

	    int isLoaded(lua_State *L)
		{
	        lua_pushboolean(L, Movie::IsMovieActive());
	        return 1;
		}

		int length(lua_State *L)
		{
			if (Movie::IsMovieActive())
			{
				lua_pushinteger(L, Movie::g_totalFrames);
				return 1;
			}
			else
			{
				return 0;
			}
		}

		int save(lua_State *L)
		{
	        if (lua_gettop(L) < 1)
		        return 0;

	        std::string filename = lua_tostring(L, 1);
	        Movie::SaveRecording(filename);
	        return 0;
		}

        int setReadOnly(lua_State *L)
		{
	        bool SetReadOnly = false;

	        BOOL setReadonly = lua_toboolean(L, 1);
			if (setReadonly == 1)
			{
		        SetReadOnly = true;
			}

	        Movie::SetReadOnly(SetReadOnly);
	        return 0;
		}

	    int stop(lua_State *L)
		{
	        Movie::EndPlayInput(false);
	        return 0;
		}

	}
}