// Copyright 2020 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

// Class written by Tales Carvalho (a.k.a. THC98)

#include "TAStudioFrame.h"

wxBEGIN_EVENT_TABLE(InputGrid, wxGrid)
	EVT_GRID_CELL_LEFT_CLICK(InputGrid::OnSelectCell)
wxEND_EVENT_TABLE()

/*
	TODO:
	- SetInput was implemented with the assumption that we can index the TAStudioInput vector based on input count. Thus, we need to make sure that on savestate load,
		the vector is updated with all of the savestate's previous inputs up to the frame of the savestate
*/

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
	m_sendInputsToDolphin = new wxCheckBox(this, wxID_ANY, wxT("Activate TAStudio"));
	m_groupByVI = new wxCheckBox(this, wxID_ANY, wxT("Group by VI counter"));

	m_controlWrapper->Add(m_inputFrameCount);
	m_controlWrapper->AddSpacer(1);
	m_controlWrapper->Add(m_sendInputsToDolphin);
	m_controlWrapper->AddSpacer(1);
	m_controlWrapper->Add(m_groupByVI);

	fgSizer->Add(m_inputGrid);
	fgSizer->Add(m_controlWrapper);
	this->SetClientSize(900, 800);

	SetSizer(fgSizer);
	Layout();

	fgSizer->Fit(this);
}

void TAStudioFrame::GetInput(GCPadStatus* PadStatus)
{
	if (m_sendInputsToDolphin->IsChecked()) { return; }
	m_inputFrameCount->SetValue(std::to_string(Movie::g_currentInputCount));

	m_inputGrid->AddInputToVector(Movie::g_currentFrame, Movie::g_currentInputCount, PadStatus, m_groupByVI->GetValue());
}

void TAStudioFrame::SetInput(GCPadStatus* PadStatus)
{
	if (!m_sendInputsToDolphin->IsChecked()) { return; }

	int inputFrame = Movie::g_currentInputCount;

	// Get input for corresponding inputCount
	*PadStatus = m_inputGrid->GetInputAtInputFrame(inputFrame);

}

