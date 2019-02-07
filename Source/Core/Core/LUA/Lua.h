// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

//Dragonbane

#pragma once

#include <string>
#include "Common/CommonTypes.h"
#include <DolphinWX/MAIN.H>

#include <lua.hpp>

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