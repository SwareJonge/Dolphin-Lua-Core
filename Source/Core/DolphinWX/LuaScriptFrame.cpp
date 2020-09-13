// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.


#include <lua.hpp>
#include <lua.h>
#include <luaconf.h>

#include <wx/button.h>
#include <wx/filedlg.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include "Core/Core.h"
#include "Core/HW/GCPad.h"
#include "Core/HW/GCPadEmu.h"
#include "Core/State.h"
#include "Core/HW/ProcessorInterface.h"

#include "Core/HW/Wiimote.h"
#include "Core/HW/WiimoteEmu/Attachment/Classic.h"
#include "Core/HW/WiimoteEmu/Attachment/Nunchuk.h"
#include "Core/HW/WiimoteEmu/WiimoteEmu.h"
#include "Core/HW/WiimoteReal/WiimoteReal.h"

#include "Core/Movie.h"
#include "DolphinWX/Frame.h"
#include "DolphinWX/Main.h"
#include "DolphinWX/WxUtils.h"
#include "DolphinWX/LuaScripting.h"
#include "InputCommon/GCPadStatus.h"
#include "Core/PowerPC/PowerPC.h"
#include "InputCommon/InputConfig.h"
#include "VideoCommon/RenderBase.h"
#include "Core/Host.h"
#include "Core/LUA/Lua.h"
#include "VideoCommon/Statistics.h"



namespace Lua
{
static int band(lua_State* L);

static int printToTextCtrl(lua_State* L);

// Memory Library
static int ReadByte(lua_State *L);
static int ReadShort(lua_State *L);
static int ReadInt(lua_State *L);
static int ReadFloat(lua_State *L);
static int ReadString(lua_State *L);
static int WriteByte(lua_State *L);
static int WriteShort(lua_State *L);
static int WriteInt(lua_State *L);
static int WriteFloat(lua_State *L);
static int WriteString(lua_State *L);

static int DrawText(lua_State *L);
static int Msg(lua_State *L);

static int frameAdvance(lua_State* L);
static int getFrameCount(lua_State* L);
static int getMovieLength(lua_State* L);
static int softReset(lua_State* L);

// Slots from 0-99.
static int saveState(lua_State* L);
static int loadState(lua_State* L);

// The argument should be a percentage of speed. I.E. setEmulatorSpeed(100) would set it to normal
// playback speed
static int setEmulatorSpeed(lua_State* L);

static int getAnalog(lua_State* L);
static int setAnalog(lua_State* L);
static int setAnalogPolar(lua_State* L);
static int getCStick(lua_State* L);
static int setCStick(lua_State* L);
static int setCStickPolar(lua_State* L);
static int setnunstick(lua_State *L);
static int setnunstickPolar(lua_State *L);
static int getButtons(lua_State *L);
static int setWiiButtons(lua_State *L);
static int setNunchukButtons(lua_State *L);
static int setButtons(lua_State *L);
static int setDPad(lua_State* L);
static int setWiimoteDPad(lua_State *L);
static int getTriggers(lua_State* L);
static int setTriggers(lua_State* L);
LuaScriptFrame *LuaScriptFrame::m_current_instance;

LuaScriptFrame::LuaScriptFrame(wxWindow* parent)
    : wxFrame(parent, wxID_ANY, _("Lua Console"), wxDefaultPosition, wxSize(431, 397),
              wxDEFAULT_FRAME_STYLE ^ wxRESIZE_BORDER)
{
  CreateGUI();
  //SetIcons(WxUtils::GetDolphinIconBundle());

  Center();
  Show();

  m_current_instance = this;
  m_lua_thread = nullptr;
}

LuaScriptFrame::~LuaScriptFrame()
{
  // Stop currently executing Lua thread
  if (m_lua_thread)
  {
    // wxThread.Kill() crashes on non-windows platforms,
    // and right now I don't have a solution for Mac/Linux,
    // so I'm kind of cheating here.
#ifdef _WIN32
    m_lua_thread->Kill();
    m_lua_thread = nullptr;
#else
    m_lua_thread->m_destruction_flag = true;
    wxThread::This()->Sleep(1);
#endif
  }
  m_current_instance = nullptr;
  main_frame->m_lua_script_frame = nullptr;
}

void LuaScriptFrame::OnClose(wxCloseEvent& event)
{
	if (!m_lua_thread)
	{
		Destroy();
	}
	else
	{		
	    wxMessageBox(_("You must stop your script before closing this window!"));
	}
	return;
}

//
// CreateGUI
//
// Creates actual Lua console window.
//
void LuaScriptFrame::CreateGUI()
{
  SetSizeHints(wxDefaultSize, wxDefaultSize);

  CreateMenuBar();

  wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

  m_script_file_label =
      new wxStaticText(this, wxID_ANY, _("Script File:"), wxDefaultPosition, wxDefaultSize, 0);
  m_script_file_label->Wrap(-1);
  main_sizer->Add(m_script_file_label, 0, wxALL, 5);

  m_file_path =
      new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(300, -1), 0);
  m_file_path->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));

  main_sizer->Add(m_file_path, 0, wxALL, 5);

  wxBoxSizer* buttons = new wxBoxSizer(wxHORIZONTAL);

  m_browse_button = new wxButton(this, wxID_ANY, _("Browse..."), wxPoint(-1, -1), wxDefaultSize, 0);
  buttons->Add(m_browse_button, 0, wxALL, 5);

  m_run_button = new wxButton(this, wxID_ANY, _("Run"), wxDefaultPosition, wxDefaultSize, 0);
  buttons->Add(m_run_button, 0, wxALL, 5);

  m_stop_button = new wxButton(this, wxID_ANY, _("Stop"), wxDefaultPosition, wxDefaultSize, 0);
  buttons->Add(m_stop_button, 0, wxALL, 5);

  main_sizer->Add(buttons, 1, wxEXPAND, 5);

  m_output_console_literal =
      new wxStaticText(this, wxID_ANY, _("Output Console:"), wxDefaultPosition, wxDefaultSize, 0);
  m_output_console_literal->Wrap(-1);
  main_sizer->Add(m_output_console_literal, 0, wxALL, 5);

  m_output_console = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                    wxSize(415, 200), wxHSCROLL | wxTE_MULTILINE | wxTE_READONLY);
  main_sizer->Add(m_output_console, 0, wxALL, 6);

  SetSizer(main_sizer);
  Layout();

  Centre(wxBOTH);

  // Connect Events
  Bind(wxEVT_MENU, &LuaScriptFrame::OnClearClicked, this, m_clear->GetId());
  Bind(wxEVT_MENU, &LuaScriptFrame::OnDocumentationClicked, this, m_documentation->GetId());
  Bind(wxEVT_MENU, &LuaScriptFrame::OnAPIClicked, this, m_api->GetId());

