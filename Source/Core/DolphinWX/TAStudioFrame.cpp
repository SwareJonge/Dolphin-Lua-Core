// Copyright 2020 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

// Class written by Tales Carvalho (a.k.a. THC98)

/*TODO:
- Add framecount text above row label
*/

#include "TAStudioFrame.h"


TAStudioFrame::TAStudioFrame(wxWindow* parent, wxWindowID id, const wxString& title,
	const wxPoint& position, const wxSize& size, long style)
	: wxDialog(parent, id, title, position, size, style)
{
	SetSizeHints(wxDefaultSize, wxDefaultSize);

	// build GUI here
	wxFlexGridSizer* fgSizer;
	fgSizer = new wxFlexGridSizer(0, 2, 0, 0);
	fgSizer->SetFlexibleDirection(wxBOTH);
	fgSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_inputGrid = new InputGrid(this);

	m_controlWrapper = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Buttons"));

	m_inputFrameCount = new wxTextCtrl(this, wxID_ANY);
	m_controlWrapper->Add(m_inputFrameCount);

	fgSizer->Add(m_inputGrid);
	fgSizer->Add(m_controlWrapper);
	this->SetClientSize(800, 800);

	SetSizer(fgSizer);
	Layout();

	fgSizer->Fit(this);
}

void TAStudioFrame::GetInput(GCPadStatus* PadStatus)
{
	m_inputFrameCount->SetValue(std::to_string(Movie::g_currentInputCount));
	//m_currentInput->SetValue(Movie::GetInputDisplay());
	m_inputGrid->SetInputAtFrame(Movie::g_currentInputCount, PadStatus);
}

void TAStudioFrame::SetInput(GCPadStatus* PadStatus)
{

}

InputGrid::InputGrid(wxWindow* parent) : wxGrid(parent, wxID_ANY, wxDefaultPosition, wxSize(600, 800))
{
	int numColumns = COLUMN_LABEL.size();
	CreateGrid(1000, numColumns, wxGridSelectRows);
	for (int i = 0; i < numColumns; i++)
	{
		SetColLabelValue(i, COLUMN_LABEL[i]);
		SetColSize(i, 20);
	}
	//Fit();
}

void InputGrid::SetInputAtFrame(u64 inputCount, GCPadStatus* PadStatus)
{
	// check if inputCount exists in grid

	// if inputCount doesn't exist, create a new row with PadStatus

	//InsertRows(inputCount);

	int row = inputCount;

	SetCellValue(wxGridCellCoords(row, COLUMN_ANA_X), std::to_string(PadStatus->stickX));
	SetCellValue(wxGridCellCoords(row, COLUMN_ANA_Y), std::to_string(PadStatus->stickY));
	SetCellValue(wxGridCellCoords(row, COLUMN_C_X), std::to_string(PadStatus->stickX));
	SetCellValue(wxGridCellCoords(row, COLUMN_C_Y), std::to_string(PadStatus->stickY));
	SetCellValue(wxGridCellCoords(row, COLUMN_L_ANA), std::to_string(PadStatus->triggerLeft));
	SetCellValue(wxGridCellCoords(row, COLUMN_R_ANA), std::to_string(PadStatus->triggerRight));

	SetCellValue(wxGridCellCoords(row, COLUMN_A), PadStatus->button & PAD_BUTTON_A ? COLUMN_LABEL[COLUMN_A] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_B), PadStatus->button & PAD_BUTTON_B ? COLUMN_LABEL[COLUMN_B] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_X), PadStatus->button & PAD_BUTTON_X ? COLUMN_LABEL[COLUMN_X] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_Y), PadStatus->button & PAD_BUTTON_Y ? COLUMN_LABEL[COLUMN_Y] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_S), PadStatus->button & PAD_BUTTON_START ? COLUMN_LABEL[COLUMN_S] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_Z), PadStatus->button & PAD_TRIGGER_Z ? COLUMN_LABEL[COLUMN_Z] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_L), PadStatus->button & PAD_TRIGGER_L ? COLUMN_LABEL[COLUMN_L] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_R), PadStatus->button & PAD_TRIGGER_R ? COLUMN_LABEL[COLUMN_R] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_D_UP), PadStatus->button & PAD_BUTTON_UP ? COLUMN_LABEL[COLUMN_D_UP] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_D_DOWN), PadStatus->button & PAD_BUTTON_DOWN ? COLUMN_LABEL[COLUMN_D_DOWN] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_D_LEFT), PadStatus->button & PAD_BUTTON_LEFT ? COLUMN_LABEL[COLUMN_D_LEFT] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_D_RIGHT), PadStatus->button & PAD_BUTTON_RIGHT ? COLUMN_LABEL[COLUMN_D_RIGHT] : "");

	//SelectRow(row);
	MakeCellVisible(row, COLUMN_A);

	//SetCellValue(wxGridCellCoords(row, col), value);
}

/*GCPadStatus InputGrid::GetInputAtFrame(u64 inputCount)
{
	// return a GCPadStatus with the content of row inputCount
}*/