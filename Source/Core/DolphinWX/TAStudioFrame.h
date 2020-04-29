// Copyright 2020 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

// Class written by Tales Carvalho (a.k.a. THC98)

#include <string>
#include <vector>
#include <algorithm>

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

struct TAStudioInput
{
	u64 FrameCount;
	//u64 InputCount;
	GCPadStatus Input;
};

class InputGrid : public wxGrid
{
	private:
		enum
		{
			COLUMN_INPUT_COUNT = 0,
			COLUMN_VI_COUNT,
			COLUMN_ANA_X,
			COLUMN_ANA_Y,
			COLUMN_A,
			COLUMN_B,
			COLUMN_X,
			COLUMN_Y,
			COLUMN_S,
			COLUMN_Z,
			COLUMN_L,
			COLUMN_R,
			COLUMN_L_ANA,
			COLUMN_R_ANA,
			COLUMN_D_UP,
			COLUMN_D_DOWN,
			COLUMN_D_LEFT,
			COLUMN_D_RIGHT,
			COLUMN_C_X,
			COLUMN_C_Y
		};
		const std::vector<wxString> COLUMN_LABEL = {
			"Input", "VI", "aX", "aY", "A", "B", "X",
			"Y", "S", "Z", "L", "R", "La", "Ra", "dU",
			"dD", "dL", "dR", "cX", "cY"
		};
		std::vector<TAStudioInput> m_inputVector;
		u64 m_firstInputInGrid;
		u64 m_firstFrameInGrid;
		int m_gridNumberOfRows;
		bool m_groupByVI;
		wxDECLARE_EVENT_TABLE();

	public:
		InputGrid(wxWindow* parent);
		void OnSelectCell(wxGridEvent& evt);
		void OnCellChanged(wxGridEvent& evt);
		void OnMouseWheel(wxMouseEvent& evt);
		void UpdateGridValues();
		void AddInputToVector(u64 frameCount, u64 inputCount, GCPadStatus* input, bool groupByVI);
		GCPadStatus GetInputAtRow(u64 inputCount);
		void DeleteInputAtRow(int row);
		void SetInputAtRow(int row, TAStudioInput tastudioInput, u64 inputCount);
		GCPadStatus GetInputAtInputFrame(int inputCount);
		void SetInputAtInputFrame(GCPadStatus* PadStatus, int inputCount);
		void HighlightActiveFrame(u64 frameCount);
		int GetTAStudioInputVectorSize();
};

class TAStudioFrame : public wxDialog
{
	protected:
		wxStaticBoxSizer* m_controlWrapper;
		InputGrid* m_inputGrid;
		wxTextCtrl* m_inputFrameCount;
		wxCheckBox* m_sendInputsToDolphin;
		wxCheckBox* m_groupByVI;

	public:
		TAStudioFrame(wxWindow* parent,
			wxWindowID id = wxID_ANY,
			const wxString& title = _("TAStudio"),
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxDEFAULT_DIALOG_STYLE);
		void GetInput(GCPadStatus* PadStatus);
		void SetInput(GCPadStatus* PadStatus);
		void ParseSavestateInputs(u8* movInput);
	
};