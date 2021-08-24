// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

// Contributions by luckytyphlosion are
// licensed under GPLv2+

#include <mbedtls/md5.h>
#include <lua.hpp>
#include <lua.h>
#include <luaconf.h>

#include "Common/ChunkFile.h"
#include "Common/CommonPaths.h"
#include "Common/Hash.h"
#include "Common/NandPaths.h"
#include "Common/Thread.h"
#include "Common/Timer.h"
#include "Core/ConfigManager.h"
#include "Core/Core.h"
#include "Core/CoreTiming.h"
#include "Common/FileSearch.h"
#include "Common/FileUtil.h"
#include "Common/StringUtil.h"
#include "Core/Movie.h"
#include "Core/LUA/Lua.h"
#include "Core/NetPlayProto.h"
#include "Core/State.h"
#include "Core/DSP/DSPCore.h"
#include "Core/HW/DVDInterface.h"
#include "Core/HW/EXI_Device.h"
#include "Core/HW/ProcessorInterface.h"
#include "Core/HW/SI.h"
#include "Core/HW/Wiimote.h"
#include "Core/HW/WiimoteEmu/WiimoteEmu.h"
#include "Core/HW/WiimoteEmu/WiimoteHid.h"
#include "Core/HW/WiimoteEmu/Attachment/Classic.h"
#include "Core/HW/WiimoteEmu/Attachment/Nunchuk.h"
#include "Core/IPC_HLE/WII_IPC_HLE_Device_usb.h"
#include "Core/PowerPC/PowerPC.h"
#include "InputCommon/GCPadStatus.h"
#include "VideoCommon/Statistics.h"
#include "VideoCommon/VideoConfig.h"
#include "Core/Host.h"

//#include "DolphinWX/Main.h"
//#include "DolphinWX/Frame.h"

//Lua Functions (C)
int ReadValue8(lua_State* L)
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

		result = Memory::Read_U8(address);

		lua_pushinteger(L, result); // return value
		return 1;                   // number of return values
	}
	// if more than 1 argument, read multilelve pointer
	
	if (Lua::ExecuteMultilevelLoop(L) != 0)
	{
		result =Memory::Read_U8(Lua::ExecuteMultilevelLoop(L));
	}

	lua_pushinteger(L, result);
	return 1; // number of return values
}

int ReadValue16(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc < 1)
		return 0;

	u16 result = 0;
	// if there's one argument read address from the first argument
	if (argc < 2)
	{
		u32 address = lua_tointeger(L, 1);

		result = Memory::Read_U16(address);

		lua_pushinteger(L, result); // return value
		return 1;
	}
	// if more than 1 argument, read multilelve pointer
	if (Lua::ExecuteMultilevelLoop(L) != 0)
	{
		result = Memory::Read_U16(Lua::ExecuteMultilevelLoop(L));
	}

	lua_pushinteger(L, result);
	return 1; // number of return values
}

int ReadValue32(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc < 1)
		return 0;

	u32 result = 0;
	// if there's one argument read address from the first argument
	if (argc < 2)
	{
		u32 address = lua_tointeger(L, 1);

		result = Memory::Read_U32(address);

		lua_pushinteger(L, result); // return value
		return 1;
	}
	// if more than 1 argument, read multilelve pointer
	if (Lua::ExecuteMultilevelLoop(L) != 0)
	{
		result = Memory::Read_U32(Lua::ExecuteMultilevelLoop(L));
		// result = Memory::Read_U8(LastOffset);
	}

	lua_pushinteger(L, result); // return value
	return 1; // number of return values
}

int ReadValueFloat(lua_State* L)
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
	return 1;                   // number of return values
}

int ReadValueString(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc < 2)
		return 0;
	// can't do the multilevel loop properly unless i'm not lazy
	u32 address = lua_tointeger(L, 1);
	int count = lua_tointeger(L, 2);

	std::string result = PowerPC::Read_String(address, count);

	lua_pushstring(L, result.c_str()); // return value
	return 1; // number of return values
}