#ifndef __WIN32
  Bind(wxEVT_CLOSE_WINDOW, &LuaScriptFrame::OnClose, this, wxID_ANY);
#endif

  m_browse_button->Bind(wxEVT_BUTTON, &LuaScriptFrame::BrowseOnButtonClick, this,
                        m_browse_button->GetId());

  m_run_button->Bind(wxEVT_BUTTON, &LuaScriptFrame::RunOnButtonClick, this, m_run_button->GetId());

  m_stop_button->Bind(wxEVT_BUTTON, &LuaScriptFrame::StopOnButtonClick, this,
                      m_stop_button->GetId());
}

void LuaScriptFrame::CreateMenuBar()
{
  m_menubar = new wxMenuBar(0);
  m_console_menu = new wxMenu();
  m_clear =
      new wxMenuItem(m_console_menu, wxID_ANY, wxString(_("Clear")), wxEmptyString, wxITEM_NORMAL);
  m_console_menu->Append(m_clear);
  m_help_menu = new wxMenu();
  m_documentation = new wxMenuItem(m_help_menu, wxID_ANY, wxString(_("Lua Documentation")),
                                   wxEmptyString, wxITEM_NORMAL);
  m_api = new wxMenuItem(m_help_menu, wxID_ANY, wxString(_("Dolphin Lua API")), wxEmptyString,
                         wxITEM_NORMAL);
  m_help_menu->Append(m_documentation);
  m_help_menu->Append(m_api);

  m_menubar->Append(m_console_menu, _("Console"));
  m_menubar->Append(m_help_menu, _("Help"));

  SetMenuBar(m_menubar);
}

void LuaScriptFrame::Log(const wxString& message)
{
  m_output_console->AppendText(message);
}

void LuaScriptFrame::OnClearClicked(wxCommandEvent& event)
{
  m_output_console->Clear();
}

