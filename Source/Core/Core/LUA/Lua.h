// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

//Dragonbane

#pragma once

#include <string>
#include "Common/CommonTypes.h"
#include "DolphinWX/Main.h"

#include <lua.hpp>
#include <lua.h>
#include <luaconf.h>


typedef int                 BOOL;
int ReadValue8(lua_State *L);
int ReadValue16(lua_State *L);
int ReadValue32(lua_State *L);
int ReadValueFloat(lua_State *L);
int ReadValueString(lua_State *L);
int WriteValue8(lua_State *L);
int WriteValue16(lua_State *L);
int WriteValue32(lua_State *L);
int WriteValueFloat(lua_State *L);
int WriteValueString(lua_State *L);
int GetPointerNormal(lua_State *L);
int PressButton(lua_State *L);
int ReleaseButton(lua_State *L);
int SetMainStickX(lua_State *L);
int SetMainStickY(lua_State *L);
int SetCStickX(lua_State *L);
int SetCStickY(lua_State *L);
int SaveState(lua_State *L);
int LoadState(lua_State *L);
int GetFrameCount(lua_State *L);
int GetInputFrameCount(lua_State *L);
int SetScreenText(lua_State *L);
int PauseEmulation(lua_State *L);
int SetInfoDisplay(lua_State *L);
int MsgBox(lua_State *L);
int CancelScript(lua_State *L);
void HandleLuaErrors(lua_State *L, int status);
struct GCPadStatus;

namespace Lua
{
	struct StateEvent
	{
		bool doSave = false;
		bool useSlot = false;
		int slotID = 0;
		std::string fileName = "";
	};

	struct LuaScript
	{
		std::string fileName;
		lua_State *luaState;
		bool hasStarted;
		bool requestedTermination;
		bool wantsSavestateCallback;
	};

	//Dragonbane: LUA Savestate support
	extern bool lua_isStateOperation;
	extern bool lua_isStateDone;
	extern bool lua_isStateSaved;
	extern bool lua_isStateLoaded;
	extern StateEvent m_stateData;

	void Init();
	void Shutdown();
	void LoadScript(std::string fileName);
	void TerminateScript(std::string fileName);
	bool IsScriptRunning(std::string fileName);
	void UpdateScripts(GCPadStatus* PadStatus);
    u32 readPointer(u32 startAddress, u32 offset);
    u32 ExecuteMultilevelLoop(lua_State *L);
    void IsInMEMArea(u32 pointer, bool *b);

	void iPressButton(const char* button);
	void iReleaseButton(const char* button);
	void iSetMainStickX(int xVal);
	void iSetMainStickY(int yVal);
	void iSetCStickX(int xVal);
	void iSetCStickY(int yVal);
	void iSaveState(bool toSlot, int slotID, std::string fileName);
	void iLoadState(bool fromSlot, int slotID, std::string fileName);
	void iCancelCurrentScript();
}