//Write Stuff
int WriteValue8(lua_State* L)
{
	if (Movie::IsPlayingInput())
		return 0;
	
	int argc = lua_gettop(L);

	if (argc < 2)
		return 0;

	u32 address = lua_tointeger(L, 1);
	u8 value = lua_tointeger(L, 2);

	Memory::Write_U8(value, address);

	return 0; // number of return values
}

int WriteValue16(lua_State* L)
{
	if (Movie::IsPlayingInput())
		return 0;
	
	int argc = lua_gettop(L);

	if (argc < 2)
		return 0;

	u32 address = lua_tointeger(L, 1);
	u16 value = lua_tointeger(L, 2);

	Memory::Write_U16(value, address);

	return 0; // number of return values
}

int WriteValue32(lua_State* L)
{
	if (Movie::IsPlayingInput())
		return 0;
	
	int argc = lua_gettop(L);

	if (argc < 2)
		return 0;

	u32 address = lua_tointeger(L, 1);
	u32 value = lua_tointeger(L, 2);

	Memory::Write_U32(value, address);

	return 0; // number of return values
}

int WriteValueFloat(lua_State* L)
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

int WriteValueString(lua_State* L)
{
	if (Movie::IsPlayingInput())
		return 0;
	
	int argc = lua_gettop(L);

	if (argc < 2)
		return 0;

	u32 address = lua_tointeger(L, 1);
	const char* value = lua_tostring(L, 2);

	std::string string = StringFromFormat("%s", value);

	PowerPC::Write_String(string, address);

	return 0; // number of return values
}

int GetPointerNormal(lua_State* L)
{
	int argc = lua_gettop(L);
	// if there are no arguments, don't execute further
	if (argc < 1)
		return 0; // don't pass any return values
	// if there is 1 argument, use the old method
	if (argc < 2)
	{
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
	return 1; // number of return values
}

int GetGameID(lua_State* L)
{
	lua_pushstring(L, SConfig::GetInstance().GetUniqueID().c_str());
	return 1;
}

int GetScriptsDir(lua_State* L)
{
	lua_pushstring(L, File::GetUserPath(D_SCRIPTS_IDX));
	return 1;
}

int PressButton(lua_State* L)
{
	if (Movie::IsPlayingInput())
		return 0;
	
	int argc = lua_gettop(L);

	if (argc < 1)
		return 0;

	const char* button = lua_tostring(L, 1);

	Lua::iPressButton(button);

	return 0; // number of return values
}

int ReleaseButton(lua_State* L)
{
	if (Movie::IsPlayingInput())
		return 0;
	
	int argc = lua_gettop(L);

	if (argc < 1)
		return 0;

	const char* button = lua_tostring(L, 1);

	Lua::iReleaseButton(button);

	return 0; // number of return values
}

int SetMainStickX(lua_State* L)
{
	if (Movie::IsPlayingInput())
		return 0;
	
	int argc = lua_gettop(L);

	if (argc < 1)
		return 0;

	int xPos = lua_tointeger(L, 1);

	Lua::iSetMainStickX(xPos);

	return 0;
}
int SetMainStickY(lua_State* L)
{
	if (Movie::IsPlayingInput())
		return 0;
	
	int argc = lua_gettop(L);

	if (argc < 1)
		return 0;

	int yPos = lua_tointeger(L, 1);

	Lua::iSetMainStickY(yPos);

	return 0;
}

int SetCStickX(lua_State* L)
{
	if (Movie::IsPlayingInput())
		return 0;
	
	int argc = lua_gettop(L);

	if (argc < 1)
		return 0;

	int xPos = lua_tointeger(L, 1);

	Lua::iSetCStickX(xPos);

	return 0;
}
int SetCStickY(lua_State* L)
{
	if (Movie::IsPlayingInput())
		return 0;
	
	int argc = lua_gettop(L);

	if (argc < 1)
		return 0;

	int yPos = lua_tointeger(L, 1);

	Lua::iSetCStickY(yPos);

	return 0;
}

int SaveState(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc < 2)
		return 0;

	bool useSlot = false;

	BOOL Slot = lua_toboolean(L, 1);
	int slotID = 0;
	std::string string = "";

	if (Slot)
	{
		useSlot = true;
		slotID = lua_tointeger(L, 2);
	}
	else
	{
		const char* fileName = lua_tostring(L, 2);
		string = StringFromFormat("%s", fileName);
	}

	Lua::iSaveState(useSlot, slotID, string);

	return 0; // number of return values
}

