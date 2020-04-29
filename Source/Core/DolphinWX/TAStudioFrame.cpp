// Copyright 2020 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

// Class written by Tales Carvalho (a.k.a. THC98)

#include "TAStudioFrame.h"

wxBEGIN_EVENT_TABLE(InputGrid, wxGrid)
	EVT_GRID_CELL_LEFT_CLICK(InputGrid::OnSelectCell)
	EVT_GRID_CELL_CHANGED(InputGrid::OnCellChanged)
	EVT_MOUSEWHEEL(InputGrid::OnMouseWheel)
wxEND_EVENT_TABLE()

/*
	TODO:
	- Implement a function that updates InputGrid::m_inputVector when a state is loaded.
	- Optimize the functions handling groupByVI access by creating a table that indexes every frameCount to the
		inputCount they're related to, so we don't need to execute searches everytime.
	- Update InputGrid::m_groupByVI when the checkbox TAStudioFrame::m_groupByVI is changed (using event).
	- Dolphin throws an error when the emulator is closed, probably because something should be done at the destructor.
		Find out what that is.
	- Dolphin sometimes crashes when there's too much input being processed (usually when groupByVI isn't checked).
		One possible solution to that is disable the grid's auto update in GetInput (in case the crashes happen because
		of too many GUI updates) and creating a button to do that manually.
	- User functions we should look into:
		- Copy/paste inputs;
		- Make current frame in grid selected (SelectRow isn't a good idea because calling it too many times quickly
			causes performance issues);
		- Go to specific frame;
		- User friendly way to edit analog inputs (something like TAS Input maybe?).

*/

TAStudioFrame::TAStudioFrame(wxWindow* parent, wxWindowID id, const wxString& title,
	const wxPoint& position, const wxSize& size, long style)
	: wxDialog(parent, id, title, position, size, style)
{
	SetSizeHints(wxDefaultSize, wxDefaultSize);

	// build GUI here
	wxFlexGridSizer* fgSizer;
	fgSizer = new wxFlexGridSizer(1, 2, 0, 0);
	fgSizer->SetFlexibleDirection(wxBOTH);
	fgSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_inputGrid = new InputGrid(this);
	wxGridCellAttr *ReadOnlyAttr = new wxGridCellAttr;
	ReadOnlyAttr->SetReadOnly();

	m_inputGrid->SetColAttr(0, ReadOnlyAttr);
	m_inputGrid->SetColAttr(1, ReadOnlyAttr);

	m_controlWrapper = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Buttons"));

	m_inputFrameCount = new wxTextCtrl(this, wxID_ANY);
	m_sendInputsToDolphin = new wxCheckBox(this, wxID_ANY, wxT("Send inputs to Dolphin"));
	m_groupByVI = new wxCheckBox(this, wxID_ANY, wxT("Group by VI counter"));

	m_controlWrapper->Add(m_inputFrameCount);
	m_controlWrapper->AddSpacer(1);
	m_controlWrapper->Add(m_sendInputsToDolphin);
	m_controlWrapper->AddSpacer(1);
	m_controlWrapper->Add(m_groupByVI);

	fgSizer->Add(m_inputGrid);
	fgSizer->Add(m_controlWrapper);
	//this->SetClientSize(900, 800);

	SetSizer(fgSizer);
	Layout();

	fgSizer->Fit(this);
}

void TAStudioFrame::GetInput(GCPadStatus* PadStatus)
{
	m_inputGrid->AddInputToVector(Movie::g_currentFrame, Movie::g_currentInputCount, PadStatus, m_groupByVI->GetValue());
	m_inputGrid->HighlightActiveFrame(Movie::g_currentInputCount);
}

