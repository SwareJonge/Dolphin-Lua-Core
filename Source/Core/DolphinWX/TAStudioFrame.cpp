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

	wxGrid* grid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxSize(400, 300));
	const int NUM_COLUMNS = 19;
	grid->CreateGrid(1, NUM_COLUMNS);
	
	// Set column names
	char colNames[NUM_COLUMNS][10] = { "Save", "aX", "aY", "A", "B", "X", "Y", "S", "Z", "L", "R", "La", "Ra", "dU", "dD", "dL", "dR", "cX", "cY" };
	for (int i = 0; i < NUM_COLUMNS; i++)
	{
		grid->SetColLabelValue(i, wxString(colNames[i]));
		grid->SetColSize(i, 20);
	}
	grid->Fit();
	this->SetClientSize(800, 400);

	SetSizer(fgSizer);
	Layout();

	fgSizer->Fit(this);
}

void TAStudioFrame::GetInput(GCPadStatus* PadStatus)
{
	m_inputFrameCount->SetValue(std::to_string(Movie::g_currentInputCount));
	//m_currentInput->SetValue(Movie::GetInputDisplay());
}

void TAStudioFrame::SetInput(GCPadStatus* PadStatus)
{

}