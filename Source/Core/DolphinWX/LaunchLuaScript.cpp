// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include <wx/bitmap.h>
#include <wx/defs.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/gdicmn.h>
#include <wx/image.h>
#include <wx/mstream.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/translation.h>
#include <wx/windowid.h>
#include <wx/msgdlg.h>
#include <array>
#include <wx/filepicker.h>
#include <wx/utils.h>

#include "Common/Common.h"
#include "DolphinWX/LaunchLuaScript.h"
#include "Core/Core.h"
#include "Core/HW/Memmap.h"
#include "Common/IniFile.h"
#include "Common/CommonPaths.h"

#include "Common/StringUtil.h"
#include "Common/FileUtil.h"
#include "DiscIO/Filesystem.h"
#include "Common/FileSearch.h"

#include "DiscIO/FileSystemGCWii.h"
#include "DiscIO/Volume.h"
#include "DiscIO/VolumeCreator.h"
#include "Core/ConfigManager.h"
#include "Core/Movie.h"
#include "Core/LUA/Lua.h"

#include "DolphinWX/ISOFile.h"
#include "DolphinWX/ISOProperties.h"
#include "DolphinWX/WxUtils.h"

//Dragonbane
BEGIN_EVENT_TABLE(LuaWindow, wxDialog)

EVT_CHOICE(1, LuaWindow::OnSelectionChanged) //Script Selection
EVT_BUTTON(2, LuaWindow::OnButtonPressed) //Start
EVT_BUTTON(3, LuaWindow::OnButtonPressed) //Cancel

END_EVENT_TABLE()

LuaWindow::LuaWindow(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
	SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer(0, 2, 0, 0);
	fgSizer1->SetFlexibleDirection(wxBOTH);
	fgSizer1->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_staticText10 = new wxStaticText(this, wxID_ANY, wxT("Script File:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText10->Wrap(-1);
	m_staticText10->SetFont(wxFont(wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString));
	fgSizer1->Add(m_staticText10, 0, wxALIGN_CENTER | wxALL, 25);

	//Script Choice
	wxArrayString m_choice_scriptChoices;
	m_choice_script = new wxChoice(this, 1, wxDefaultPosition, wxSize(200, -1), m_choice_scriptChoices, 0);
	fgSizer1->Add(m_choice_script, 0, wxALIGN_CENTER | wxALL, 10);

	fgSizer1->Add(0, 10, 1, wxEXPAND, 5);
	fgSizer1->Add(0, 10, 1, wxEXPAND, 5);

	m_button4 = new wxButton(this, 2, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0);
	fgSizer1->Add(m_button4, 0, wxALIGN_RIGHT | wxLEFT, 50);

	m_button5 = new wxButton(this, 3, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	fgSizer1->Add(m_button5, 0, wxALIGN_CENTER | wxRIGHT, 10);

	fgSizer1->AddSpacer(20);

	SetSizer(fgSizer1);
	Layout();

	fgSizer1->Fit(this);

	Centre(wxBOTH);

	Bind(wxEVT_CLOSE_WINDOW, &LuaWindow::OnCloseWindow, this);
}

void LuaWindow::OnSelectionChanged(wxCommandEvent& event)
{
	if (event.GetId() == 1) //Script Selection
	{
	}
}

void LuaWindow::OnButtonPressed(wxCommandEvent& event)
{
	if (!Core::IsRunningAndStarted())
	{
		wxMessageBox("A game needs to be running in order to execute scripts!");
		return;
	}

	wxString selectedScriptName = m_choice_script->GetStringSelection();

	if (selectedScriptName == wxEmptyString || selectedScriptName.Len() < 3)
	{
		wxMessageBox("No script selected!");
		return;
	}

	std::string FileName = WxStrToStr(selectedScriptName);

	if (event.GetId() == 2) //Start
	{
		if (File::Exists(File::GetUserPath(D_SCRIPTS_IDX) + FileName) == false)
		{
			wxMessageBox("Script file does not exist anymore!");
			return;
		}

		if (Lua::IsScriptRunning(FileName))
		{
			wxMessageBox("Script is already running!");
			return;
		}

		Lua::LoadScript(FileName);
	}

	if (event.GetId() == 3) //Cancel
	{
		if (Lua::IsScriptRunning(FileName) == false)
		{
			wxMessageBox("Script is not loaded!");
			return;
		}

		Lua::TerminateScript(FileName);
	}
}

void LuaWindow::Shown()
{
	//Refresh Script List
	m_choice_script->Clear();

	//Find all Lua files
	std::vector<std::string> rFilenames = DoFileSearch({".lua"}, {File::GetUserPath(D_SCRIPTS_IDX)});

	if (rFilenames.size() > 0)
	{
		for (u32 i = 0; i < rFilenames.size(); i++)
		{
			std::string FileName;
			SplitPath(rFilenames[i], nullptr, &FileName, nullptr);

			if (FileName.substr(0, 1).compare("_"))
			{
				m_choice_script->Append(StrToWxStr(FileName + ".lua"));
			}
		}
	}
}

void LuaWindow::OnCloseWindow(wxCloseEvent& event)
{
	if (event.CanVeto())
	{
		event.Skip(false);
		Show(false);
	}
}
