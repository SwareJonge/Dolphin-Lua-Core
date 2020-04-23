// Copyright 2020 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

// Class written by Tales Carvalho (a.k.a. THC98)

#include <string>

#include <wx/dialog.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/headerctrl.h>
#include <wx/menu.h>
#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/grid.h>

#include "Core/Movie.h"

#include "InputCommon/GCPadStatus.h"

class TAStudioFrame : public wxDialog
{
	protected:
		wxStaticBoxSizer* m_controlWrapper;
		wxListCtrl* m_inputList;
		wxTextCtrl* m_saveFrameState;
		wxTextCtrl* m_inputFrameCount;
		wxTextCtrl* m_aX;
		wxTextCtrl* m_aY;
		wxTextCtrl* m_A;
		wxTextCtrl* m_B;
		wxTextCtrl* m_X;
		wxTextCtrl* m_Y;
		wxTextCtrl* m_S;
		wxTextCtrl* m_Z;
		wxTextCtrl* m_L;
		wxTextCtrl* m_R;
		wxTextCtrl* m_Lanalog;
		wxTextCtrl* m_Ranalog;
		wxTextCtrl* m_dUp;
		wxTextCtrl* m_dDown;
		wxTextCtrl* m_dLeft;
		wxTextCtrl* m_dRight;
		wxTextCtrl* m_cX;
		wxTextCtrl* m_cY;
		//wxTextCtrl* m_currentInput;

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