void LuaScriptFrame::OnDocumentationClicked(wxCommandEvent& event)
{
  wxLaunchDefaultBrowser("https://www.lua.org/pil/contents.html");
}

void LuaScriptFrame::OnAPIClicked(wxCommandEvent& event)
{
  wxLaunchDefaultBrowser("https://github.com/NickDriscoll/dolphin/blob/Lua_scripting/Lua.md");
}

void LuaScriptFrame::BrowseOnButtonClick(wxCommandEvent& event)
{
  wxFileDialog dialog(this, _("Select Lua script."));

  if (dialog.ShowModal() == wxID_CANCEL)
    return;

  m_file_path->SetValue(dialog.GetPath());
  dialog.Destroy();
}

void LuaScriptFrame::RunOnButtonClick(wxCommandEvent& event)
{
  if (m_lua_thread == nullptr)
  {	  
    m_lua_thread = new LuaThread(this, m_file_path->GetValue());
    m_lua_thread->Run();
  }
}

void LuaScriptFrame::StopOnButtonClick(wxCommandEvent& event)
{
  if (m_lua_thread)
  {
    m_lua_thread->m_destruction_flag = true;
  }
}

void LuaScriptFrame::NullifyLuaThread()
{
  m_lua_thread = nullptr;
}

GCPadStatus& LuaScriptFrame::GetPadStatus(int number)
{
  return m_lua_thread->m_pad_status[number];
}

wm_buttons& LuaScriptFrame::GetPadWiiStatus(int number)
{
	return m_lua_thread->m_Lua_Wiimote[number].m_padWii_status;
}

wm_nc& LuaScriptFrame::GetNunchukStatus(int number)
{
	return m_lua_thread->m_Lua_Wiimote[number].m_nunchuk;
}

GCPadStatus LuaScriptFrame::GetLastPadStatus(int number)
{
  return m_lua_thread->m_last_pad_status[number];
}

wm_buttons LuaScriptFrame::GetLastPadWiiStatus(int number)
{
	return m_lua_thread->m_last_Lua_Wiimote[number].m_padWii_status;
}

LuaThread* LuaScriptFrame::GetLuaThread()
{
  return m_lua_thread;
}

LuaScriptFrame* LuaScriptFrame::GetCurrentInstance()
{
  if (m_current_instance == nullptr)
    m_current_instance = new LuaScriptFrame(main_frame);
  return m_current_instance;
}

// Functions to register with Lua
#pragma region Lua_Functs

static int band(lua_State *L)
{
	lua_pushinteger(L, lua_tointeger(L, 1) & lua_tointeger(L, 2));
	return 1;
}

// Prints a string to the text control of this frame
static int printToTextCtrl(lua_State *L)
{
	LuaScriptFrame::GetCurrentInstance()->GetEventHandler()->CallAfter(&LuaScriptFrame::Log,
	                                                                   wxString(lua_tostring(L, 1)));
	LuaScriptFrame::GetCurrentInstance()->GetEventHandler()->CallAfter(&LuaScriptFrame::Log, wxString("\n"));

	return 0;
}

static int ReadByte(lua_State *L)
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

static int ReadShort(lua_State *L)
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

static int ReadInt(lua_State *L)
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
		// result = Memory::Read_U8(LastOffset);
	}
	lua_pushinteger(L, result); // return value
	return 1;                   // number of return values
}

static int ReadFloat(lua_State *L)
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
static int ReadString(lua_State *L)
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
static int WriteByte(lua_State *L)
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

static int WriteShort(lua_State *L)
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

static int WriteInt(lua_State *L)
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

static int WriteFloat(lua_State *L)
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
static int WriteString(lua_State *L)
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

static int GetPointer(lua_State *L)
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

static int DrawText(lua_State *L)
{
	int argc = lua_gettop(L);
	// check if there are less than 3 arguments
	if (argc < 1)
	{
		return 0; // if so don't execute further
	}

	std::string text;
	text = StringFromFormat("%s", lua_tostring(L, 1));
	text.append("\n");
	Statistics::SetString(text);

	return 0;
}

static int Msg(lua_State *L)
{
	int argc = lua_gettop(L);

	if (argc < 1)
		return 0;

	const char *text = lua_tostring(L, 1);

	int delay = 5000; // Default: 5 seconds
	// if there is a second argument let user define delay
	if (argc == 2)
	{
		delay = lua_tointeger(L, 2);
	}

	std::string message = StringFromFormat("Lua Msg: %s", text);

	Core::DisplayMessage(message, delay);

	return 0; // number of return values
}

