// Copyright 2020 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

// Class written by Tales Carvalho (a.k.a. THC98)

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

	m_inputFrameCount = new wxTextCtrl(this, wxID_ANY);
	fgSizer->Add(m_inputFrameCount);

	m_currentInput = new wxTextCtrl(this, wxID_ANY);
	fgSizer->Add(m_currentInput);

	SetSizer(fgSizer);
	Layout();

	fgSizer->Fit(this);
}

void TAStudioFrame::GetInput(GCPadStatus* PadStatus)
{
	m_inputFrameCount->SetValue(std::to_string(Movie::g_currentInputCount));
	m_currentInput->SetValue(Movie::GetInputDisplay());
}

void TAStudioFrame::SetInput(GCPadStatus* PadStatus)
{

}