int LoadState(lua_State* L)
{
	if (Movie::IsPlayingInput())
		return 0;
	
	int argc = lua_gettop(L);

	if (argc < 2)
		return 0;

	bool useSlot = false;

	BOOL Slot = lua_toboolean(L, 1);
	int slotID = 0;
	std::string string = "";

	if (Slot)
	{
		useSlot = true;
		slotID = lua_tointeger(L, 2);
	}
	else
	{
		const char* fileName = lua_tostring(L, 2);
		string = StringFromFormat("%s", fileName);
	}

	Lua::iLoadState(useSlot, slotID, string);

	return 0; // number of return values
}

int GetFrameCount(lua_State* L)
{
	int argc = lua_gettop(L);

	lua_pushinteger(L, Movie::g_currentFrame); // return value
	return 1; // number of return values
}

int GetInputFrameCount(lua_State* L)
{
	int argc = lua_gettop(L);

	lua_pushinteger(L, Movie::g_currentInputCount + 1); // return value
	return 1; // number of return values
}

int SetScreenText(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc < 1)
		return 0;

	const char *text = lua_tostring(L, 1);

	std::string screen_text = StringFromFormat("%s", text);
	screen_text.append("\n");

	Statistics::SetString(screen_text);

	return 0;
}

int PauseEmulation(lua_State* L)
{
	int argc = lua_gettop(L);

	Core::SetState(Core::CORE_PAUSE);

	return 0;
}

int SetInfoDisplay(lua_State* L)
{
	int argc = lua_gettop(L);	
	SConfig::GetInstance().m_ShowRAMDisplay = !SConfig::GetInstance().m_ShowRAMDisplay;	
	SConfig::GetInstance().SaveSettings();
	return 0;
}

int SetFrameAndAudioDump(lua_State* L)
{
	int argc = lua_gettop(L);

	bool enableDump = (lua_toboolean(L, 1) != 0);
	SConfig::GetInstance().m_DumpFrames = enableDump;
	SConfig::GetInstance().m_DumpAudio = enableDump;
	//main_frame->GetMenuBar()->FindItem(IDM_TOGGLE_DUMP_FRAMES)->Check(enableDump);
	//main_frame->GetMenuBar()->FindItem(IDM_TOGGLE_DUMP_AUDIO)->Check(enableDump);    
	SConfig::GetInstance().SaveSettings();

	return 0;
}

int MsgBox(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc < 1)
		return 0;

	const char* text = lua_tostring(L, 1);

	int delay = 5000; //Default: 5 seconds

	if (argc == 2)
	{
		delay = lua_tointeger(L, 2);
	}

	std::string message = StringFromFormat("Lua Msg: %s", text);

	Core::DisplayMessage(message, delay);

	return 0; // number of return values
}

int CancelScript(lua_State* L)
{
	int argc = lua_gettop(L);

	Lua::iCancelCurrentScript();

	return 0; // number of return values
}

void HandleLuaErrors(lua_State* L, int status)
{
	if (status != 0)
	{
		std::string message = StringFromFormat("Lua Error: %s", lua_tostring(L, -1));

		PanicAlertT(message.c_str());

		lua_pop(L, 1); // remove error message
	}
}

namespace Lua
{
	//Dragonbane: Lua Stuff
	static std::list<LuaScript> scriptList;
	static int currScriptID;

	static GCPadStatus PadLocal;

