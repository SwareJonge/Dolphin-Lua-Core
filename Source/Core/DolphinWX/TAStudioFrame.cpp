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

	m_inputGrid->AddInputToVector(Movie::g_currentFrame, Movie::g_currentInputCount, PadStatus);

	//m_inputGrid->UpdateGridValues();
	
	//m_inputGrid->SetInputAtRow(Movie::g_currentInputCount, PadStatus);
}

void TAStudioFrame::SetInput(GCPadStatus* PadStatus)
{

}

InputGrid::InputGrid(wxWindow* parent) : wxGrid(parent, wxID_ANY, wxDefaultPosition, wxSize(600, 800))
{
	m_firstInputInGrid = 1;
	m_gridNumberOfRows = 30;

	int numColumns = COLUMN_LABEL.size();
	CreateGrid(m_gridNumberOfRows, numColumns, wxGridSelectRows);
	for (int i = 0; i < numColumns; i++)
	{
		SetColLabelValue(i, COLUMN_LABEL[i]);
		switch (i)
		{
			case COLUMN_ANA_X:
			case COLUMN_ANA_Y:
			case COLUMN_C_X:
			case COLUMN_C_Y:
			case COLUMN_L_ANA:
			case COLUMN_R_ANA:
				SetColSize(i, 40);
				break;
			default:
				SetColSize(i, 20);
				break;
		}
	}
	//Fit();
}

void InputGrid::UpdateGridValues()
{
	for (int i = 0; i < m_gridNumberOfRows; i++)
	{
		u64 currInputCount = i + m_firstInputInGrid;
		const auto p = std::find_if(m_inputVector.begin(), m_inputVector.end(),
			[currInputCount](const TAStudioInput& inp) {return inp.InputCount == currInputCount; });
		if (p != m_inputVector.end())
		{
			SetInputAtRow(i, &p->Input);
		}
		else
		{
			DeleteInputAtRow(i);
		}
	}
}

void InputGrid::AddInputToVector(u64 frameCount, u64 inputCount, GCPadStatus* input)
{
	TAStudioInput inp;
	inp.FrameCount = frameCount;
	inp.InputCount = inputCount;
	inp.Input = *input;
	m_inputVector.push_back(inp);

	if (inputCount > m_firstInputInGrid + m_gridNumberOfRows - 1)
	{
		m_firstInputInGrid += 15;
		for (int i = 0; i < m_gridNumberOfRows; i++)
			SetRowLabelValue(i, std::to_string(i + m_firstInputInGrid));
		UpdateGridValues();
	}
	else
	{
		SetInputAtRow(inputCount - m_firstInputInGrid, input);
	}
}

void InputGrid::DeleteInputAtRow(int row)
{
	for (int i = 0; i < COLUMN_LABEL.size(); i++)
	{
		SetCellValue(wxGridCellCoords(row, i), "");
	}
}

void InputGrid::SetInputAtRow(int row, GCPadStatus* PadStatus)
{
	// check if inputCount exists in grid

	// if inputCount doesn't exist, create a new row with PadStatus

	//InsertRows(inputCount);

	SetCellValue(wxGridCellCoords(row, COLUMN_ANA_X), std::to_string(PadStatus->stickX));
	SetCellValue(wxGridCellCoords(row, COLUMN_ANA_Y), std::to_string(PadStatus->stickY));
	SetCellValue(wxGridCellCoords(row, COLUMN_C_X), std::to_string(PadStatus->substickX));
	SetCellValue(wxGridCellCoords(row, COLUMN_C_Y), std::to_string(PadStatus->substickY));
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