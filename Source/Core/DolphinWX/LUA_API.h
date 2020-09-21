// Copyright 2020 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <lua.hpp>
#include <lua.h>
#include <luaconf.h>

#include "Common/CommonTypes.h"


typedef int BOOL;

namespace Lua
{
	namespace bit
	{    
		int band(lua_State *L);
	}
	namespace console
	{
		int log(lua_State *L);
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
	namespace joypad
	{				
		namespace GC
		{
			int getAnalog(lua_State *L);
            int setAnalog(lua_State *L);
            int setAnalogPolar(lua_State *L);
            int getCStick(lua_State *L);
            int setCStick(lua_State *L);
            int setCStickPolar(lua_State *L);
            int getButtons(lua_State *L);
            int setButtons(lua_State *L);
            int getTriggers(lua_State *L);
            int setTriggers(lua_State *L);
		}
		namespace Wii
		{
			int getExtension(lua_State *L);
			int getButtons(lua_State *L);
            int setButtons(lua_State *L);
			// Extensions
			namespace Nunchuk
			{
				int getButtons(lua_State *L);
				int setButtons(lua_State *L);
                int getAnalog(lua_State *L);
                int setAnalog(lua_State *L);
			} // namespace Nunchuk
            namespace Classic					
			{            
				int getButtons(lua_State *L);            
				int setButtons(lua_State *L);
				int getAnalogL(lua_State *L);
				int setAnalogL(lua_State *L);
                int getAnalogR(lua_State *L);
                int setAnalogR(lua_State *L);
            } // namespace Classic
		} // namespace Wii
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