// Steps a frame if the emulator is paused, pauses it otherwise.
static int frameAdvance(lua_State *L)
{
	u64 current_frame = Movie::GetCurrentFrame();
	Movie::DoFrameStep();

	// Block until a frame has actually processed
	// Prevents a script from executing it's main loop more than once per frame.
	while (current_frame == Movie::GetCurrentFrame());
	return 0;
}

static int getFrameCount(lua_State *L)
{
	lua_pushinteger(L, Movie::g_currentFrame);
	return 1;
}

static int getMovieLength(lua_State *L)
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

static int softReset(lua_State *L)
{
	ProcessorInterface::ResetButton_Tap();
	return 0;
}

static int saveState(lua_State *L)
{
	int slot = lua_tointeger(L, 1);

	State::Save(slot);
	return 0;
}

static int loadState(lua_State *L)
{
	int slot = lua_tointeger(L, 1);

	State::Load(slot);
	return 0;
}

static int getAnalog(lua_State *L)
{
	int number = lua_tointeger(L, 1) - 1;
	lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastPadStatus(number).stickX);
	lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastPadStatus(number).stickY);

	return 2;
}

static int setAnalog(lua_State* L)
{
	int number = lua_tointeger(L, 1) - 1;
	if (lua_gettop(L) != 3)
	{
		return luaL_error(L, "Incorrect # of arguments passed to setAnalog. setAnalog expects 3 arguments/n");
	}

	u8 x_pos = lua_tointeger(L, 2);
	u8 y_pos = lua_tointeger(L, 3);

	LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).stickX = x_pos;
	LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).stickY = y_pos;

	return 0;
}

// Same thing as setAnalog except it takes polar coordinates
// Must use an m int the range [0, 128)
static int setAnalogPolar(lua_State* L)
{

	int number = lua_tointeger(L, 1) - 1;
	int m = lua_tointeger(L, 2);
	if (m < 0 || m >= 128)
	{
		return luaL_error(L, "m is outside of acceptable range [0, 128)");
	}

	// Gotta convert theta to radians
	double theta = lua_tonumber(L, 3) * M_PI / 180.0;

	// Round to the nearest whole number, then subtract 128 so that our
	//"origin" is the stick in neutral position.
	LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).stickX = static_cast<u8>(floor(m * cos(theta)) + 128);
	LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).stickY = static_cast<u8>(floor(m * sin(theta)) + 128);

	return 0;
}

static int getCStick(lua_State* L)
{
	int number = lua_tointeger(L, 1) - 1;
	lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastPadStatus(number).substickX);
	lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastPadStatus(number).substickY);

	return 2;
}


static int setnunstick(lua_State *L)
{
	int number = lua_tointeger(L, 1) - 1; 
	if (lua_gettop(L) != 3)
	{
		return luaL_error(L, "Incorrect # of arguments passed to setAnalog. setAnalog expects 3 arguments/n");
	}

	LuaScriptFrame::GetCurrentInstance()->GetNunchukStatus(number).jx = lua_tonumber(L, 2);
	LuaScriptFrame::GetCurrentInstance()->GetNunchukStatus(number).jy = lua_tonumber(L, 3);

	return 0;
}

static int setnunstickPolar(lua_State *L)
{
	int number = lua_tointeger(L, 1) - 1;
	int m = lua_tointeger(L, 2);
	if (m < 0 || m >= 128)
	{
		return luaL_error(L, "m is outside of acceptable range [0, 128)");
	}

	int theta = lua_tointeger(L, 3);

	// Convert theta to radians
	theta = theta * M_PI / 180.0;

	LuaScriptFrame::GetCurrentInstance()->GetNunchukStatus(number).jx = (u8)(floor(m * cos(theta)) + 128);
	LuaScriptFrame::GetCurrentInstance()->GetNunchukStatus(number).jy = (u8)(floor(m * sin(theta)) + 128);

	return 0;
}



static int setCStick(lua_State* L)
{
	int number = lua_tointeger(L, 1) - 1;
    LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).substickX = lua_tointeger(L, 2);
	LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).substickY = lua_tointeger(L, 3);

  return 0;
}