void TAStudioFrame::SetInput(GCPadStatus* PadStatus)
{
	if (!m_sendInputsToDolphin->GetValue()) 
	{ 
		return; 
	}

	// Get input for corresponding inputCount (next in-game input)
	int inputFrame = Movie::g_currentInputCount + 1;

	if (m_inputGrid->GetTAStudioInputVectorSize() <= inputFrame) 
	{
		// Handle case where we've reached the end of the InputGrid table
		// Currently, this will start registering inputs by TASInput/Controller
		return; 
	}

	m_inputGrid->HighlightActiveFrame(inputFrame);
																			 
	*PadStatus = m_inputGrid->GetInputAtInputFrame(inputFrame);
}

InputGrid::InputGrid(wxWindow* parent) : wxGrid(parent, wxID_ANY)
{
	m_firstInputInGrid = 1;
	m_firstFrameInGrid = 1;
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
			case COLUMN_D_UP:
			case COLUMN_D_DOWN:
			case COLUMN_D_LEFT:
			case COLUMN_D_RIGHT:
				SetColSize(i, 30);
				break;
			default:
				SetColSize(i, 20);
				break;
		}
	}
}

int InputGrid::GetTAStudioInputVectorSize()
{
	return m_inputVector.size();
}

GCPadStatus InputGrid::GetInputAtInputFrame(int inputFrame)
{
	return m_inputVector[inputFrame].Input;
}

void InputGrid::OnSelectCell(wxGridEvent& evt)
{
	int row = evt.GetRow();
	int col = evt.GetCol();
	std::string cell = this->GetCellValue(row, col);

	// If input doesn't exist in vector, return
	if (m_groupByVI)
	{
		int currFrameCount = row + m_firstFrameInGrid;
		const auto p = std::find_if(m_inputVector.begin(), m_inputVector.end(),
			[currFrameCount](const TAStudioInput& inp) {return inp.FrameCount == currFrameCount; });
		if (p == m_inputVector.end()) 
		{
			return;
		}
	}
	else
	{
		if (row + m_firstInputInGrid >= m_inputVector.size())
		{
			return;
		}
	}

	switch (col)
	{
		case COLUMN_INPUT_COUNT:
		case COLUMN_VI_COUNT:
			return;
			break;
		case COLUMN_ANA_X:
		case COLUMN_ANA_Y:
		case COLUMN_C_X:
		case COLUMN_C_Y:
		case COLUMN_L_ANA:
		case COLUMN_R_ANA:
			evt.Skip(); // Allow the user to edit the cell
			return;
			break;
		default:
			break;
	}

	if (cell == COLUMN_LABEL[col])
	{
		this->SetCellValue(row, col, "");
	}
	else
	{
		this->SetCellValue(row, col, COLUMN_LABEL[col]);
	}

	// Update the value(s) in the vector
	if (m_groupByVI)
	{
		int currFrameCount = row + m_firstFrameInGrid;
		// Repeat for every input in m_inputVector whose FrameCount == currFrameCount
		std::vector<TAStudioInput>::iterator it = m_inputVector.begin();
		while ((it = std::find_if(it, m_inputVector.end(), [currFrameCount](const TAStudioInput& inp) {
				return inp.FrameCount == currFrameCount; })) != m_inputVector.end()) {
			it->Input = GetInputAtRow(row);
			it++;
		}
	}
	else
	{
		m_inputVector[row + m_firstInputInGrid].Input = GetInputAtRow(row);
	}

	UpdateGridValues(); // This can probably be removed once the program is working properly.
						// This is being executed to make sure that the input written to the
						// current row was correctly stored to the vector.
}

void InputGrid::OnCellChanged(wxGridEvent& evt)
{
	int row = evt.GetRow();

	// Update the value(s) in the vector
	if (m_groupByVI)
	{
		int currFrameCount = row + m_firstFrameInGrid;
		// Repeat for every input in m_inputVector whose FrameCount == currFrameCount
		std::vector<TAStudioInput>::iterator it = m_inputVector.begin();
		while ((it = std::find_if(it, m_inputVector.end(), [currFrameCount](const TAStudioInput& inp) {
			return inp.FrameCount == currFrameCount; })) != m_inputVector.end()) {
			it->Input = GetInputAtRow(row);
			it++;
		}
	}
	else
	{
		m_inputVector[row + m_firstInputInGrid].Input = GetInputAtRow(row);
	}

	UpdateGridValues();
}