	const int m_gc_pad_buttons_bitmask[12] = {
		PAD_BUTTON_DOWN, PAD_BUTTON_UP, PAD_BUTTON_LEFT, PAD_BUTTON_RIGHT, PAD_BUTTON_A, PAD_BUTTON_B,
		PAD_BUTTON_X, PAD_BUTTON_Y, PAD_TRIGGER_Z, PAD_TRIGGER_L, PAD_TRIGGER_R, PAD_BUTTON_START
	};

	//LUA Savestate Stuff
	StateEvent m_stateData;

	bool lua_isStateOperation = false;
	bool lua_isStateSaved = false;
	bool lua_isStateLoaded = false;
	bool lua_isStateDone = false;


	//Dragonbane: Lua Wrapper Functions
	void iPressButton(const char* button)
	{
		if (!strcmp(button, "A"))
		{
			PadLocal.button |= m_gc_pad_buttons_bitmask[4];
			PadLocal.analogA = 0xFF;
		}
		else if (!strcmp(button, "B"))
		{
			PadLocal.button |= m_gc_pad_buttons_bitmask[5];
			PadLocal.analogB = 0xFF;
		}
		else if (!strcmp(button, "X"))
		{
			PadLocal.button |= m_gc_pad_buttons_bitmask[6];
		}
		else if (!strcmp(button, "Y"))
		{
			PadLocal.button |= m_gc_pad_buttons_bitmask[7];
		}
		else if (!strcmp(button, "Z"))
		{
			PadLocal.button |= m_gc_pad_buttons_bitmask[8];
		}
		else if (!strcmp(button, "L"))
		{
			PadLocal.triggerLeft = 255;
			PadLocal.button |= m_gc_pad_buttons_bitmask[9];
		}
		else if (!strcmp(button, "R"))
		{
			PadLocal.triggerRight = 255;
			PadLocal.button |= m_gc_pad_buttons_bitmask[10];
		}
		else if (!strcmp(button, "Start"))
		{
			PadLocal.button |= m_gc_pad_buttons_bitmask[11];
		}
		else if (!strcmp(button, "D-Up"))
		{
			PadLocal.button |= m_gc_pad_buttons_bitmask[1];
		}
		else if (!strcmp(button, "D-Down"))
		{
			PadLocal.button |= m_gc_pad_buttons_bitmask[0];
		}
		else if (!strcmp(button, "D-Left"))
		{
			PadLocal.button |= m_gc_pad_buttons_bitmask[2];
		}
		else if (!strcmp(button, "D-Right"))
		{
			PadLocal.button |= m_gc_pad_buttons_bitmask[3];
		}		
	}
	void iReleaseButton(const char* button)
	{
		if (!strcmp(button, "A"))
		{
			PadLocal.button &= ~m_gc_pad_buttons_bitmask[4];
			PadLocal.analogA = 0x00;
		}
		else if (!strcmp(button, "B"))
		{
			PadLocal.button &= ~m_gc_pad_buttons_bitmask[5];
			PadLocal.analogB = 0x00;
		}
		else if (!strcmp(button, "X"))
		{
			PadLocal.button &= ~m_gc_pad_buttons_bitmask[6];
		}
		else if (!strcmp(button, "Y"))
		{
			PadLocal.button &= ~m_gc_pad_buttons_bitmask[7];
		}
		else if (!strcmp(button, "Z"))
		{
			PadLocal.button &= ~m_gc_pad_buttons_bitmask[8];
		}
		else if (!strcmp(button, "L"))
		{
			PadLocal.triggerLeft = 0;
			PadLocal.button &= ~m_gc_pad_buttons_bitmask[9];
		}
		else if (!strcmp(button, "R"))
		{
			PadLocal.triggerRight = 0;
			PadLocal.button &= ~m_gc_pad_buttons_bitmask[10];
		}
		else if (!strcmp(button, "Start"))
		{
			PadLocal.button &= ~m_gc_pad_buttons_bitmask[11];
		}
		else if (!strcmp(button, "D-Up"))
		{
			PadLocal.button &= ~m_gc_pad_buttons_bitmask[1];
		}
		else if (!strcmp(button, "D-Down"))
		{
			PadLocal.button &= ~m_gc_pad_buttons_bitmask[0];
		}
		else if (!strcmp(button, "D-Left"))
		{
			PadLocal.button &= ~m_gc_pad_buttons_bitmask[2];
		}
		else if (!strcmp(button, "D-Right"))
		{
			PadLocal.button &= ~m_gc_pad_buttons_bitmask[3];
		}
	}