static int setCStickPolar(lua_State* L)
{
	int number = lua_tointeger(L, 1) - 1;
	int m = lua_tointeger(L, 2);
	if (m < 0 || m >= 128)
	{
		return luaL_error(L, "m is outside of acceptable range [0, 128)");
	}

	int theta = lua_tointeger(L, 3);

	// Convert theta to radians
	theta = theta * M_PI / 180.0;

	LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).substickX = (u8)(floor(m * cos(theta)) + 128);
	LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).substickY = (u8)(floor(m * sin(theta)) + 128);

	return 0;
}

static int getButtons(lua_State* L)
{
	int number = lua_tointeger(L, 1) - 1;
	lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastPadStatus(number).button);
	return 1;
}

// Each button passed in is set to pressed.
static int setButtons(lua_State* L)
{
	int number = lua_tointeger(L, 1) - 1;
	const char *s = lua_tostring(L, 2);

	for (size_t i = 0; i < strlen(s); i++)
	{
		if (s[i] == 'U')
		{
			LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button = 0;
			break;
		}

		switch (s[i])
		{
		case 'A':
			LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button |= PadButton::PAD_BUTTON_A;
			break;
		case 'B':
			LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button |= PadButton::PAD_BUTTON_B;
			break;
		case 'X':
			LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button |= PadButton::PAD_BUTTON_X;
			break;
		case 'Y':
			LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button |= PadButton::PAD_BUTTON_Y;
			break;
		case 'S':
			LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button |= PadButton::PAD_BUTTON_START;
			break;
		case 'Z':
			LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button |= PadButton::PAD_TRIGGER_Z;
			break;
		}
	}
	return 0;
}

static int setNunchukButtons(lua_State *L)
{
	int number = lua_tointeger(L, 1) - 1;
	const char *s = lua_tostring(L, 2);

	for (size_t i = 0; i < strlen(s); i++)
	{
		switch (s[i])
		{
		case 'C':
			LuaScriptFrame::GetCurrentInstance()->GetNunchukStatus(number).bt.hex |= WiimoteEmu::Nunchuk::BUTTON_C;
			LuaScriptFrame::GetCurrentInstance()->GetNunchukStatus(number).bt.c = 0xFF;
			break;
		case 'Z':
			LuaScriptFrame::GetCurrentInstance()->GetNunchukStatus(number).bt.hex |= WiimoteEmu::Nunchuk::BUTTON_Z;
			LuaScriptFrame::GetCurrentInstance()->GetNunchukStatus(number).bt.z = 0xFF;
			break;
		}
	}
	return 0;

}

static int setWiiButtons(lua_State* L)
{
	int number = lua_tointeger(L, 1) - 1;
	const char *s = lua_tostring(L, 2);

	for (size_t i = 0; i < strlen(s); i++)
	{
		switch (s[i])
		{
		case 'A':
			LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).hex |= WiimoteEmu::Wiimote::BUTTON_A;
			LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).a = 0xFF;
			break;
		case 'B':
			LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).hex |= WiimoteEmu::Wiimote::BUTTON_B;
			LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).b = 0xFF;
			break;
		case '-':
			LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).hex |= WiimoteEmu::Wiimote::BUTTON_MINUS;
			LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).minus = 0xFF;
			break;
		case '+':
			LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).hex |= WiimoteEmu::Wiimote::BUTTON_PLUS;
			LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).plus = 0xFF;
			break;
		case '1':
			LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).hex |= WiimoteEmu::Wiimote::BUTTON_ONE;
			LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).one = 0xFF;
			break;
		case '2':
			LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).hex |= WiimoteEmu::Wiimote::BUTTON_TWO;
			LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).two = 0xFF;
			break;
		}
	}
	return 0;
}

static int setWiimoteDPad(lua_State *L)
{
	int number = lua_tointeger(L, 1) - 1;
	const char *str = lua_tostring(L, 2);

	for (size_t i = 0; i < strlen(str); i++)
	{
		switch (str[i])
		{
		case 'U':
			LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).hex |= WiimoteEmu::Wiimote::PAD_UP;
			break;
		case 'D':
			LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).hex |= WiimoteEmu::Wiimote::PAD_DOWN;
			break;
		case 'L':
			LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).hex |= WiimoteEmu::Wiimote::PAD_LEFT;
			break;
		case 'R':
			LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).hex |= WiimoteEmu::Wiimote::PAD_RIGHT;
			break;
		}
	}	

	return 0;
}

