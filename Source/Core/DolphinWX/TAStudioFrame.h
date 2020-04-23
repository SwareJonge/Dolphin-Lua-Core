// Copyright 2020 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

// Class written by Tales Carvalho (a.k.a. THC98)

#include <string>

#include <wx/dialog.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>

#include "Core/Movie.h"

#include "InputCommon/GCPadStatus.h"

class TAStudioFrame : public wxDialog
{
	protected:
		wxListCtrl* m_inputList;
		wxStaticBoxSizer* m_controlWrapper;
		wxTextCtrl* m_inputFrameCount;
		wxTextCtrl* m_currentInput;

	public:
		TAStudioFrame(wxWindow* parent,
			wxWindowID id = wxID_ANY,
			const wxString& title = _("TAStudio"),
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxDEFAULT_DIALOG_STYLE | wxSTAY_ON_TOP | wxRESIZE_BORDER);
		void GetInput(GCPadStatus* PadStatus);
		void SetInput(GCPadStatus* PadStatus);
	
};