	u32 readPointer(u32 startAddress, u32 offset)
	{
	    u32 pointer = Memory::Read_U32(startAddress) + offset;
	    // check if pointer is not in the mem1 or mem2
	    if (Lua::IsInMEMArea(pointer))
	    {
			if ((pointer > 0x80000000 && pointer < 0x81800000) || (pointer > 0x90000000 && pointer < 0x94000000))
			{
				pointer -= 0x80000000;
			}

			return pointer;
	    }
		else return 0;
	}

	u32 normalizePointer(u32 pointer)
	{
		if ((pointer > 0x80000000 && pointer < 0x81800000) || (pointer > 0x90000000 && pointer < 0x94000000))
		{
			pointer -= 0x80000000;
		}
		return pointer;
	}

	u32 ExecuteMultilevelLoop(lua_State *L)
    {
	    int argc = lua_gettop(L);
	    u32 mainPointer = lua_tointeger(L, 1);
	    // we need to read the main pointer once, so we can use this one in the for loop
	    u32 pointer = mainPointer;

	    for (int i = 2; i <= argc; ++i)
	    {
		    // read offsets
		    u32 offset = lua_tointeger(L, i);
		    // dedicated function to read the offsets and pointer
		    pointer = readPointer(pointer, offset);
		    if ((pointer == 0) || (pointer == offset))
		    {
			    pointer = 0;
			    break;			    
		    }
	    }
		return pointer;	    
    }

	void iSetMainStickX(int xVal)
	{
		PadLocal.stickX = xVal;
	}
	void iSetMainStickY(int yVal)
	{
		PadLocal.stickY = yVal;
	}
	void iSetCStickX(int xVal)
	{
		PadLocal.substickX = xVal;
	}
	void iSetCStickY(int yVal)
	{
		PadLocal.substickY = yVal;
	}
	void iSaveState(bool toSlot, int slotID, std::string fileName)
	{
		m_stateData.doSave = true;
		m_stateData.useSlot = toSlot;
		m_stateData.slotID = slotID;
		m_stateData.fileName = fileName;	

		lua_isStateSaved = false;
		lua_isStateLoaded = false;
		lua_isStateDone = false;
		lua_isStateOperation = true;

		if (currScriptID != -1)
		{
			int n = 0;

			for (std::list<LuaScript>::iterator it = scriptList.begin(); it != scriptList.end(); ++it)
			{
				if (currScriptID == n)
				{
					it->wantsSavestateCallback = true;
					break;
				}

				++n;
			}
		}

		Host_UpdateMainFrame();
	}
	void iLoadState(bool fromSlot, int slotID, std::string fileName)
	{
		m_stateData.doSave = false;
		m_stateData.useSlot = fromSlot;
		m_stateData.slotID = slotID;
		m_stateData.fileName = fileName;

		lua_isStateSaved = false;
		lua_isStateLoaded = false;
		lua_isStateDone = false;
		lua_isStateOperation = true;

		if (currScriptID != -1)
		{
			int n = 0;

			for (std::list<LuaScript>::iterator it = scriptList.begin(); it != scriptList.end(); ++it)
			{
				if (currScriptID == n)
				{
					it->wantsSavestateCallback = true;
					break;
				}

				++n;
			}
		}

		Host_UpdateMainFrame();
	}
	void iCancelCurrentScript()
	{
		int n = 0;

		for (std::list<LuaScript>::iterator it = scriptList.begin(); it != scriptList.end(); ++it)
		{
			if (currScriptID == n)
			{
				it->requestedTermination = true;
				break;
			}

			++n;
		}
	}


