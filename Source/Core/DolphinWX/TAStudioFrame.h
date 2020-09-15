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
#include <wx/textdlg.h>
#include <wx/button.h>

#include "Core/Movie.h"

#include "InputCommon/GCPadStatus.h"

struct TAStudioInput
{
	u64 FrameCount;
	GCPadStatus Input;
};

class InputGrid : public wxGrid
{
private:
	enum
	{
		COLUMN_INPUT_COUNT = 0,
		COLUMN_VI_COUNT,
		COLUMN_ACTIVE,
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
	std::vector<wxString> COLUMN_LABEL = {
		"Input", "VI", "Diff", "aX", "aY", "A",
		"B", "X", "Y", "S", "Z", "L", "R", "La",
		"Ra", "dU", "dD", "dL", "dR", "cX", "cY"
	};
	std::vector<TAStudioInput> m_inputVector;
	std::vector<GCPadStatus> m_vectorClipboard;
	std::vector< std::vector<u64> > m_viToInputCount;
	std::vector<int> m_selectedRows;
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
	void SetMainStickInSelectedRows(u8 x, u8 y);
	void SetCStickInSelectedRows(u8 x, u8 y);
	void ToggleCellValue(int row, int col);
	void SetCellInput(int row, int col, bool value);
	void OnSelectCell(wxGridEvent& evt);
	void OnCellChanged(wxGridEvent& evt);
	void OnMouseWheel(wxMouseEvent& evt);
	void OnRangeSelect(wxGridRangeSelectEvent& evt);
	void OnCellRightClick(wxGridEvent& evt);
	void GoToCurrentFrame();
	void GoToInputFrame(int frame);
	void GoToVI(int vi);
	void ParseStateInputs();
	void UpdateGridValues();
	void AddInputToVector(u64 frameCount, u64 inputCount, GCPadStatus* input);
	GCPadStatus GetInputAtRow(u64 inputCount);
	GCPadStatus GetInputAtSelectedRow();
	GCPadStatus* BlankInput();
	void DeleteInputAtRow(int row);
	void SetInputAtRow(int row, TAStudioInput tastudioInput, u64 inputCount);
	void InsertBlankFrame(bool below);
	void InsertInputsAtInputCount(u64 inputCount, GCPadStatus* input, int size);
	void SetSelectedInputsPress(bool toggle, bool value = false);
	void CopySelectedFrames();
	void PasteFrames(bool below);
	GCPadStatus GetInputAtInputFrame(int inputCount);
	void SetInputAtInputFrame(GCPadStatus* PadStatus, int inputCount);
	void HighlightActiveFrame(u64 frameCount);
	int GetTAStudioInputVectorSize();
};

class TAStudioFrame : public wxDialog
{
protected:
	struct Control
	{
		wxTextCtrl* text;
		wxSlider* slider;
		int value = -1;
		int text_id;
		int slider_id;
		u32 range;
		u32 default_value = 128;
		bool set_by_keyboard = false;
		bool reverse = false;
	};

	struct Stick
	{
		wxStaticBitmap* bitmap;
		Control x_cont;
		Control y_cont;
	};

	InputGrid* m_inputGrid;

	wxCheckBox* m_sendInputsToDolphin;
	wxCheckBox* m_groupByVI;
	wxCheckBox* m_autoUpdateGrid;

	const int ID_C_STICK = 1001;
	const int ID_MAIN_STICK = 1002;
	int m_eleID = 1003;

	Stick m_main_stick, m_c_stick;
	Control* m_stick_controls[4];

	void OnGroupByVIChanged(wxCommandEvent&);
	void OnAutoUpdateGridChanged(wxCommandEvent&);
	void OnUpdateGridClick(wxCommandEvent&);
	void OnGoToCurrentFrameClick(wxCommandEvent&);
	void OnGoToInputFrameClick(wxCommandEvent&);
	void OnGoToVIClick(wxCommandEvent&);
	void OnInsertFrameAboveClick(wxCommandEvent&);
	void OnInsertFrameBelowClick(wxCommandEvent&);
	void OnToggleSelectedInputsClick(wxCommandEvent&);
	void OnSetSelectedInputsPressedClick(wxCommandEvent&);
	void OnSetSelectedInputsNotPressedClick(wxCommandEvent&);
	void OnCopyFrameClick(wxCommandEvent&);
	void OnPasteFramesAboveClick(wxCommandEvent&);
	void OnPasteFramesBelowClick(wxCommandEvent&);
	void OnGetMainStickClick(wxCommandEvent&);
	void OnSetMainStickClick(wxCommandEvent&);
	void OnGetCStickClick(wxCommandEvent&);
	void OnSetCStickClick(wxCommandEvent&);

	wxBitmap CreateStickBitmap(int x, int y);
	Control CreateControl(long style, int width, int height, bool reverse = false, u32 range = 255, u32 default_value = 128);
	Stick CreateStick(int id_stick, int xRange, int yRange, u32 defaultX, u32 defaultY, bool reverseX, bool reverseY);
	wxStaticBoxSizer* CreateStickLayout(Stick* tempStick, const wxString& title);

	void UpdateFromSliders(wxCommandEvent& event);
	void UpdateFromText(wxCommandEvent& event);
	void OnMouseDownL(wxMouseEvent& event);
	void OnMouseUpR(wxMouseEvent& event);
	void OnRightClickSlider(wxMouseEvent& event);

public:
	TAStudioFrame(wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& title = _("TAStudio"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_DIALOG_STYLE);
	void GetInput(GCPadStatus* PadStatus);
	void SetInput(GCPadStatus* PadStatus);
	void UpdateGrid();
	void OnLoadstateCallback();
};