//InputGrid::InputGrid(wxWindow* parent) : wxGrid(parent, wxID_ANY, wxDefaultPosition, wxSize(700, 800))
InputGrid::InputGrid(wxWindow* parent) : wxGrid(parent, wxID_ANY)
{
	m_firstInputInGrid = 1;
	m_gridNumberOfRows = 30;

	int numColumns = COLUMN_LABEL.size();
	CreateGrid(m_gridNumberOfRows, numColumns, wxGridSelectRows);
	HideRowLabels();
	for (int i = 0; i < numColumns; i++)
	{
		SetColLabelValue(i, COLUMN_LABEL[i]);
		switch (i)
		{
			case COLUMN_INPUT_COUNT:
			case COLUMN_VI_COUNT:
				SetColSize(i, 60);
				break;
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

GCPadStatus InputGrid::GetInputAtInputFrame(int inputFrame)
{
	if (m_inputVector.size() >= inputFrame) { return; } // Handle case where we've reached the end of the InputGrid table
														// Currently, this will start registering inputs by TASInput/Controller
	return m_inputVector[inputFrame - 1].Input;
}

void InputGrid::OnSelectCell(wxGridEvent& evt)
{
	int row = evt.GetRow();
	int col = evt.GetCol();
	wxMessageBox("OnSelectCell: " + std::to_string(row) + ", " + std::to_string(col));
}

void InputGrid::UpdateGridValues(bool groupByVI)
{
	for (int i = 0; i < m_gridNumberOfRows; i++)
	{
		u64 currCount = i + m_firstInputInGrid;
		if (groupByVI)
		{
			const auto p = std::find_if(m_inputVector.begin(), m_inputVector.end(),
				[currCount](const TAStudioInput& inp) {return inp.FrameCount == currCount; });
			if (p != m_inputVector.end())
			{
				SetInputAtRow(i, *p);
			}
			else
			{
				DeleteInputAtRow(i);
			}
		}
		else
		{
			const auto p = std::find_if(m_inputVector.begin(), m_inputVector.end(),
				[currCount](const TAStudioInput& inp) {return inp.InputCount == currCount; });
			if (p != m_inputVector.end())
			{
				SetInputAtRow(i, *p);
			}
			else
			{
				DeleteInputAtRow(i);
			}
		}
	}
}

void InputGrid::AddInputToVector(u64 frameCount, u64 inputCount, GCPadStatus* input, bool groupByVI)
{
	// remove element in vector if input already exists
	//m_inputVector.erase(std::remove_if(m_inputVector.begin(), m_inputVector.end(),
	//	[inputCount](const TAStudioInput& inp) {return inp.InputCount == inputCount; }));

	TAStudioInput inp;
	inp.FrameCount = frameCount;
	inp.InputCount = inputCount;
	inp.Input = *input;
	m_inputVector.push_back(inp);

	// insert input in visual grid
	if (groupByVI)
	{
		// if row already exists, just insert the input in that row
		if (frameCount >= m_firstInputInGrid && frameCount < m_firstInputInGrid + m_gridNumberOfRows)
		{
			SetInputAtRow(frameCount - m_firstInputInGrid, inp);
		}
		// else, update the visual grid so the new input is visible
		else
		{
			m_firstInputInGrid = frameCount - m_gridNumberOfRows / 2;
			if (m_firstInputInGrid < 1)
			{
				m_firstInputInGrid = 1;
			}
			UpdateGridValues(groupByVI);
		}
	}
	else
	{
		// if row already exists, just insert the input in that row
		if (inputCount >= m_firstInputInGrid && inputCount < m_firstInputInGrid + m_gridNumberOfRows)
		{
			SetInputAtRow(inputCount - m_firstInputInGrid, inp);
		}
		// else, update the visual grid so the new input is visible
		else
		{
			m_firstInputInGrid = inputCount - m_gridNumberOfRows / 2;
			if (m_firstInputInGrid < 1)
			{
				m_firstInputInGrid = 1;
			}
			UpdateGridValues(groupByVI);
		}
	}
}

void InputGrid::DeleteInputAtRow(int row)
{
	for (int i = 0; i < COLUMN_LABEL.size(); i++)
	{
		SetCellValue(wxGridCellCoords(row, i), "");
	}
}

void InputGrid::SetInputAtRow(int row, TAStudioInput tastudioInput)
{
	GCPadStatus padStatus = tastudioInput.Input;

	SetCellValue(wxGridCellCoords(row, COLUMN_INPUT_COUNT), std::to_string(tastudioInput.InputCount));
	SetCellValue(wxGridCellCoords(row, COLUMN_VI_COUNT), std::to_string(tastudioInput.FrameCount));

	SetCellValue(wxGridCellCoords(row, COLUMN_ANA_X), std::to_string(padStatus.stickX));
	SetCellValue(wxGridCellCoords(row, COLUMN_ANA_Y), std::to_string(padStatus.stickY));
	SetCellValue(wxGridCellCoords(row, COLUMN_C_X), std::to_string(padStatus.substickX));
	SetCellValue(wxGridCellCoords(row, COLUMN_C_Y), std::to_string(padStatus.substickY));
	SetCellValue(wxGridCellCoords(row, COLUMN_L_ANA), std::to_string(padStatus.triggerLeft));
	SetCellValue(wxGridCellCoords(row, COLUMN_R_ANA), std::to_string(padStatus.triggerRight));

	SetCellValue(wxGridCellCoords(row, COLUMN_A), padStatus.button & PAD_BUTTON_A ? COLUMN_LABEL[COLUMN_A] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_B), padStatus.button & PAD_BUTTON_B ? COLUMN_LABEL[COLUMN_B] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_X), padStatus.button & PAD_BUTTON_X ? COLUMN_LABEL[COLUMN_X] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_Y), padStatus.button & PAD_BUTTON_Y ? COLUMN_LABEL[COLUMN_Y] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_S), padStatus.button & PAD_BUTTON_START ? COLUMN_LABEL[COLUMN_S] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_Z), padStatus.button & PAD_TRIGGER_Z ? COLUMN_LABEL[COLUMN_Z] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_L), padStatus.button & PAD_TRIGGER_L ? COLUMN_LABEL[COLUMN_L] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_R), padStatus.button & PAD_TRIGGER_R ? COLUMN_LABEL[COLUMN_R] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_D_UP), padStatus.button & PAD_BUTTON_UP ? COLUMN_LABEL[COLUMN_D_UP] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_D_DOWN), padStatus.button & PAD_BUTTON_DOWN ? COLUMN_LABEL[COLUMN_D_DOWN] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_D_LEFT), padStatus.button & PAD_BUTTON_LEFT ? COLUMN_LABEL[COLUMN_D_LEFT] : "");
	SetCellValue(wxGridCellCoords(row, COLUMN_D_RIGHT), padStatus.button & PAD_BUTTON_RIGHT ? COLUMN_LABEL[COLUMN_D_RIGHT] : "");

}