	//Main Functions
	static void RegisterGeneralLuaFunctions(lua_State* luaState)
	{
		//Make C functions available to Lua programs
		lua_register(luaState, "ReadValue8", ReadValue8);
		lua_register(luaState, "ReadValue16", ReadValue16);
		lua_register(luaState, "ReadValue32", ReadValue32);
		lua_register(luaState, "ReadValueFloat", ReadValueFloat);
		lua_register(luaState, "ReadValueString", ReadValueString);
		lua_register(luaState, "GetPointerNormal", GetPointerNormal);

		lua_register(luaState, "WriteValue8", WriteValue8);
		lua_register(luaState, "WriteValue16", WriteValue16);
		lua_register(luaState, "WriteValue32", WriteValue32);
		lua_register(luaState, "WriteValueFloat", WriteValueFloat);
		lua_register(luaState, "WriteValueString", WriteValueString);

		lua_register(luaState, "GetGameID", GetGameID);
		lua_register(luaState, "GetScriptsDir", GetScriptsDir);

		lua_register(luaState, "PressButton", PressButton);
		lua_register(luaState, "ReleaseButton", ReleaseButton);
		lua_register(luaState, "SetMainStickX", SetMainStickX);
		lua_register(luaState, "SetMainStickY", SetMainStickY);
		lua_register(luaState, "SetCStickX", SetCStickX);
		lua_register(luaState, "SetCStickY", SetCStickY);

		lua_register(luaState, "SaveState", SaveState);
		lua_register(luaState, "LoadState", LoadState);

		lua_register(luaState, "GetFrameCount", GetFrameCount);
		lua_register(luaState, "GetInputFrameCount", GetInputFrameCount);
		lua_register(luaState, "MsgBox", MsgBox);
		
		lua_register(luaState, "SetScreenText", SetScreenText);
		lua_register(luaState, "PauseEmulation", PauseEmulation);
		lua_register(luaState, "SetInfoDisplay", SetInfoDisplay);

		// added by luckytyphlosion
		lua_register(luaState, "SetFrameAndAudioDump", SetFrameAndAudioDump);
	}

	void Init()
	{
		//For Pad manipulation
		memset(&PadLocal, 0, sizeof(PadLocal));

		//Auto launch Scripts that start with _

	    std::vector<std::string> rFilenames = DoFileSearch({".lua"}, {File::GetUserPath(D_SCRIPTS_IDX)});

		if (rFilenames.size() > 0)
		{
			for (u32 i = 0; i < rFilenames.size(); i++)
			{
				std::string FileName;
				SplitPath(rFilenames[i], nullptr, &FileName, nullptr);

				if (!FileName.substr(0, 1).compare("_"))
				{
					LoadScript(FileName + ".lua");
				}
			}
		}
	}

	void Shutdown()
	{
		// Kill all Scripts
		for (std::list<LuaScript>::iterator it = scriptList.begin(); it != scriptList.end(); ++it)
		{
			if (it->hasStarted)
			{
				lua_close(it->luaState);
			}
		}

		scriptList.clear();
	}

	bool IsInMEMArea(u32 pointer)
	{
	    if ((pointer > 0x80000000 && pointer < 0x81800000) || (pointer > 0x90000000 && pointer < 0x94000000))
	    {
			return true;
	    }
	    else if ((pointer > 0x0 && pointer < 0x1800000) || (pointer > 0x10000000 && pointer < 0x14000000))
	    {
		    
			return true;
	    }
		else
		{		   
		    return false;
		}
		    
	}

	void LoadScript(std::string fileName)
	{
		LuaScript newScript;

		newScript.wantsSavestateCallback = false;
		newScript.requestedTermination = false;
		newScript.hasStarted = false;
		newScript.fileName = fileName;

		scriptList.push_back(newScript);
	}

	void TerminateScript(std::string fileName)
	{
		for (std::list<LuaScript>::iterator it = scriptList.begin(); it != scriptList.end(); ++it) //could this crash when an entry is deleted by the CPU thread during this?
		{
			if (it->fileName == fileName)
			{
				it->requestedTermination = true;
				break;
			}
		}
	}

