// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

//Dragonbane

#include <wx/defs.h>
#include <wx/dialog.h>
#include <wx/gdicmn.h>
#include <wx/string.h>
#include <wx/translation.h>
#include <wx/windowid.h>
#include <wx/artprov.h>
#include <wx/stattext.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>

#include <wx/artprov.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/frame.h>


#include "Common/CommonTypes.h"
#include <string>


class wxWindow;
class wxButton;

class LuaWindow : public wxDialog
{
private:

	DECLARE_EVENT_TABLE();

protected:
	wxChoice* m_choice_script;
	wxPanel* m_panel1;
	wxButton* m_button4;
	wxButton* m_button5;
	wxStaticText* m_staticText10;

public:

	LuaWindow(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Launch Script by DB"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style  = wxDEFAULT_DIALOG_STYLE | wxSTAY_ON_TOP);
	void OnSelectionChanged(wxCommandEvent& event);
	void OnButtonPressed(wxCommandEvent& event);
	void OnCloseWindow(wxCloseEvent& event);

	void Shown();

};
