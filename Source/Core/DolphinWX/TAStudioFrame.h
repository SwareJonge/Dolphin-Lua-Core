// Copyright 2020 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

// Class written by Tales Carvalho (a.k.a. THC98) and Sean Owens (a.k.a. Malleo)

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
#include <wx/slider.h>
#include <wx/button.h>

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
		std::vector< std::vector<u64> > m_viToInputCount;
		u64 m_firstInputInGrid;
		u64 m_firstFrameInGrid;
		int m_gridNumberOfRows;
		bool m_groupByVI;
		bool m_autoUpdateGrid;
		wxDECLARE_EVENT_TABLE();

	public:
		InputGrid(wxWindow* parent);
		void SetGroupByVI(bool value);
		void SetAutoUpdateGrid(bool value);
		void OnSelectCell(wxGridEvent& evt);
		void OnCellChanged(wxGridEvent& evt);
		void OnMouseWheel(wxMouseEvent& evt);
		void GoToCurrentFrame();
		void ParseStateInputs();
		void UpdateGridValues();
		void AddInputToVector(u64 frameCount, u64 inputCount, GCPadStatus* input);
		GCPadStatus GetInputAtRow(u64 inputCount);
		void DeleteInputAtRow(int row);
		void SetInputAtRow(int row, TAStudioInput tastudioInput, u64 inputCount);
		void SetCellValueIfChanged(int row, int col, const wxString str);
		GCPadStatus GetInputAtInputFrame(int inputCount);
		void SetInputAtInputFrame(GCPadStatus* PadStatus, int inputCount);
		void HighlightActiveFrame(u64 frameCount);
		int GetTAStudioInputVectorSize();
};

class TAStudioFrame : public wxDialog
{
	protected:
		wxBoxSizer* m_sideWrapper;
		wxStaticBoxSizer* m_controlWrapper;
		wxStaticBoxSizer* m_analogWrapper;
		InputGrid* m_inputGrid;
		wxTextCtrl* m_inputFrameCount;
		wxCheckBox* m_sendInputsToDolphin;
		wxCheckBox* m_groupByVI;
		wxCheckBox* m_autoUpdateGrid;
		wxButton* m_updateGrid;
		wxButton* m_goToCurrentFrame;
		wxSlider* m_stickXSlider;
		wxSlider* m_stickYSlider;

		void OnGroupByVIChanged(wxCommandEvent&);
		void OnAutoUpdateGridChanged(wxCommandEvent&);
		void OnUpdateGridClick(wxCommandEvent&);
		void OnGoToCurrentFrameClick(wxCommandEvent&);

	public:
		TAStudioFrame(wxWindow* parent,
			wxWindowID id = wxID_ANY,
			const wxString& title = _("TAStudio"),
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxDEFAULT_DIALOG_STYLE);
		void GetInput(GCPadStatus* PadStatus);
		void SetInput(GCPadStatus* PadStatus);
		void OnLoadstateCallback();
	
};