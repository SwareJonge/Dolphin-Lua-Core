// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.


#include <wx/button.h>
#include <wx/filedlg.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include "DolphinWX/Frame.h"
#include "DolphinWX/Main.h"

#include "DolphinWX/LuaScripting.h"
#include "DolphinWX/LUA_API.h"



namespace Lua
{
	
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

wm_classic_extension& LuaScriptFrame::GetClassicStatus(int number)
{
	return m_lua_thread->m_Lua_Wiimote[number].m_classic;
}

GCPadStatus LuaScriptFrame::GetLastPadStatus(int number)
{
  return m_lua_thread->m_last_pad_status[number];
}

wm_buttons LuaScriptFrame::GetLastPadWiiStatus(int number)
{
	return m_lua_thread->m_last_Lua_Wiimote[number].m_padWii_status;
}

wm_nc LuaScriptFrame::GetLastNunchuckStatus(int number)
{
	return m_lua_thread->m_last_Lua_Wiimote[number].m_nunchuk;
}

wm_classic_extension LuaScriptFrame::GetLastClassicStatus(int number)
{
	return m_lua_thread->m_last_Lua_Wiimote[number].m_classic;
}

int LuaScriptFrame::GetLastWiimoteExt(int number)
{
	return m_lua_thread->m_last_Lua_Wiimote[number].m_extension;
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

int luaopen_libs(lua_State* L)
{
  static const luaL_Reg bit[] =
  {
    {"band", Lua::bit::band},
    {nullptr, nullptr}
  };

  static const luaL_Reg console[] =
  {
    {"log", Lua::console::log },
    {nullptr, nullptr}
  };

  static const luaL_Reg emu[] =
  {
    {"frameAdvance", Lua::emu::frameAdvance},
    {"frameCount", Lua::emu::getFrameCount},
	{"lagCount", Lua::emu::getLagCount},
    {"softReset", Lua::emu::softReset},
    {"setEmulatorSpeed", Lua::emu::setEmulatorSpeed},
    {nullptr, nullptr}
  };

  static const luaL_Reg gui[] =
  {	  
	  {"drawText", Lua::gui::DrawText}, 
      {"msg", Lua::gui::Msg},
      {nullptr, nullptr}
  };

  static const luaL_Reg joypadGC[] =
  {
    {"getAnalog", Lua::joypad::GC::getAnalog },
	{"setAnalog", Lua::joypad::GC::setAnalog},
    {"setAnalogPolar", Lua::joypad::GC::setAnalogPolar},
	{"getCStick", Lua::joypad::GC::getCStick},
	{"setCStick", Lua::joypad::GC::setCStick},
	{"setCStickPolar", Lua::joypad::GC::setCStickPolar},
	{"getButtons", Lua::joypad::GC::getButtons},
	{"setButtons", Lua::joypad::GC::setButtons},
	{"getTriggers", Lua::joypad::GC::getTriggers},
	{"setTriggers", Lua::joypad::GC::setTriggers},
    {nullptr, nullptr}
  };

  static const luaL_Reg joypadWii[] = 
  {
	  //{"setIR", setIR}, 
	  {"getButtons", Lua::joypad::Wii::getButtons}, 
	  {"getExtension", Lua::joypad::Wii::getExtension}, 
      {"getNunStick", Lua::joypad::Wii::Nunchuk::getAnalog},
	  {"getNunchukButtons", Lua::joypad::Wii::Nunchuk::getButtons},
	  {"getClassicButtons", Lua::joypad::Wii::Classic::getButtons},
	  {"getClassicStickL", Lua::joypad::Wii::Classic::getAnalogL},
	  {"getClassicStickR", Lua::joypad::Wii::Classic::getAnalogR},

	  {"setButtons", Lua::joypad::Wii::setButtons}, 
      {"setNunStick", Lua::joypad::Wii::Nunchuk::setAnalog},
	  {"setNunchukButtons", Lua::joypad::Wii::Nunchuk::setButtons},
	  {"setClassicButtons", Lua::joypad::Wii::Classic::setButtons},
	  {"setClassicStickL", Lua::joypad::Wii::Classic::setAnalogL},
	  {"setClassicStickR", Lua::joypad::Wii::Classic::setAnalogR},
	  	                                   
	  {nullptr, nullptr}
  };


  static const luaL_Reg memory[] =
  {	  
	  {"ReadByte", Lua::memory::ReadByte},
	  {"ReadShort", Lua::memory::ReadShort},
	  {"ReadInt", Lua::memory::ReadInt},
	  {"ReadFloat", Lua::memory::ReadFloat},
	  {"ReadString", Lua::memory::ReadString},
	  // Write Stuff
	  {"WriteByte", Lua::memory::WriteByte},
	  {"WriteShort", Lua::memory::WriteShort},
	  {"WriteInt", Lua::memory::WriteInt},
	  {"WriteFloat", Lua::memory::WriteFloat},
	  {"WriteString", Lua::memory::WriteString},
	  // Pointer
	  {"GetPointer", Lua::memory::GetPointer},

      {nullptr, nullptr}
  };

  static const luaL_Reg _Movie[]
  {
	  {"startsFromSaveState", Lua::movie::startsFromSaveState},
	  {"startsFromClearSave", Lua::movie::startsFromClearSave},
	  {"isLoaded", Lua::movie::isLoaded},
	  {"length", Lua::movie::length},
	  {"save", Lua::movie::save},
	  {"setReadOnly", Lua::movie::setReadOnly},
	  {"stop", Lua::movie::stop},
	  
      {nullptr, nullptr}
  };

  static const luaL_Reg SaveState[]
  {
	  {"load", Lua::savestate::load},
	  {"loadSlot", Lua::savestate::loadSlot},
	  {"save", Lua::savestate::save},
	  {"saveSlot", Lua::savestate::saveSlot},
	  {nullptr, nullptr}
  };

  luaL_newlib(L, bit);
  lua_setglobal(L, "bit");
  luaL_newlib(L, console);
  lua_setglobal(L, "console");
  luaL_newlib(L, emu);
  lua_setglobal(L, "emu");
  luaL_newlib(L, gui);
  lua_setglobal(L, "gui");

  luaL_newlib(L, joypadGC);
  lua_setglobal(L, "joypad_GC");
  luaL_newlib(L, joypadWii);
  lua_setglobal(L, "joypad_Wii");
  luaL_newlib(L, memory);
  lua_setglobal(L, "memory");
  luaL_newlib(L, _Movie);
  lua_setglobal(L, "movie");
  luaL_newlib(L, SaveState);
  lua_setglobal(L, "savestate");

  return 1;
}
#pragma endregion

}  // namespace Lua
