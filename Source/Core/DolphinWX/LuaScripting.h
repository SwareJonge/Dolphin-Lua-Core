// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <map>
#include <mutex>
#include <stdio.h>
#include <wx/frame.h>

#include "Common/CommonTypes.h"
#include "Core/HW/WiimoteEmu/Attachment/Nunchuk.h"
#include "Core/HW/WiimoteEmu/WiimoteEmu.h"
#include "InputCommon/GCPadStatus.h"

// Forward Declarations
class wxString;
class wxFileDialog;
class wxTextCtrl;
class wxStaticText;
class wxMenuItem;
class wxMenuBar;
class wxMenu;
class wxButton;
struct lua_State;
struct lua_Debug;
struct luaL_Reg;


  struct LuaWiimote
  {
	  wm_buttons m_padWii_status;
	  wm_accel accelData;
	  wm_nc m_nunchuk;
	  wm_ir_basic ir_data;
	  wm_ir_extended ext_ir_data;
	  wm_classic_extension m_classic;
  };

namespace Lua
{
using LuaFunction = int (*)(lua_State* L);

class LuaScriptFrame;

void HookFunction(lua_State*, lua_Debug*);
int luaopen_libs(lua_State* L);

class LuaThread final : public wxThread
{
public:
  LuaThread(LuaScriptFrame* p, const wxString& file);
  ~LuaThread();
  void GetValues(GCPadStatus *PadStatus, int number);
  void GetWiiValues(u8 *data, WiimoteEmu::ReportFeatures rptf, int controllerID, int ext, const wiimote_key key);
  bool m_destruction_flag = false;
  //std::vector<GCPadStatus> m_pad_status;


  GCPadStatus m_pad_status[4];
  LuaWiimote m_Lua_Wiimote[4];

  GCPadStatus m_last_pad_status[4];
  LuaWiimote m_last_Lua_Wiimote[4];

private:
  LuaScriptFrame* m_parent = nullptr;
  wxString m_file_path;
  wxThread::ExitCode Entry() override;
};

class LuaScriptFrame final : public wxFrame
{
public:
  LuaScriptFrame(wxWindow* parent);
  ~LuaScriptFrame();

  void Log(const wxString& message);
  void NullifyLuaThread();
  GCPadStatus& GetPadStatus(int number);  
  wm_buttons& GetPadWiiStatus(int number);
  wm_nc& GetNunchukStatus(int number);
  GCPadStatus GetLastPadStatus(int number);
  wm_buttons GetLastPadWiiStatus(int number);
  LuaThread* GetLuaThread();
  static LuaScriptFrame* GetCurrentInstance();

private:
  void CreateGUI();
  void CreateMenuBar();
  void OnClearClicked(wxCommandEvent& event);
  void OnClose(wxCloseEvent& event);
  void OnDocumentationClicked(wxCommandEvent& event);
  void OnAPIClicked(wxCommandEvent& event);
  void BrowseOnButtonClick(wxCommandEvent& event);
  void RunOnButtonClick(wxCommandEvent& event);
  void StopOnButtonClick(wxCommandEvent& event);
  wxMenuBar* m_menubar;
  wxMenuItem* m_clear;
  wxMenuItem* m_documentation;
  wxMenuItem* m_api;
  wxMenu* m_console_menu;
  wxMenu* m_help_menu;
  wxStaticText* m_script_file_label;
  wxTextCtrl* m_file_path;
  wxButton* m_browse_button;
  wxButton* m_run_button;
  wxButton* m_stop_button;
  wxStaticText* m_output_console_literal;
  wxTextCtrl* m_output_console;
  LuaThread* m_lua_thread;
  static LuaScriptFrame* m_current_instance;
};
}  // namespace Lua