GCPadStatus InputGrid::GetInputAtRow(u64 row)
{
	GCPadStatus PadStatus;

	// Use wxAtoi to convert wxString to integer
	PadStatus.stickX = wxAtoi(GetCellValue(wxGridCellCoords(row, COLUMN_ANA_X)));
	PadStatus.stickY = wxAtoi(GetCellValue(wxGridCellCoords(row, COLUMN_ANA_Y)));
	PadStatus.substickX = wxAtoi(GetCellValue(wxGridCellCoords(row, COLUMN_C_X)));
	PadStatus.substickY = wxAtoi(GetCellValue(wxGridCellCoords(row, COLUMN_C_Y)));
	PadStatus.triggerLeft = wxAtoi(GetCellValue(wxGridCellCoords(row, COLUMN_L_ANA)));
	PadStatus.triggerRight = wxAtoi(GetCellValue(wxGridCellCoords(row, COLUMN_R_ANA)));

	PadStatus.button |= GetCellValue(wxGridCellCoords(row, COLUMN_A)) == COLUMN_LABEL[COLUMN_A] ? PAD_BUTTON_A : 0;
	PadStatus.button |= GetCellValue(wxGridCellCoords(row, COLUMN_B)) == COLUMN_LABEL[COLUMN_B] ? PAD_BUTTON_B : 0;
	PadStatus.button |= GetCellValue(wxGridCellCoords(row, COLUMN_X)) == COLUMN_LABEL[COLUMN_X] ? PAD_BUTTON_X : 0;
	PadStatus.button |= GetCellValue(wxGridCellCoords(row, COLUMN_Y)) == COLUMN_LABEL[COLUMN_Y] ? PAD_BUTTON_Y : 0;
	PadStatus.button |= GetCellValue(wxGridCellCoords(row, COLUMN_S)) == COLUMN_LABEL[COLUMN_S] ? PAD_BUTTON_START : 0;
	PadStatus.button |= GetCellValue(wxGridCellCoords(row, COLUMN_Z)) == COLUMN_LABEL[COLUMN_Z] ? PAD_TRIGGER_Z : 0;
	PadStatus.button |= GetCellValue(wxGridCellCoords(row, COLUMN_L)) == COLUMN_LABEL[COLUMN_L] ? PAD_TRIGGER_L : 0;
	PadStatus.button |= GetCellValue(wxGridCellCoords(row, COLUMN_R)) == COLUMN_LABEL[COLUMN_R] ? PAD_TRIGGER_R : 0;
	PadStatus.button |= GetCellValue(wxGridCellCoords(row, COLUMN_D_UP)) == COLUMN_LABEL[COLUMN_D_UP] ? PAD_BUTTON_UP : 0;
	PadStatus.button |= GetCellValue(wxGridCellCoords(row, COLUMN_D_DOWN)) == COLUMN_LABEL[COLUMN_D_DOWN] ? PAD_BUTTON_DOWN : 0;
	PadStatus.button |= GetCellValue(wxGridCellCoords(row, COLUMN_D_LEFT)) == COLUMN_LABEL[COLUMN_D_LEFT] ? PAD_BUTTON_LEFT : 0;
	PadStatus.button |= GetCellValue(wxGridCellCoords(row, COLUMN_D_RIGHT)) == COLUMN_LABEL[COLUMN_D_RIGHT] ? PAD_BUTTON_RIGHT : 0;

	return PadStatus;
}