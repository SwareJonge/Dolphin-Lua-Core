// Copyright 2020 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "DolphinWX/LUA_API.h"
#include "DolphinWX/LuaScripting.h"

namespace Lua
{
	namespace console
	{
		int log(lua_State *L)
		{	    
			LuaScriptFrame::GetCurrentInstance()->GetEventHandler()->CallAfter(&LuaScriptFrame::Log, wxString(lua_tostring(L, 1)));	    
			LuaScriptFrame::GetCurrentInstance()->GetEventHandler()->CallAfter(&LuaScriptFrame::Log, wxString("\n"));			
			return 0;
		}
	}
}