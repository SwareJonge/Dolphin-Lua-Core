// Copyright 2020 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "Common/StringUtil.h"
#include "Core/LUA/Lua.h"
#include "Core/Movie.h"
#include "Core/PowerPC/PowerPC.h"
#include "DolphinWX/LUA_API.h"

namespace Lua
{
	namespace memory
	{
    int ReadByte(lua_State *L)
    {
	    int argc = lua_gettop(L);

	    if (argc < 1)
	    {
		    return 0;
	    }
	    u8 result = 0;
	    // if there's one argument read address from the first argument
	    if (argc < 2)
	    {
		    u32 address = lua_tointeger(L, 1);

		    result = PowerPC::Read_U8(address);

		    lua_pushinteger(L, result); // return value
		    return 1;                   // number of return values
	    }
	    // if more than 1 argument, read multilelve pointer

	    if (Lua::ExecuteMultilevelLoop(L) != 0)
	    {
		    result = PowerPC::Read_U8(Lua::ExecuteMultilevelLoop(L));
	    }
	    lua_pushinteger(L, result);

	    return 1; // number of return
    }

    int ReadShort(lua_State *L)
    {
	    int argc = lua_gettop(L);

	    if (argc < 1)
		    return 0;

	    u16 result = 0;
	    // if there's one argument read address from the first argument
	    if (argc < 2)
	    {
		    u32 address = lua_tointeger(L, 1);

		    result = PowerPC::Read_U16(address);

		    lua_pushinteger(L, result); // return value
		    return 1;
	    }
	    // if more than 1 argument, read multilelve pointer
	    if (Lua::ExecuteMultilevelLoop(L) != 0)
	    {
		    result = PowerPC::Read_U16(Lua::ExecuteMultilevelLoop(L));
	    }
	    lua_pushinteger(L, result);

	    return 1; // number of return values
    }

    int ReadInt(lua_State *L)
    {
	    int argc = lua_gettop(L);

	    if (argc < 1)
		    return 0;

	    u32 result = 0;
	    // if there's one argument read address from the first argument
	    if (argc < 2)
	    {
		    u32 address = lua_tointeger(L, 1);

		    result = PowerPC::Read_U32(address);

		    lua_pushinteger(L, result); // return value
		    return 1;
	    }
	    // if more than 1 argument, read multilelve pointer
	    if (Lua::ExecuteMultilevelLoop(L) != 0)
	    {
		    result = PowerPC::Read_U32(Lua::ExecuteMultilevelLoop(L));
	    }
	    lua_pushinteger(L, result); // return value
	    return 1;                   // number of return values
    }

    int ReadFloat(lua_State *L)
    {
	    int argc = lua_gettop(L);

	    if (argc < 1)
		    return 0;

	    float result = 0;
	    // if there's one argument read address from the first argument
	    if (argc < 2)
	    {
		    u32 address = lua_tointeger(L, 1);

		    result = PowerPC::Read_F32(address);

		    lua_pushnumber(L, result); // return value
		    return 1;
	    }
	    // if more than 1 argument, read multilelve pointer
	    if (Lua::ExecuteMultilevelLoop(L) != 0)
	    {
		    result = PowerPC::Read_F32(Lua::ExecuteMultilevelLoop(L));
	    }

	    lua_pushnumber(L, result); // return value
	    return 1;                  // number of return values
    }
    int ReadString(lua_State *L)
    {
	    int argc = lua_gettop(L);

	    if (argc < 2)
		    return 0;

	    u32 address = lua_tointeger(L, 1);
	    int count = lua_tointeger(L, 2);

	    std::string result = PowerPC::Read_String(address, count);

	    lua_pushstring(L, result.c_str()); // return value
	    return 1;                          // number of return values
    }

    // Write Stuff
    int WriteByte(lua_State *L)
    {
	    if (Movie::IsPlayingInput())
		    return 0;

	    int argc = lua_gettop(L);

	    if (argc < 2)
		    return 0;

	    u32 address = lua_tointeger(L, 1);
	    u8 value = lua_tointeger(L, 2);

	    PowerPC::Write_U8(value, address);

	    return 0; // number of return values
    }

    int WriteShort(lua_State *L)
    {
	    if (Movie::IsPlayingInput())
		    return 0;

	    int argc = lua_gettop(L);

	    if (argc < 2)
		    return 0;

	    u32 address = lua_tointeger(L, 1);
	    u16 value = lua_tointeger(L, 2);

	    PowerPC::Write_U16(value, address);

	    return 0; // number of return values
    }

    int WriteInt(lua_State *L)
    {
	    if (Movie::IsPlayingInput())
		    return 0;

	    int argc = lua_gettop(L);

	    if (argc < 2)
		    return 0;

	    u32 address = lua_tointeger(L, 1);
	    u32 value = lua_tointeger(L, 2);

	    PowerPC::Write_U32(value, address);

	    return 0; // number of return values
    }

    int WriteFloat(lua_State *L)
    {
	    if (Movie::IsPlayingInput())
		    return 0;

	    int argc = lua_gettop(L);

	    if (argc < 2)
		    return 0;

	    u32 address = lua_tointeger(L, 1);
	    double value = lua_tonumber(L, 2);

	    PowerPC::Write_F32((float)value, address);

	    return 0; // number of return values
    }
    int WriteString(lua_State *L)
    {
	    if (Movie::IsPlayingInput())
		    return 0;

	    int argc = lua_gettop(L);

	    if (argc < 2)
		    return 0;

	    u32 address = lua_tointeger(L, 1);
	    const char *value = lua_tostring(L, 2);

	    std::string string = StringFromFormat("%s", value);

	    PowerPC::Write_String(string, address);

	    return 0; // number of return values
    }

    int GetPointer(lua_State *L)
    {
	    int argc = lua_gettop(L);
	    // if there are no arguments, don't execute further
	    if (argc < 1)
		    return 0; // don't pass any return values
	    // if there is 1 argument, use the old method
	    if (argc < 2)
	    {
		    // Read The address from the first argument
		    u32 address = lua_tointeger(L, 1);
		    // Since we don't need to read any offsets we can just do this
		    u32 pointer = Lua::readPointer(address, 0x0);
		    // return so the function doesn't execute further
		    lua_pushinteger(L, pointer); // return value
		    return 1;
	    }
	    // new method, supports multilevel pointers
	    // we need to read the main pointer once, so we can use this one in the for loop
	    u32 pointer = Lua::ExecuteMultilevelLoop(L);
	    lua_pushinteger(L, pointer); // return value
	    return 1;                    // number of return values
    }
	}
} // namespace Lua
