// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include <lua.hpp>

#include "Core/Core.h"
#include "DolphinWX/LuaScripting.h"
#include "Core/Movie.h"
#include "DolphinWX/Frame.h"

#include "Core/HW/Wiimote.h"
#include "Core/HW/WiimoteEmu/Attachment/Classic.h"
#include "Core/HW/WiimoteEmu/Attachment/Nunchuk.h"
#include "Core/HW/WiimoteEmu/Encryption.h"
#include "Core/HW/WiimoteEmu/WiimoteEmu.h"
#include "Core/HW/WiimoteReal/WiimoteReal.h"
#include "InputCommon/GCPadStatus.h"
#include "InputCommon/InputConfig.h"

namespace Lua
{

LuaThread::LuaThread(LuaScriptFrame* p, const wxString& file)
    : m_parent(p), m_file_path(file), wxThread()
{
  // Zero out controllers
	for (int i = 0; i < 4; i++)
	{
		m_pad_status[i].button = 0;
		m_pad_status[i].stickX = GCPadStatus::MAIN_STICK_CENTER_X;
		m_pad_status[i].stickY = GCPadStatus::MAIN_STICK_CENTER_Y;
		m_pad_status[i].triggerLeft = 0;
		m_pad_status[i].triggerRight = 0;
		m_pad_status[i].substickX = GCPadStatus::C_STICK_CENTER_X;
		m_pad_status[i].substickY = GCPadStatus::C_STICK_CENTER_Y;
		m_padWii_status[i].hex = 0;
		nunchuk[i].bt.hex = 0;
		nunchuk[i].jx = WiimoteEmu::Nunchuk::STICK_CENTER;
		nunchuk[i].jy = WiimoteEmu::Nunchuk::STICK_CENTER;
	}


  /*m_padWii_status.a = 0;
  m_padWii_status.b = 0;
  m_padWii_status.one = 0;
  m_padWii_status.two = 0;
  m_padWii_status.minus = 0;
  m_padWii_status.plus = 0;
  m_padWii_status.home = 0;
  m_padWii_status.unknown = 0;
  m_padWii_status.acc_x_lsb = 128;
  m_padWii_status.acc_y_lsb = 128;
  m_padWii_status.acc_z_lsb = 128;
  m_padWii_status.up = 0;
  m_padWii_status.down = 0;
  m_padWii_status.left = 0;
  m_padWii_status.right = 0;*/


  // Register GetValues()
  Movie::SetGCInputManip([this](GCPadStatus* status, int number)
  {
    GetValues(status, number);
  }, Movie::GCManipIndex::LuaGCManip);

  Movie::SetWiiInputManip([this](u8* data, WiimoteEmu::ReportFeatures rptf, int controllerID, int ext, const wiimote_key key)
  {
    GetWiiValues(data, rptf, controllerID, ext, key);
  }, Movie::WiiManipIndex::LuaWiiManip);

}

LuaThread::~LuaThread()
{
	// Nullify GC manipulator function to prevent crash when lua console is closed
	Movie::SetGCInputManip(nullptr, Movie::GCManipIndex::LuaGCManip);
	Movie::SetWiiInputManip(nullptr, Movie::WiiManipIndex::LuaWiiManip);
	m_parent->NullifyLuaThread();
}

wxThread::ExitCode LuaThread::Entry()
{
  std::unique_ptr<lua_State, decltype(&lua_close)> state(luaL_newstate(), lua_close);

  // Register
  lua_sethook(state.get(), &HookFunction, LUA_MASKLINE, 0);

  // Make standard libraries available to loaded script
  luaL_openlibs(state.get());

  //Make custom libraries available to loaded script
  luaopen_libs(state.get());

  if (luaL_loadfile(state.get(), m_file_path) != LUA_OK)
  {
    m_parent->Log("Error opening file.\n");

    return reinterpret_cast<wxThread::ExitCode>(-1);
  }

  // Pause emu
  Core::SetState(Core::CORE_PAUSE);

  if (lua_pcall(state.get(), 0, LUA_MULTRET, 0) != LUA_OK)
  {
    m_parent->Log(lua_tostring(state.get(), 1));

    return reinterpret_cast<wxThread::ExitCode>(-1);
  }
  Exit();
  return reinterpret_cast<wxThread::ExitCode>(0);
}

void LuaThread::GetValues(GCPadStatus *PadStatus, int number)
{
  
  if (LuaThread::m_pad_status[number].stickX != GCPadStatus::MAIN_STICK_CENTER_X)
		PadStatus->stickX = LuaThread::m_pad_status[number].stickX;

  if (LuaThread::m_pad_status[number].stickY != GCPadStatus::MAIN_STICK_CENTER_Y)
	  PadStatus->stickY = LuaThread::m_pad_status[number].stickY;

  if (LuaThread::m_pad_status[number].triggerLeft != 0)
	  PadStatus->triggerLeft = LuaThread::m_pad_status[number].triggerLeft;

  if (LuaThread::m_pad_status[number].triggerRight != 0)
	  PadStatus->triggerRight = LuaThread::m_pad_status[number].triggerRight;

  if (LuaThread::m_pad_status[number].substickX != GCPadStatus::C_STICK_CENTER_X)
	  PadStatus->substickX = LuaThread::m_pad_status[number].substickX;

  if (LuaThread::m_pad_status[number].substickY != GCPadStatus::C_STICK_CENTER_Y)
	  PadStatus->substickY = LuaThread::m_pad_status[number].substickY;

  PadStatus->button |= LuaThread::m_pad_status[number].button;

  //Update internal gamepad representation with the same struct we're sending out
  m_last_pad_status[number] = *PadStatus;
}

void LuaThread::GetWiiValues(u8 *data, WiimoteEmu::ReportFeatures rptf, int controllerID, int ext, const wiimote_key key)
{
	//u8 *const coreData = rptf.core ? (data + rptf.core) : nullptr;
	//u8 *const accelData = rptf.accel ? (data + rptf.accel) : nullptr;
	//u8 *const irData = rptf.ir ? (data + rptf.ir) : nullptr;
	u8 *const extData = rptf.ext ? (data + rptf.ext) : nullptr;

	if (LuaThread::m_padWii_status[controllerID].hex != 0)
		((wm_buttons *)(rptf.core ? (data + rptf.core) : nullptr))->hex |= LuaThread::m_padWii_status[controllerID].hex;

	u8 mode;
	// Mode 5 not supported in core anyway.
	if (rptf.ext)
		mode = (rptf.ext - rptf.ir) == 10 ? 1 : 3;
	else
		mode = (rptf.size - rptf.ir) == 10 ? 1 : 3;

	if (mode == 1)
	{

	}
	else
	{

	}

	if (extData && ext == 1)
	{
		if (LuaThread::nunchuk[controllerID].bt.hex != 0)
			((wm_nc *)(rptf.ext ? (data + rptf.ext) : nullptr))->bt.hex |= LuaThread::nunchuk[controllerID].bt.hex;
		if (LuaThread::nunchuk[controllerID].jx != WiimoteEmu::Nunchuk::STICK_CENTER)
			((wm_nc *)(rptf.ext ? (data + rptf.ext) : nullptr))->jx = LuaThread::nunchuk[controllerID].jx;
		if (LuaThread::nunchuk[controllerID].jy != WiimoteEmu::Nunchuk::STICK_CENTER)
			((wm_nc *)(rptf.ext ? (data + rptf.ext) : nullptr))->jy = LuaThread::nunchuk[controllerID].jy;
	}
	// Update internal gamepad representation with the same struct we're sending out
	m_last_padWii_status[controllerID] = *((wm_buttons *)(rptf.core ? (data + rptf.core) : nullptr));
	last_nunchuk[controllerID] = *((wm_nc *)(rptf.ext ? (data + rptf.ext) : nullptr));
}

void HookFunction(lua_State* L, lua_Debug* ar)
{
  if (LuaScriptFrame::GetCurrentInstance()->GetLuaThread()->m_destruction_flag)
  {
    luaL_error(L, "Script exited.\n");
  }
}

}  // namespace Lua
