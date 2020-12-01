// Copyright 2020 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <lua.hpp>
#include <lua.h>
#include <luaconf.h>

#include "Core/LUA/Lua.h"
#include "Common/CommonTypes.h"


typedef int BOOL;

namespace Lua
{
	namespace bit
	{    
		int band(lua_State *L);
	}
	namespace emu
	{
		int frameAdvance(lua_State *L);
		int getFrameCount(lua_State *L);
        int getLagCount(lua_State *L);
		int softReset(lua_State *L);
        int setEmulatorSpeed(lua_State *L);
	}
	namespace gui
	{
		int DrawText(lua_State *L);
		int Msg(lua_State *L);
	}
	namespace memory
	{    
		int ReadByte(lua_State *L);
		int ReadShort(lua_State *L);
        int ReadInt(lua_State *L);
        int ReadFloat(lua_State *L);
        int ReadString(lua_State *L);
        int WriteByte(lua_State *L);
        int WriteShort(lua_State *L);
        int WriteInt(lua_State *L);
        int WriteFloat(lua_State *L);
        int WriteString(lua_State *L);
        int GetPointer(lua_State *L);
	} // namespace memory
	namespace movie
	{    
		int startsFromSaveState(lua_State *L);
		int startsFromClearSave(lua_State *L);
        int isLoaded(lua_State *L);
        int length(lua_State *L);
        int save(lua_State *L);
        int setReadOnly(lua_State *L);
        int stop(lua_State *L);
	} // namespace movie
	namespace savestate
	{
		int load(lua_State *L);
		int loadSlot(lua_State *L);
        int save(lua_State *L);
        int saveSlot(lua_State *L);
	} // namespace savestate
} // namespace Lua