void InputGrid::OnMouseWheel(wxMouseEvent& evt)
{
	if (evt.GetWheelRotation() > 0)
	{
		if (m_groupByVI)
		{
			m_firstFrameInGrid -= m_gridNumberOfRows / 4;
			UpdateGridValues();
		}
		else
		{
			m_firstInputInGrid -= m_gridNumberOfRows / 4;
			UpdateGridValues();
		}
	}
	else
	{
		if (m_groupByVI)
		{
			m_firstFrameInGrid += m_gridNumberOfRows / 4;
			UpdateGridValues();
		}
		else
		{
			m_firstInputInGrid += m_gridNumberOfRows / 4;
			UpdateGridValues();
		}
	}
}

void InputGrid::UpdateGridValues()
{
	for (int i = 0; i < m_gridNumberOfRows; i++)
	{
		if (m_groupByVI) // currCount = current frameCount
		{
			u64 currFrameCount = i + m_firstFrameInGrid;
			// Find if given frameCount exists in m_inputVector
			const auto p = std::find_if(m_inputVector.begin(), m_inputVector.end(),
				[currFrameCount](const TAStudioInput& inp) {return inp.FrameCount == currFrameCount; });
			if (p != m_inputVector.end())
			{
				SetInputAtRow(i, *p, p - m_inputVector.begin());
			}
			else
			{
				DeleteInputAtRow(i);
			}
		}
		else // currCount = current inputCount
		{
			u64 currInputCount = i + m_firstInputInGrid;
			if (currInputCount < m_inputVector.size())
			{
				SetInputAtRow(i, m_inputVector[currInputCount], currInputCount);
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
	m_groupByVI = groupByVI;

	// This should NEVER repeat more than once after the load state functions are implemented
	// But I left it here to make sure nothing breaks.
	while (m_inputVector.size() < (inputCount + 1))
	{
		m_inputVector.push_back(TAStudioInput());
	}

	m_inputVector[inputCount].FrameCount = frameCount;
	m_inputVector[inputCount].Input = *input;

	// Insert input in visual grid
	if (m_groupByVI)
	{
		// If a row with current frameCount already exists, just insert the input in that row
		if (frameCount >= m_firstFrameInGrid && frameCount < m_firstFrameInGrid + m_gridNumberOfRows)
		{
			SetInputAtRow(frameCount - m_firstFrameInGrid, m_inputVector[inputCount], inputCount);
		}
		// Else, update the visual grid so the new input is visible
		else
		{
			m_firstFrameInGrid = frameCount - m_gridNumberOfRows / 2;
			if (m_firstFrameInGrid < 1)
			{
				m_firstFrameInGrid = 1;
			}
			UpdateGridValues();
		}
	}
	else
	{
		// If a row with current inputCount already exists, just insert the input in that row
		if (inputCount >= m_firstInputInGrid && inputCount < m_firstInputInGrid + m_gridNumberOfRows)
		{
			SetInputAtRow(inputCount - m_firstInputInGrid, m_inputVector[inputCount], inputCount);
		}
		// Else, update the visual grid so the new input is visible
		else
		{
			m_firstInputInGrid = inputCount - m_gridNumberOfRows / 2;
			if (m_firstInputInGrid < 1)
			{
				m_firstInputInGrid = 1;
			}
			UpdateGridValues();
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

void InputGrid::SetInputAtRow(int row, TAStudioInput tastudioInput, u64 inputCount)
{
	GCPadStatus padStatus = tastudioInput.Input;

	SetCellValue(wxGridCellCoords(row, COLUMN_INPUT_COUNT), std::to_string(inputCount));
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

	PadStatus.button = 0;
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

void InputGrid::HighlightActiveFrame(u64 inputCount)
{
	if (m_groupByVI)
	{
		this->SelectRow(inputCount - m_firstFrameInGrid);
	}
	else
	{
		this->SelectRow(inputCount - m_firstInputInGrid);
	}
}