	bool IsScriptRunning(std::string fileName)
	{
		for (std::list<LuaScript>::iterator it = scriptList.begin(); it != scriptList.end(); ++it)
		{
			if (it->fileName == fileName)
			{
				return true;
			}
		}

		return false;
	}


	//Called every input frame (60 times per second in TP)
	void UpdateScripts(GCPadStatus* PadStatus)
	{
		if (!Core::IsRunningAndStarted())
			return;

		//Update Local Pad
		PadLocal = *PadStatus;

		//Iterate through all the loaded LUA Scripts
		int n = 0;
		std::list<LuaScript>::iterator it = scriptList.begin();

		while (it != scriptList.end())
		{ 
			int status = 0;
			currScriptID = n; //Update Script ID for Wrapper Functions

			if (it->hasStarted == false) //Start Script
			{
				//Create new LUA State
				it->luaState = luaL_newstate();

				//Open standard LUA libraries
				luaL_openlibs(it->luaState);

				//Register C Functions
				RegisterGeneralLuaFunctions(it->luaState);

				//Unique to normal Scripts
				lua_register(it->luaState, "CancelScript", CancelScript);

				std::string file = File::GetUserPath(D_SCRIPTS_IDX) + it->fileName;

				status = luaL_dofile(it->luaState, file.c_str());

				if (status == 0)
				{
					//Execute Start function
					lua_getglobal(it->luaState, "onScriptStart");

					status = lua_pcall(it->luaState, 0, LUA_MULTRET, 0);
				}

				if (status != 0)
				{
					HandleLuaErrors(it->luaState, status);
					lua_close(it->luaState);

					it = scriptList.erase(it);
					--n;
				}
				else
				{
					it->hasStarted = true;
				}
			}
			else if (it->requestedTermination) //Cancel Script and delete the entry from the list
			{
				lua_getglobal(it->luaState, "onScriptCancel");

				status = lua_pcall(it->luaState, 0, LUA_MULTRET, 0);

				if (status != 0)
				{
					HandleLuaErrors(it->luaState, status);
				}

				lua_close(it->luaState);

				status = -1;
				it = scriptList.erase(it);
				--n;
			}
			else //Update Script
			{
				//LUA Callbacks first (so Update can already react to it)
				if (it->wantsSavestateCallback && lua_isStateOperation)
				{
					if (lua_isStateSaved)
					{
						//Saved State Callback
						it->wantsSavestateCallback = false;

						lua_getglobal(it->luaState, "onStateSaved");

						status = lua_pcall(it->luaState, 0, LUA_MULTRET, 0);

						if (status != 0)
						{
							HandleLuaErrors(it->luaState, status);
							lua_close(it->luaState);

							it = scriptList.erase(it);
							--n;
						}

						lua_isStateOperation = false;
						lua_isStateSaved = false;
						lua_isStateLoaded = false;
					}
					else if (lua_isStateLoaded)
					{
						//Loaded State Callback
						it->wantsSavestateCallback = false;

						lua_getglobal(it->luaState, "onStateLoaded");

						status = lua_pcall(it->luaState, 0, LUA_MULTRET, 0);

						if (status != 0)
						{
							HandleLuaErrors(it->luaState, status);
							lua_close(it->luaState);

							it = scriptList.erase(it);
							--n;
						}

						lua_isStateOperation = false;
						lua_isStateSaved = false;
						lua_isStateLoaded = false;
					}
				}

				//Call normal Update function
				if (status == 0)
				{
					lua_getglobal(it->luaState, "onScriptUpdate");

					status = lua_pcall(it->luaState, 0, LUA_MULTRET, 0);

					if (status != 0)
					{
						HandleLuaErrors(it->luaState, status);
						lua_close(it->luaState);

						it = scriptList.erase(it);
						--n;
					}
				}
			}

			if (status == 0) //Next item in the list if no deletion took place
				++it;

			++n;
		}

		//Send changed Pad back
		*PadStatus = PadLocal;
	}

}