static int setDPad(lua_State* L)
{
	int number = lua_tointeger(L, 1) - 1;
	const char *str = lua_tostring(L, 2);

	for (size_t i = 0; i < strlen(str); i++)
	{
		switch (str[i])
		{
		case 'U':
			LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button |= PadButton::PAD_BUTTON_UP;
			break;
		case 'D':
			LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button |= PadButton::PAD_BUTTON_DOWN;
			break;
		case 'L':
			LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button |= PadButton::PAD_BUTTON_LEFT;
			break;
		case 'R':
			LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button |= PadButton::PAD_BUTTON_RIGHT;
			break;
		}
	}

	return 0;
}

static int setEmulatorSpeed(lua_State *L)
{
	SConfig::GetInstance().m_EmulationSpeed = lua_tonumber(L, 1) * 0.01f;

	return 0;
}

static int getTriggers(lua_State *L)
{
	int number = lua_tointeger(L, 1) - 1;
	lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastPadStatus(number).triggerLeft);
	lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastPadStatus(number).triggerRight);

	return 2;
}

static int setTriggers(lua_State *L)
{
	int number = lua_tointeger(L, 1) - 1;
	LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).triggerLeft = lua_tointeger(L, 2);
	LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).triggerRight = lua_tointeger(L, 3);

	return 0;
}

int luaopen_libs(lua_State* L)
{
  static const luaL_Reg bit[] =
  {
    {"band", band},
    {nullptr, nullptr}
  };

  static const luaL_Reg client[] =
  {
    {"print", printToTextCtrl },
    {nullptr, nullptr}
  };

  static const luaL_Reg gui[] =
  {	  
	  {"DrawText", DrawText}, 
      {"Msg", Msg},
      {nullptr, nullptr}
  };

  static const luaL_Reg memory[] =
  {	  
	  {"ReadByte", ReadByte},
      {"ReadShort", ReadShort},
	  {"ReadInt", ReadInt},
      {"ReadFloat", ReadFloat},
      {"ReadString", ReadString},
	  // Write Stuff
	  {"WriteByte", WriteByte},
	  {"WriteShort", WriteShort},
	  {"WriteInt", WriteInt},
	  {"WriteFloat", WriteFloat},
	  {"WriteString", WriteString},
	  // Pointer
	  {"GetPointer", GetPointer},
      {nullptr, nullptr}
  };

  static const luaL_Reg emu[] =
  {
    {"frameAdvance", frameAdvance },
    {"getFrameCount", getFrameCount },
    {"getMovieLength", getMovieLength },
    {"softReset", softReset },
    {"saveState", saveState },
    {"loadState", loadState },
    {"setEmulatorSpeed", setEmulatorSpeed },
    {nullptr, nullptr}
  };

  static const luaL_Reg joypadGC[] =
  {
    {"getAnalog", getAnalog },
    {"setAnalog", setAnalog },
    {"setAnalogPolar", setAnalogPolar },
    {"getCStick", getCStick },
    {"setCStick", setCStick },
    {"setCStickPolar", setCStickPolar },
    {"getButtons", getButtons },
    {"setButtons", setButtons },
    {"setDPad", setDPad },
    {"getTriggers", getTriggers },
    {"setTriggers", setTriggers },
    {nullptr, nullptr}
  };

  static const luaL_Reg joypadWii[] = 
  {
	  //{"setIR", setIR}, 
	  {"setButtons", setWiiButtons}, 
      {"setNunStick", setnunstick},
	  {"setNunchukButtons", setNunchukButtons},
	  {"setDPad", setWiimoteDPad},
	  {nullptr, nullptr}

  };




  luaL_newlib(L, joypadGC);
  lua_setglobal(L, "joypad_GC");
  luaL_newlib(L, joypadWii);
  lua_setglobal(L, "joypad_Wii");
  luaL_newlib(L, bit);
  lua_setglobal(L, "bit");
  luaL_newlib(L, gui);
  lua_setglobal(L, "gui");
  luaL_newlib(L, memory);
  lua_setglobal(L, "memory");
  luaL_newlib(L, client);
  lua_setglobal(L, "client");
  luaL_newlib(L, emu);
  lua_setglobal(L, "emu");
  return 1;
}
#pragma endregion

}  // namespace Lua
