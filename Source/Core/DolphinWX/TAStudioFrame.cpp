// Copyright 2020 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

// Class written by Tales Carvalho (a.k.a. THC98) and Sean Owens (a.k.a. Malleo)

#include "TAStudioFrame.h"

wxBEGIN_EVENT_TABLE(InputGrid, wxGrid)
EVT_GRID_CELL_CHANGED(InputGrid::OnCellChanged)
EVT_MOUSEWHEEL(InputGrid::OnMouseWheel)
EVT_GRID_CELL_RIGHT_CLICK(InputGrid::OnCellRightClick)
wxEND_EVENT_TABLE()

TAStudioFrame::TAStudioFrame(wxWindow* parent, wxWindowID id, const wxString& title,
	const wxPoint& position, const wxSize& size, long style)
	: wxDialog(parent, id, title, position, size, style)
{
	SetSizeHints(wxDefaultSize, wxDefaultSize);

	// build GUI here
	wxBoxSizer* boxSizer;
	boxSizer = new wxBoxSizer(wxHORIZONTAL);

	m_inputGrid = new InputGrid(this);

	wxBoxSizer* sideWrapper = new wxBoxSizer(wxVERTICAL);
	wxStaticBoxSizer* controlWrapper = new wxStaticBoxSizer(wxVERTICAL, this, _("Controls"));

	m_sendInputsToDolphin = new wxCheckBox(this, wxID_ANY, _("Read + Write Mode"));
	m_groupByVI = new wxCheckBox(this, wxID_ANY, _("Group by VI counter"));
	m_autoUpdateGrid = new wxCheckBox(this, wxID_ANY, _("Disable auto update grid"));

	wxButton* updateGrid = new wxButton(this, wxID_ANY, _("Update grid"), wxDefaultPosition, wxSize(200, 20));
	wxButton* goToCurrentFrame = new wxButton(this, wxID_ANY, _("Go to current frame"), wxDefaultPosition, wxSize(200, 20));
	wxButton* goToInputFrame = new wxButton(this, wxID_ANY, _("Go to input frame..."), wxDefaultPosition, wxSize(200, 20));
	wxButton* goToVI = new wxButton(this, wxID_ANY, _("Go to VI..."), wxDefaultPosition, wxSize(200, 20));
	wxButton* insertFrameAbove = new wxButton(this, wxID_ANY, _("Insert blank frame above selection"), wxDefaultPosition, wxSize(200, 20));
	wxButton* insertFrameBelow = new wxButton(this, wxID_ANY, _("Insert blank frame below selection"), wxDefaultPosition, wxSize(200, 20));
	wxButton* toggleSelectedInputs = new wxButton(this, wxID_ANY, _("Toggle selected inputs"), wxDefaultPosition, wxSize(200, 20));
	wxButton* setSelectedInputsPressed = new wxButton(this, wxID_ANY, _("Set selected inputs as pressed"), wxDefaultPosition, wxSize(200, 20));
	wxButton* setSelectedInputsNotPressed = new wxButton(this, wxID_ANY, _("Set selected inputs as not pressed"), wxDefaultPosition, wxSize(200, 20));
	wxButton* copyFrame = new wxButton(this, wxID_ANY, _("Copy selected frames"), wxDefaultPosition, wxSize(200, 20));
	wxButton* pasteFramesAbove = new wxButton(this, wxID_ANY, _("Paste frames above selection"), wxDefaultPosition, wxSize(200, 20));
	wxButton* pasteFramesBelow = new wxButton(this, wxID_ANY, _("Paste frames below selection"), wxDefaultPosition, wxSize(200, 20));

	m_main_stick = CreateStick(ID_MAIN_STICK, 255, 255, 128, 128, false, true);
	wxStaticBoxSizer* const main_box = CreateStickLayout(&m_main_stick, _("Main Stick"));

	m_c_stick = CreateStick(ID_C_STICK, 255, 255, 128, 128, false, true);
	wxStaticBoxSizer* const c_box = CreateStickLayout(&m_c_stick, _("C Stick"));

	for (unsigned int i = 0; i < 4; ++i)
	{
		m_stick_controls[i] = nullptr;
	}

	m_stick_controls[0] = &m_main_stick.x_cont;
	m_stick_controls[1] = &m_main_stick.y_cont;
	m_stick_controls[2] = &m_c_stick.x_cont;
	m_stick_controls[3] = &m_c_stick.y_cont;

	for (int i = 0; i < 4; i++)
	{
		m_stick_controls[i]->slider->Bind(wxEVT_RIGHT_UP, &TAStudioFrame::OnRightClickSlider, this);
	}

	wxButton* getMainStick = new wxButton(this, wxID_ANY, _("Get"));
	wxButton* setMainStick = new wxButton(this, wxID_ANY, _("Set"));

	wxBoxSizer* mainStickButtonsWrapper = new wxBoxSizer(wxHORIZONTAL);

	mainStickButtonsWrapper->Add(getMainStick);
	mainStickButtonsWrapper->Add(setMainStick);

	main_box->Add(mainStickButtonsWrapper);

	wxButton* getCStick = new wxButton(this, wxID_ANY, _("Get"));
	wxButton* setCStick = new wxButton(this, wxID_ANY, _("Set"));

	wxBoxSizer* cStickButtonsWrapper = new wxBoxSizer(wxHORIZONTAL);

	cStickButtonsWrapper->Add(getCStick);
	cStickButtonsWrapper->Add(setCStick);

	c_box->Add(cStickButtonsWrapper);

	m_groupByVI->Bind(wxEVT_CHECKBOX, &TAStudioFrame::OnGroupByVIChanged, this);
	m_autoUpdateGrid->Bind(wxEVT_CHECKBOX, &TAStudioFrame::OnAutoUpdateGridChanged, this);
	updateGrid->Bind(wxEVT_BUTTON, &TAStudioFrame::OnUpdateGridClick, this);
	goToCurrentFrame->Bind(wxEVT_BUTTON, &TAStudioFrame::OnGoToCurrentFrameClick, this);
	goToInputFrame->Bind(wxEVT_BUTTON, &TAStudioFrame::OnGoToInputFrameClick, this);
	goToVI->Bind(wxEVT_BUTTON, &TAStudioFrame::OnGoToVIClick, this);
	insertFrameAbove->Bind(wxEVT_BUTTON, &TAStudioFrame::OnInsertFrameAboveClick, this);
	insertFrameBelow->Bind(wxEVT_BUTTON, &TAStudioFrame::OnInsertFrameBelowClick, this);
	toggleSelectedInputs->Bind(wxEVT_BUTTON, &TAStudioFrame::OnToggleSelectedInputsClick, this);
	setSelectedInputsPressed->Bind(wxEVT_BUTTON, &TAStudioFrame::OnSetSelectedInputsPressedClick, this);
	setSelectedInputsNotPressed->Bind(wxEVT_BUTTON, &TAStudioFrame::OnSetSelectedInputsNotPressedClick, this);
	copyFrame->Bind(wxEVT_BUTTON, &TAStudioFrame::OnCopyFrameClick, this);
	pasteFramesAbove->Bind(wxEVT_BUTTON, &TAStudioFrame::OnPasteFramesAboveClick, this);
	pasteFramesBelow->Bind(wxEVT_BUTTON, &TAStudioFrame::OnPasteFramesBelowClick, this);
	getMainStick->Bind(wxEVT_BUTTON, &TAStudioFrame::OnGetMainStickClick, this);
	setMainStick->Bind(wxEVT_BUTTON, &TAStudioFrame::OnSetMainStickClick, this);
	getCStick->Bind(wxEVT_BUTTON, &TAStudioFrame::OnGetCStickClick, this);
	setCStick->Bind(wxEVT_BUTTON, &TAStudioFrame::OnSetCStickClick, this);

	controlWrapper->Add(m_sendInputsToDolphin);
	controlWrapper->AddSpacer(1);
	controlWrapper->Add(m_groupByVI);
	controlWrapper->AddSpacer(1);
	controlWrapper->Add(m_autoUpdateGrid);
	controlWrapper->AddSpacer(1);
	controlWrapper->Add(updateGrid);
	controlWrapper->AddSpacer(1);
	controlWrapper->Add(goToCurrentFrame);
	controlWrapper->AddSpacer(1);
	controlWrapper->Add(goToInputFrame);
	controlWrapper->AddSpacer(1);
	controlWrapper->Add(goToVI);
	controlWrapper->AddSpacer(1);
	controlWrapper->Add(insertFrameAbove);
	controlWrapper->AddSpacer(1);
	controlWrapper->Add(insertFrameBelow);
	controlWrapper->AddSpacer(1);
	controlWrapper->Add(toggleSelectedInputs);
	controlWrapper->AddSpacer(1);
	controlWrapper->Add(setSelectedInputsPressed);
	controlWrapper->AddSpacer(1);
	controlWrapper->Add(setSelectedInputsNotPressed);
	controlWrapper->AddSpacer(1);
	controlWrapper->Add(copyFrame);
	controlWrapper->AddSpacer(1);
	controlWrapper->Add(pasteFramesAbove);
	controlWrapper->AddSpacer(1);
	controlWrapper->Add(pasteFramesBelow);

	sideWrapper->Add(controlWrapper);
	sideWrapper->AddSpacer(1);
	sideWrapper->Add(main_box);
	sideWrapper->AddSpacer(1);
	sideWrapper->Add(c_box);

	boxSizer->Add(m_inputGrid);
	boxSizer->AddSpacer(5);
	boxSizer->Add(sideWrapper);

	SetSizer(boxSizer);
	Layout();

	boxSizer->Fit(this);
}

void TAStudioFrame::GetInput(GCPadStatus* PadStatus)
{
	m_inputGrid->AddInputToVector(Movie::g_currentFrame, Movie::g_currentInputCount, PadStatus);
}

void TAStudioFrame::SetInput(GCPadStatus* PadStatus)
{
	if (!m_sendInputsToDolphin->GetValue())
	{
		return;
	}

	// Get input for corresponding inputCount (next in-game input)
	int inputFrame = Movie::g_currentInputCount;

	if (m_inputGrid->GetTAStudioInputVectorSize() <= inputFrame)
	{
		// Handle case where we've reached the end of the InputGrid table
		// Currently, this will start registering inputs by TASInput/Controller
		return;
	}

	*PadStatus = m_inputGrid->GetInputAtInputFrame(inputFrame);
}

void TAStudioFrame::UpdateGrid()
{
	m_inputGrid->UpdateGridValues();
}

void TAStudioFrame::OnLoadstateCallback()
{
	m_inputGrid->ParseStateInputs();
}

void TAStudioFrame::OnGroupByVIChanged(wxCommandEvent& evt)
{
	m_inputGrid->SetGroupByVI(m_groupByVI->GetValue());
}

void TAStudioFrame::OnAutoUpdateGridChanged(wxCommandEvent& evt)
{
	m_inputGrid->SetAutoUpdateGrid(!m_autoUpdateGrid->GetValue());
}

void TAStudioFrame::OnUpdateGridClick(wxCommandEvent& evt)
{
	m_inputGrid->UpdateGridValues();
}

void TAStudioFrame::OnGoToCurrentFrameClick(wxCommandEvent& evt)
{
	m_inputGrid->GoToCurrentFrame();
}

void TAStudioFrame::OnGoToInputFrameClick(wxCommandEvent& evt)
{
	wxTextEntryDialog inputFrameDialog(this, wxEmptyString, "Insert the desired input frame number");
	inputFrameDialog.SetTextValidator(wxFILTER_DIGITS);
	if (inputFrameDialog.ShowModal() == wxID_OK)
	{
		int value = wxAtoi(inputFrameDialog.GetValue());
		m_inputGrid->GoToInputFrame(value);
	}
	inputFrameDialog.Destroy();
}

void TAStudioFrame::OnGoToVIClick(wxCommandEvent& evt)
{
	wxTextEntryDialog viDialog(this, wxEmptyString, "Insert the desired VI number");
	viDialog.SetTextValidator(wxFILTER_DIGITS);
	if (viDialog.ShowModal() == wxID_OK)
	{
		int value = wxAtoi(viDialog.GetValue());
		m_inputGrid->GoToVI(value);
	}
	viDialog.Destroy();
}

void TAStudioFrame::OnInsertFrameAboveClick(wxCommandEvent& evt)
{
	m_inputGrid->InsertBlankFrame(false);
}

void TAStudioFrame::OnInsertFrameBelowClick(wxCommandEvent& evt)
{
	m_inputGrid->InsertBlankFrame(true);
}

void TAStudioFrame::OnToggleSelectedInputsClick(wxCommandEvent& evt)
{
	m_inputGrid->SetSelectedInputsPress(true);
}

void TAStudioFrame::OnSetSelectedInputsPressedClick(wxCommandEvent& evt)
{
	m_inputGrid->SetSelectedInputsPress(false, true);
}

void TAStudioFrame::OnSetSelectedInputsNotPressedClick(wxCommandEvent& evt)
{
	m_inputGrid->SetSelectedInputsPress(false, false);
}

void TAStudioFrame::OnCopyFrameClick(wxCommandEvent& evt)
{
	m_inputGrid->CopySelectedFrames();
}

void TAStudioFrame::OnPasteFramesAboveClick(wxCommandEvent& evt)
{
	m_inputGrid->PasteFrames(false);
}

void TAStudioFrame::OnPasteFramesBelowClick(wxCommandEvent& evt)
{
	m_inputGrid->PasteFrames(true);
}

// Stick functions

void TAStudioFrame::OnGetMainStickClick(wxCommandEvent& evt)
{
	GCPadStatus input = m_inputGrid->GetInputAtSelectedRow();
	m_main_stick.x_cont.text->SetValue(std::to_string(input.stickX));
	m_main_stick.y_cont.text->SetValue(std::to_string(input.stickY));
}

void TAStudioFrame::OnSetMainStickClick(wxCommandEvent& evt)
{
	m_inputGrid->SetMainStickInSelectedRows(m_main_stick.x_cont.value, m_main_stick.y_cont.value);
}

void TAStudioFrame::OnGetCStickClick(wxCommandEvent& evt)
{
	GCPadStatus input = m_inputGrid->GetInputAtSelectedRow();
	m_c_stick.x_cont.text->SetValue(std::to_string(input.substickX));
	m_c_stick.y_cont.text->SetValue(std::to_string(input.substickY));
}

void TAStudioFrame::OnSetCStickClick(wxCommandEvent& evt)
{
	m_inputGrid->SetCStickInSelectedRows(m_c_stick.x_cont.value, m_c_stick.y_cont.value);
}

wxBitmap TAStudioFrame::CreateStickBitmap(int x, int y)
{
	x = x / 2;
	y = y / 2;

	wxMemoryDC memDC;
	wxBitmap bitmap(129, 129);
	memDC.SelectObject(bitmap);
	memDC.SetBackground(*wxLIGHT_GREY_BRUSH);
	memDC.Clear();
	memDC.SetBrush(*wxWHITE_BRUSH);
	memDC.DrawCircle(65, 65, 64);
	memDC.SetBrush(*wxRED_BRUSH);
	memDC.DrawLine(64, 64, x, y);
	memDC.DrawLine(63, 64, x - 1, y);
	memDC.DrawLine(65, 64, x + 1, y);
	memDC.DrawLine(64, 63, x, y - 1);
	memDC.DrawLine(64, 65, x, y + 1);
	memDC.SetPen(*wxBLACK_PEN);
	memDC.CrossHair(64, 64);
	memDC.SetBrush(*wxBLUE_BRUSH);
	memDC.DrawCircle(x, y, 5);
	memDC.SelectObject(wxNullBitmap);
	return bitmap;
}

TAStudioFrame::Control TAStudioFrame::CreateControl(long style, int width, int height, bool reverse, u32 range, u32 default_value)
{
	Control tempCont;
	tempCont.range = range;
	tempCont.default_value = default_value;
	tempCont.slider = new wxSlider(this, m_eleID++, default_value, 0, range, wxDefaultPosition, wxDefaultSize, style);
	tempCont.slider->SetMinSize(wxSize(width, height));
	tempCont.slider->Bind(wxEVT_SLIDER, &TAStudioFrame::UpdateFromSliders, this);
	tempCont.text = new wxTextCtrl(this, m_eleID++, std::to_string(default_value), wxDefaultPosition, wxSize(40, 20));
	tempCont.text->SetMaxLength(range > 999 ? 4 : 3);
	tempCont.text_id = m_eleID - 1;
	tempCont.text->Bind(wxEVT_TEXT, &TAStudioFrame::UpdateFromText, this);
	tempCont.slider_id = m_eleID - 2;
	tempCont.reverse = reverse;
	return tempCont;
}

TAStudioFrame::Stick TAStudioFrame::CreateStick(int id_stick, int xRange, int yRange, u32 defaultX, u32 defaultY, bool reverseX, bool reverseY)
{
	Stick tempStick;
	tempStick.bitmap = new wxStaticBitmap(this, id_stick, CreateStickBitmap(128, 128), wxDefaultPosition, wxDefaultSize);
	tempStick.bitmap->Bind(wxEVT_MOTION, &TAStudioFrame::OnMouseDownL, this);
	tempStick.bitmap->Bind(wxEVT_LEFT_DOWN, &TAStudioFrame::OnMouseDownL, this);
	tempStick.bitmap->Bind(wxEVT_RIGHT_UP, &TAStudioFrame::OnMouseUpR, this);
	tempStick.x_cont = CreateControl(wxSL_HORIZONTAL | (reverseX ? wxSL_INVERSE : 0), 120, -1, reverseX, xRange, defaultX);
	tempStick.y_cont = CreateControl(wxSL_VERTICAL | (reverseY ? wxSL_INVERSE : 0), -1, 120, reverseY, yRange, defaultY);
	return tempStick;
}

wxStaticBoxSizer* TAStudioFrame::CreateStickLayout(Stick* tempStick, const wxString& title)
{
	wxStaticBoxSizer* const main_temp_box = new wxStaticBoxSizer(wxVERTICAL, this, title);
	wxBoxSizer* const temp_box = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* const temp_xslider_box = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* const temp_yslider_box = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* const temp_stick_box = new wxBoxSizer(wxVERTICAL);

	temp_xslider_box->Add(tempStick->x_cont.slider, 0, wxALIGN_TOP);
	temp_xslider_box->Add(tempStick->x_cont.text, 0, wxALIGN_TOP);
	temp_stick_box->Add(temp_xslider_box);
	temp_stick_box->Add(tempStick->bitmap, 0, wxALL | wxCenter, 3);
	temp_box->Add(temp_stick_box);
	temp_yslider_box->Add(tempStick->y_cont.slider, 0, wxALIGN_CENTER_VERTICAL);
	temp_yslider_box->Add(tempStick->y_cont.text, 0, wxALIGN_CENTER_VERTICAL);
	temp_box->Add(temp_yslider_box);
	main_temp_box->Add(temp_box);
	return main_temp_box;
}

void TAStudioFrame::UpdateFromSliders(wxCommandEvent& event)
{
	wxTextCtrl* text = nullptr;

	for (unsigned int i = 0; i < 4; ++i)
	{
		if (m_stick_controls[i] != nullptr && event.GetId() == m_stick_controls[i]->slider_id)
			text = m_stick_controls[i]->text;
	}

	int value = ((wxSlider*)event.GetEventObject())->GetValue();
	if (text)
		text->SetValue(std::to_string(value));
}

void TAStudioFrame::UpdateFromText(wxCommandEvent& event)
{
	unsigned long value;

	if (!((wxTextCtrl*)event.GetEventObject())->GetValue().ToULong(&value))
		return;

	for (unsigned int i = 0; i < 4; ++i)
	{
		if (m_stick_controls[i] != nullptr && event.GetId() == m_stick_controls[i]->text_id)
		{
			int v = value > m_stick_controls[i]->range ? m_stick_controls[i]->range : value;
			m_stick_controls[i]->slider->SetValue(v);
			m_stick_controls[i]->value = v;
		}
	}

	int x = (u8)(std::floor(((double)m_main_stick.x_cont.value / (double)m_main_stick.x_cont.range * 255.0) + .5));
	int y = (u8)(std::floor(((double)m_main_stick.y_cont.value / (double)m_main_stick.y_cont.range * 255.0) + .5));
	if (m_main_stick.x_cont.reverse)
		x = 256 - (u8)x;
	if (m_main_stick.y_cont.reverse)
		y = 256 - (u8)y;
	m_main_stick.bitmap->SetBitmap(CreateStickBitmap(x, y));

	x = (u8)(std::floor(((double)m_c_stick.x_cont.value / (double)m_c_stick.x_cont.range * 255.0) + .5));
	y = (u8)(std::floor(((double)m_c_stick.y_cont.value / (double)m_c_stick.y_cont.range * 255.0) + .5));
	if (m_c_stick.x_cont.reverse)
		x = 256 - (u8)x;
	if (m_c_stick.y_cont.reverse)
		y = 256 - (u8)y;
	m_c_stick.bitmap->SetBitmap(CreateStickBitmap(x, y));
}

void TAStudioFrame::OnMouseDownL(wxMouseEvent& event)
{
	if (!event.LeftIsDown())
		return;

	Stick* stick;
	if (event.GetId() == ID_MAIN_STICK)
		stick = &m_main_stick;
	else if (event.GetId() == ID_C_STICK)
		stick = &m_c_stick;
	else
		return;

	wxPoint ptM(event.GetPosition());
	stick->x_cont.value = ptM.x * stick->x_cont.range / 127;
	stick->y_cont.value = ptM.y * stick->y_cont.range / 127;

	if ((unsigned)stick->y_cont.value > stick->y_cont.range)
		stick->y_cont.value = stick->y_cont.range;
	if ((unsigned)stick->x_cont.value > stick->x_cont.range)
		stick->x_cont.value = stick->x_cont.range;

	if (stick->y_cont.reverse)
		stick->y_cont.value = stick->y_cont.range - (u16)stick->y_cont.value;
	if (stick->x_cont.reverse)
		stick->x_cont.value = stick->x_cont.range - (u16)stick->x_cont.value;

	stick->x_cont.value = (unsigned int)stick->x_cont.value > stick->x_cont.range ? stick->x_cont.range : stick->x_cont.value;
	stick->y_cont.value = (unsigned int)stick->y_cont.value > stick->y_cont.range ? stick->y_cont.range : stick->y_cont.value;

	stick->bitmap->SetBitmap(CreateStickBitmap(ptM.x * 2, ptM.y * 2));

	stick->x_cont.text->SetValue(std::to_string(stick->x_cont.value));
	stick->y_cont.text->SetValue(std::to_string(stick->y_cont.value));

	stick->x_cont.slider->SetValue(stick->x_cont.value);
	stick->y_cont.slider->SetValue(stick->y_cont.value);
	event.Skip(true);
}

void TAStudioFrame::OnMouseUpR(wxMouseEvent& event)
{
	Stick* stick = nullptr;
	if (event.GetId() == ID_MAIN_STICK)
		stick = &m_main_stick;
	else if (event.GetId() == ID_C_STICK)
		stick = &m_c_stick;

	if (stick == nullptr)
		return;

	stick->x_cont.value = stick->x_cont.default_value;
	stick->y_cont.value = stick->y_cont.default_value;
	stick->bitmap->SetBitmap(CreateStickBitmap(128, 128));
	stick->x_cont.text->SetValue(std::to_string(stick->x_cont.default_value));
	stick->y_cont.text->SetValue(std::to_string(stick->y_cont.default_value));
	stick->x_cont.slider->SetValue(stick->x_cont.default_value);
	stick->y_cont.slider->SetValue(stick->y_cont.default_value);

	event.Skip(true);
}

void TAStudioFrame::OnRightClickSlider(wxMouseEvent& event)
{
	for (unsigned int i = 0; i < 4; ++i)
	{
		if (m_stick_controls[i] != nullptr && event.GetId() == m_stick_controls[i]->slider_id)
		{
			m_stick_controls[i]->value = m_stick_controls[i]->default_value;
			m_stick_controls[i]->slider->SetValue(m_stick_controls[i]->default_value);
			m_stick_controls[i]->text->SetValue(std::to_string(m_stick_controls[i]->default_value));
		}
	}
}

// Input grid

InputGrid::InputGrid(wxWindow* parent) : wxGrid(parent, wxID_ANY)
{
	m_firstInputInGrid = 1;
	m_firstFrameInGrid = 1;
	m_gridNumberOfRows = 40;

	m_groupByVI = false;
	m_autoUpdateGrid = true;

	int numColumns = COLUMN_LABEL.size();
	SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
	CreateGrid(m_gridNumberOfRows, numColumns, wxGridSelectCells);
	HideRowLabels();
	DisableDragGridSize();
	wxGridCellAttr *readOnlyAttr = new wxGridCellAttr;
	readOnlyAttr->SetReadOnly();
	for (int i = 0; i < numColumns; i++)
	{
		SetColLabelValue(i, COLUMN_LABEL[i]);
		switch (i)
		{
		case COLUMN_INPUT_COUNT:
		case COLUMN_VI_COUNT:
		case COLUMN_ACTIVE:
			SetColAttr(i, readOnlyAttr);
			readOnlyAttr->IncRef();
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
			SetColAttr(i, readOnlyAttr);
			readOnlyAttr->IncRef();
			SetColSize(i, 30);
			break;
		default:
			SetColAttr(i, readOnlyAttr);
			readOnlyAttr->IncRef();
			SetColSize(i, 20);
			break;
		}
	}
}

void InputGrid::SetGroupByVI(bool value)
{
	if (value)
	{
		if (m_inputVector.size() > m_firstInputInGrid)
		{
			m_firstFrameInGrid = m_inputVector[m_firstInputInGrid].FrameCount;
		}
	}
	else
	{
		if (m_viToInputCount.size() > m_firstFrameInGrid)
		{
			m_firstInputInGrid = m_viToInputCount[m_firstFrameInGrid][0];
		}
	}
	m_groupByVI = value;
	UpdateGridValues();
}

void InputGrid::SetAutoUpdateGrid(bool value)
{
	m_autoUpdateGrid = value;
}

void InputGrid::SetMainStickInSelectedRows(u8 x, u8 y)
{
	wxGridCellCoords topLeft = GetSelectionBlockTopLeft()[0];
	wxGridCellCoords botRight = GetSelectionBlockBottomRight()[0];

	for (int row = topLeft.GetRow(); row <= botRight.GetRow(); row++)
	{
		if (m_groupByVI)
		{
			const int frameCount = m_firstFrameInGrid + row;
			if (m_viToInputCount[frameCount].size() == 0)
			{
				continue;
			}
			for (int j = 0; j < m_viToInputCount[frameCount].size(); j++)
			{
				m_inputVector[m_viToInputCount[frameCount][j]].Input.stickX = x;
				m_inputVector[m_viToInputCount[frameCount][j]].Input.stickY = y;
			}
		}
		else
		{
			const int inputCount = m_firstInputInGrid + row;
			if (inputCount >= m_inputVector.size())
			{
				continue;
			}
			m_inputVector[inputCount].Input.stickX = x;
			m_inputVector[inputCount].Input.stickY = y;
		}
	}

	UpdateGridValues();
}

void InputGrid::SetCStickInSelectedRows(u8 x, u8 y)
{
	wxGridCellCoords topLeft = GetSelectionBlockTopLeft()[0];
	wxGridCellCoords botRight = GetSelectionBlockBottomRight()[0];

	for (int row = topLeft.GetRow(); row <= botRight.GetRow(); row++)
	{
		if (m_groupByVI)
		{
			const int frameCount = m_firstFrameInGrid + row;
			if (m_viToInputCount[frameCount].size() == 0)
			{
				continue;
			}
			for (int j = 0; j < m_viToInputCount[frameCount].size(); j++)
			{
				m_inputVector[m_viToInputCount[frameCount][j]].Input.substickX = x;
				m_inputVector[m_viToInputCount[frameCount][j]].Input.substickY = y;
			}
		}
		else
		{
			const int inputCount = m_firstInputInGrid + row;
			if (inputCount >= m_inputVector.size())
			{
				continue;
			}
			m_inputVector[inputCount].Input.substickX = x;
			m_inputVector[inputCount].Input.substickY = y;
		}
	}

	UpdateGridValues();
}

void InputGrid::ParseStateInputs()
{
	Movie::ControllerState tempState;
	GCPadStatus temp;

	// Fill possibly empty inputs
	while (m_inputVector.size() < (Movie::g_movInputsLen / 8))
	{
		m_inputVector.push_back(TAStudioInput());
	}

	for (u32 i = 0; i < Movie::g_movInputsLen / 8; i++)
	{
		memcpy(&tempState, &Movie::g_movInputs[i * 8], 8);
		temp.button = 0;
		temp.button |= tempState.A ? PAD_BUTTON_A : 0;
		temp.button |= tempState.B ? PAD_BUTTON_B : 0;
		temp.button |= tempState.X ? PAD_BUTTON_X : 0;
		temp.button |= tempState.Y ? PAD_BUTTON_Y : 0;
		temp.button |= tempState.Z ? PAD_TRIGGER_Z : 0;
		temp.button |= tempState.Start ? PAD_BUTTON_START : 0;
		temp.button |= tempState.L ? PAD_TRIGGER_L : 0;
		temp.button |= tempState.R ? PAD_TRIGGER_R : 0;
		temp.button |= tempState.DPadUp ? PAD_BUTTON_UP : 0;
		temp.button |= tempState.DPadDown ? PAD_BUTTON_DOWN : 0;
		temp.button |= tempState.DPadLeft ? PAD_BUTTON_LEFT : 0;
		temp.button |= tempState.DPadRight ? PAD_BUTTON_RIGHT : 0;
		temp.stickX = tempState.AnalogStickX;
		temp.stickY = tempState.AnalogStickY;
		temp.substickX = tempState.CStickX;
		temp.substickY = tempState.CStickY;
		temp.triggerLeft = tempState.TriggerL;
		temp.triggerRight = tempState.TriggerR;

		// Add input to the vector without referencing the frameCount
		m_inputVector[i].Input = temp;
	}

	UpdateGridValues();
}

int InputGrid::GetTAStudioInputVectorSize()
{
	return m_inputVector.size();
}

GCPadStatus InputGrid::GetInputAtInputFrame(int inputCount)
{
	return m_inputVector[inputCount].Input;
}

void InputGrid::SetInputAtInputFrame(GCPadStatus* PadStatus, int inputCount)
{
	m_inputVector[inputCount].Input = *PadStatus;
}

void InputGrid::GoToCurrentFrame()
{
	if (m_groupByVI)
	{
		m_firstFrameInGrid = Movie::g_currentFrame - m_gridNumberOfRows / 2;
		UpdateGridValues();
	}
	else
	{
		m_firstInputInGrid = Movie::g_currentInputCount - m_gridNumberOfRows / 2;
		UpdateGridValues();
	}
}

void InputGrid::GoToInputFrame(int frame)
{
	if (m_groupByVI)
	{
		if (frame >= m_inputVector.size())
		{
			return;
		}
		m_firstFrameInGrid = m_inputVector[frame].FrameCount - m_gridNumberOfRows / 2;
		UpdateGridValues();
	}
	else
	{
		m_firstInputInGrid = frame - m_gridNumberOfRows / 2;
		UpdateGridValues();
	}
}

void InputGrid::GoToVI(int vi)
{
	if (m_groupByVI)
	{
		m_firstFrameInGrid = vi - m_gridNumberOfRows / 2;
		UpdateGridValues();
	}
	else
	{
		if (vi >= m_viToInputCount.size())
		{
			return;
		}
		m_firstInputInGrid = m_viToInputCount[vi][0] - m_gridNumberOfRows / 2;
		UpdateGridValues();
	}
}

void InputGrid::ToggleCellValue(int row, int col)
{
	std::string cell = this->GetCellValue(row, col);
	switch (col)
	{
	case COLUMN_L_ANA:
	case COLUMN_R_ANA:
		SetCellInput(row, col, (cell == "0"));
		break;
	default:
		SetCellInput(row, col, !(cell == COLUMN_LABEL[col]));
		break;
	}
}

void InputGrid::OnCellRightClick(wxGridEvent& evt)
{
	SetSelectedInputsPress(true);
}

GCPadStatus* InputGrid::BlankInput()
{
	GCPadStatus* input = new GCPadStatus();
	input->stickX = input->MAIN_STICK_CENTER_X;
	input->stickY = input->MAIN_STICK_CENTER_Y;
	input->substickX = input->C_STICK_CENTER_X;
	input->substickY = input->C_STICK_CENTER_Y;
	return input;
}

void InputGrid::InsertBlankFrame(bool below)
{
	int pos;
	if (GetSelectionBlockTopLeft().size() > 0)
	{
		if (below)
		{
			wxGridCellCoords botRight = GetSelectionBlockBottomRight()[0];
			pos = botRight.GetRow() + 1;
		}
		else
		{
			wxGridCellCoords topLeft = GetSelectionBlockTopLeft()[0];
			pos = topLeft.GetRow();
		}
	}
	else
	{
		if (below)
		{
			pos = GetGridCursorRow() + 1;
		}
		else
		{
			pos = GetGridCursorRow();
		}
	}

	if (m_groupByVI)
	{
		pos += m_firstFrameInGrid;
		std::vector<GCPadStatus> blankInputs;
		for (int i = 0; i < m_viToInputCount[pos].size(); i++)
		{
			blankInputs.push_back(*BlankInput());
		}
		InsertInputsAtInputCount(m_viToInputCount[pos][0], blankInputs.data(), blankInputs.size());
	}
	else
	{
		pos += m_firstInputInGrid;
		InsertInputsAtInputCount(pos, BlankInput(), 1);
	}
}

void InputGrid::SetSelectedInputsPress(bool toggle, bool value)
{
	if (GetSelectionBlockTopLeft().size() > 0) {
		wxGridCellCoords topLeft = GetSelectionBlockTopLeft()[0];
		wxGridCellCoords botRight = GetSelectionBlockBottomRight()[0];

		for (int col = topLeft.GetCol(); col <= botRight.GetCol(); col++)
		{
			switch (col)
			{
			case COLUMN_A:
			case COLUMN_B:
			case COLUMN_X:
			case COLUMN_Y:
			case COLUMN_S:
			case COLUMN_Z:
			case COLUMN_L:
			case COLUMN_R:
			case COLUMN_D_UP:
			case COLUMN_D_DOWN:
			case COLUMN_D_LEFT:
			case COLUMN_D_RIGHT:
			case COLUMN_L_ANA:
			case COLUMN_R_ANA:
				for (int row = topLeft.GetRow(); row <= botRight.GetRow(); row++)
				{
					if (m_groupByVI)
					{
						int currFrameCount = row + m_firstFrameInGrid;
						if (m_viToInputCount[currFrameCount].size() == 0)
						{
							continue;
						}
					}
					else
					{
						if (row + m_firstInputInGrid >= m_inputVector.size())
						{
							continue;
						}
					}
					if (toggle)
					{
						ToggleCellValue(row, col);
					}
					else
					{
						SetCellInput(row, col, value);
					}
				}
				break;
			default:
				break;
			}
		}
	}
	else
	{
		int row = GetGridCursorRow();
		int col = GetGridCursorCol();
		switch (col)
		{
		case COLUMN_A:
		case COLUMN_B:
		case COLUMN_X:
		case COLUMN_Y:
		case COLUMN_S:
		case COLUMN_Z:
		case COLUMN_L:
		case COLUMN_R:
		case COLUMN_D_UP:
		case COLUMN_D_DOWN:
		case COLUMN_D_LEFT:
		case COLUMN_D_RIGHT:
		case COLUMN_L_ANA:
		case COLUMN_R_ANA:
			if (m_groupByVI)
			{
				int currFrameCount = row + m_firstFrameInGrid;
				if (m_viToInputCount[currFrameCount].size() == 0)
				{
					break;
				}
			}
			else
			{
				if (row + m_firstInputInGrid >= m_inputVector.size())
				{
					break;
				}
			}
			if (toggle)
			{
				ToggleCellValue(row, col);
			}
			else
			{
				SetCellInput(row, col, value);
			}
			break;
		default:
			break;
		}
	}
}

void InputGrid::SetCellInput(int row, int col, bool value)
{
	if (value)
	{
		switch (col)
		{
		case COLUMN_L_ANA:
		case COLUMN_R_ANA:
			SetCellValue(row, col, "255");
			break;
		default:
			SetCellValue(row, col, COLUMN_LABEL[col]);
			break;
		}
	}
	else
	{
		switch (col)
		{
		case COLUMN_L_ANA:
		case COLUMN_R_ANA:
			SetCellValue(row, col, "0");
			break;
		default:
			SetCellValue(row, col, "");
			break;
		}
	}

	// Update the value(s) in the vector
	if (m_groupByVI)
	{
		int currFrameCount = row + m_firstFrameInGrid;
		// Repeat for every input in m_inputVector whose FrameCount == currFrameCount
		for (int i = 0; i < m_viToInputCount[currFrameCount].size(); i++)
		{
			m_inputVector[m_viToInputCount[currFrameCount][i]].Input = GetInputAtRow(row);
		}
	}
	else
	{
		m_inputVector[row + m_firstInputInGrid].Input = GetInputAtRow(row);
	}
}

void InputGrid::CopySelectedFrames()
{
	m_vectorClipboard.clear();
	if (GetSelectionBlockTopLeft().size() > 0)
	{
		wxGridCellCoords topLeft = GetSelectionBlockTopLeft()[0];
		wxGridCellCoords botRight = GetSelectionBlockBottomRight()[0];
		for (int row = topLeft.GetRow(); row <= botRight.GetRow(); row++)
		{
			if (m_groupByVI)
			{
				int currFrameCount = row + m_firstFrameInGrid;
				for (int i = 0; i < m_viToInputCount[currFrameCount].size(); i++)
				{
					m_vectorClipboard.push_back(m_inputVector[m_viToInputCount[currFrameCount][i]].Input);
				}
			}
			else
			{
				if (row + m_firstInputInGrid >= m_inputVector.size())
				{
					continue;
				}
				m_vectorClipboard.push_back(m_inputVector[row + m_firstInputInGrid].Input);
			}
		}
	}
	else
	{
		int row = GetGridCursorRow();
		if (m_groupByVI)
		{
			// currCount = frameCount
			int currFrameCount = row + m_firstFrameInGrid;
			for (int i = 0; i < m_viToInputCount[currFrameCount].size(); i++)
			{
				m_vectorClipboard.push_back(m_inputVector[m_viToInputCount[currFrameCount][i]].Input);
			}
		}
		else
		{
			// currCount = inputCount
			if (row + m_firstInputInGrid >= m_inputVector.size())
			{
				return;
			}
			m_vectorClipboard.push_back(m_inputVector[row + m_firstInputInGrid].Input);
		}
	}
}

void InputGrid::PasteFrames(bool below)
{
	if (m_vectorClipboard.size() == 0)
	{
		return;
	}
	int pos;

	if (GetSelectionBlockTopLeft().size() > 0)
	{
		wxGridCellCoords botRight = GetSelectionBlockBottomRight()[0];
		pos = botRight.GetRow() + 1;
	}
	else
	{
		pos = GetGridCursorRow() + 1;
	}

	if (m_groupByVI)
	{
		pos += m_firstFrameInGrid;
		// Convert frameCount to inputCount
		pos = m_viToInputCount[pos][0];
	}
	else
	{
		pos += m_firstInputInGrid;
	}
	InsertInputsAtInputCount(pos, m_vectorClipboard.data(), m_vectorClipboard.size());
}

void InputGrid::OnCellChanged(wxGridEvent& evt)
{
	int row = evt.GetRow();

	// Update the value(s) in the vector
	if (m_groupByVI)
	{
		int currFrameCount = row + m_firstFrameInGrid;
		// Repeat for every input in m_inputVector whose FrameCount == currFrameCount
		for (int i = 0; i < m_viToInputCount[currFrameCount].size(); i++)
		{
			m_inputVector[m_viToInputCount[currFrameCount][i]].Input = GetInputAtRow(row);
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

void InputGrid::InsertInputsAtInputCount(u64 inputCount, GCPadStatus* input, int size)
{
	if (inputCount >= m_inputVector.size())
	{
		return;
	}
	auto itPos = m_inputVector.begin() + inputCount;
	TAStudioInput* tastudioInputs = new TAStudioInput[size];
	for (int i = 0; i < size; i++) {
		tastudioInputs[i].FrameCount = itPos->FrameCount + i;
		tastudioInputs[i].Input = input[i];
	}
	m_inputVector.insert(itPos, tastudioInputs, tastudioInputs + size);
	for (int i = inputCount + 1; i < m_inputVector.size(); i++)
	{
		m_inputVector[i].FrameCount += size;
	}
	UpdateGridValues();
}

void InputGrid::AddInputToVector(u64 frameCount, u64 inputCount, GCPadStatus* input)
{
	// This should NEVER repeat more than once after the load state functions are implemented
	// But I left it here to make sure nothing breaks.
	while (m_inputVector.size() < (inputCount + 1))
	{
		m_inputVector.push_back(TAStudioInput());
	}

	m_inputVector[inputCount].FrameCount = frameCount;
	m_inputVector[inputCount].Input = *input;

	// Creating space for m_viToInputCount
	while (m_viToInputCount.size() < (frameCount + 1))
	{
		m_viToInputCount.push_back(std::vector<u64>());
	}

	// Including inputCount in m_viToInputCount[frameCount] list if it doesn't contain inputCount yet
	const auto it = std::find(m_viToInputCount[frameCount].begin(), m_viToInputCount[frameCount].end(), inputCount);
	if (it == m_viToInputCount[frameCount].end())
	{
		m_viToInputCount[frameCount].push_back(inputCount);
	}

	if (!m_autoUpdateGrid)
	{
		return;
	}

	// Insert input in visual grid
	if (m_groupByVI)
	{
		if (frameCount == m_firstFrameInGrid + m_gridNumberOfRows - 1)
		{
			UpdateGridValues();
		}
		if (frameCount < m_firstFrameInGrid || frameCount > m_firstFrameInGrid + m_gridNumberOfRows - 1)
		{
			m_firstFrameInGrid = frameCount;
		}
	}
	else
	{
		if (inputCount == m_firstInputInGrid + m_gridNumberOfRows - 1)
		{
			UpdateGridValues();
		}
		if (inputCount < m_firstInputInGrid || inputCount > m_firstInputInGrid + m_gridNumberOfRows - 1)
		{
			m_firstInputInGrid = inputCount;
		}
	}
}

void InputGrid::UpdateGridValues()
{
	BeginBatch();
	for (int i = 0; i < m_gridNumberOfRows; i++)
	{
		if (m_groupByVI)
		{
			u64 currFrameCount = i + m_firstFrameInGrid;
			// Find if given frameCount exists in m_inputVector
			if (currFrameCount < m_viToInputCount.size())
			{
				if (m_viToInputCount[currFrameCount].size() > 0)
				{
					u64 inputCount = m_viToInputCount[currFrameCount][0];
					SetInputAtRow(i, m_inputVector[inputCount], inputCount);
				}
				else
				{
					DeleteInputAtRow(i);
				}
			}
			else
			{
				DeleteInputAtRow(i);
			}
		}
		else
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
	EndBatch();
}

void InputGrid::DeleteInputAtRow(int row)
{
	for (int i = 0; i < COLUMN_LABEL.size(); i++)
	{
		SetCellValue(row, i, "");
	}
}

void InputGrid::SetInputAtRow(int row, TAStudioInput tastudioInput, u64 inputCount)
{
	GCPadStatus padStatus = tastudioInput.Input;

	SetCellValue(row, COLUMN_INPUT_COUNT, std::to_string(inputCount));
	SetCellValue(row, COLUMN_VI_COUNT, std::to_string(tastudioInput.FrameCount));

	int diff = (m_groupByVI ? tastudioInput.FrameCount - Movie::g_currentFrame : inputCount - Movie::g_currentInputCount);

	SetCellValue(row, COLUMN_ACTIVE, diff == 0 ? "*" : std::to_string(diff));

	SetCellValue(row, COLUMN_ANA_X, std::to_string(padStatus.stickX));
	SetCellValue(row, COLUMN_ANA_Y, std::to_string(padStatus.stickY));
	SetCellValue(row, COLUMN_C_X, std::to_string(padStatus.substickX));
	SetCellValue(row, COLUMN_C_Y, std::to_string(padStatus.substickY));
	SetCellValue(row, COLUMN_L_ANA, std::to_string(padStatus.triggerLeft));
	SetCellValue(row, COLUMN_R_ANA, std::to_string(padStatus.triggerRight));

	SetCellValue(row, COLUMN_A, padStatus.button & PAD_BUTTON_A ? COLUMN_LABEL[COLUMN_A] : "");
	SetCellValue(row, COLUMN_B, padStatus.button & PAD_BUTTON_B ? COLUMN_LABEL[COLUMN_B] : "");
	SetCellValue(row, COLUMN_X, padStatus.button & PAD_BUTTON_X ? COLUMN_LABEL[COLUMN_X] : "");
	SetCellValue(row, COLUMN_Y, padStatus.button & PAD_BUTTON_Y ? COLUMN_LABEL[COLUMN_Y] : "");
	SetCellValue(row, COLUMN_S, padStatus.button & PAD_BUTTON_START ? COLUMN_LABEL[COLUMN_S] : "");
	SetCellValue(row, COLUMN_Z, padStatus.button & PAD_TRIGGER_Z ? COLUMN_LABEL[COLUMN_Z] : "");
	SetCellValue(row, COLUMN_L, padStatus.button & PAD_TRIGGER_L ? COLUMN_LABEL[COLUMN_L] : "");
	SetCellValue(row, COLUMN_R, padStatus.button & PAD_TRIGGER_R ? COLUMN_LABEL[COLUMN_R] : "");
	SetCellValue(row, COLUMN_D_UP, padStatus.button & PAD_BUTTON_UP ? COLUMN_LABEL[COLUMN_D_UP] : "");
	SetCellValue(row, COLUMN_D_DOWN, padStatus.button & PAD_BUTTON_DOWN ? COLUMN_LABEL[COLUMN_D_DOWN] : "");
	SetCellValue(row, COLUMN_D_LEFT, padStatus.button & PAD_BUTTON_LEFT ? COLUMN_LABEL[COLUMN_D_LEFT] : "");
	SetCellValue(row, COLUMN_D_RIGHT, padStatus.button & PAD_BUTTON_RIGHT ? COLUMN_LABEL[COLUMN_D_RIGHT] : "");

}

GCPadStatus InputGrid::GetInputAtSelectedRow()
{
	if (GetSelectionBlockTopLeft().size() > 0)
	{
		wxGridCellCoords topLeft = GetSelectionBlockTopLeft()[0];
		int row = topLeft.GetRow();
		return GetInputAtRow(row);
	}
	else
	{
		int row = GetGridCursorRow();
		return GetInputAtRow(row);
	}
}

GCPadStatus InputGrid::GetInputAtRow(u64 row)
{
	GCPadStatus PadStatus;

	// Use wxAtoi to convert wxString to integer
	PadStatus.stickX = wxAtoi(GetCellValue(row, COLUMN_ANA_X));
	PadStatus.stickY = wxAtoi(GetCellValue(row, COLUMN_ANA_Y));
	PadStatus.substickX = wxAtoi(GetCellValue(row, COLUMN_C_X));
	PadStatus.substickY = wxAtoi(GetCellValue(row, COLUMN_C_Y));
	PadStatus.triggerLeft = wxAtoi(GetCellValue(row, COLUMN_L_ANA));
	PadStatus.triggerRight = wxAtoi(GetCellValue(row, COLUMN_R_ANA));

	PadStatus.button = 0;
	PadStatus.button |= GetCellValue(row, COLUMN_A) == COLUMN_LABEL[COLUMN_A] ? PAD_BUTTON_A : 0;
	PadStatus.button |= GetCellValue(row, COLUMN_B) == COLUMN_LABEL[COLUMN_B] ? PAD_BUTTON_B : 0;
	PadStatus.button |= GetCellValue(row, COLUMN_X) == COLUMN_LABEL[COLUMN_X] ? PAD_BUTTON_X : 0;
	PadStatus.button |= GetCellValue(row, COLUMN_Y) == COLUMN_LABEL[COLUMN_Y] ? PAD_BUTTON_Y : 0;
	PadStatus.button |= GetCellValue(row, COLUMN_S) == COLUMN_LABEL[COLUMN_S] ? PAD_BUTTON_START : 0;
	PadStatus.button |= GetCellValue(row, COLUMN_Z) == COLUMN_LABEL[COLUMN_Z] ? PAD_TRIGGER_Z : 0;
	PadStatus.button |= GetCellValue(row, COLUMN_L) == COLUMN_LABEL[COLUMN_L] ? PAD_TRIGGER_L : 0;
	PadStatus.button |= GetCellValue(row, COLUMN_R) == COLUMN_LABEL[COLUMN_R] ? PAD_TRIGGER_R : 0;
	PadStatus.button |= GetCellValue(row, COLUMN_D_UP) == COLUMN_LABEL[COLUMN_D_UP] ? PAD_BUTTON_UP : 0;
	PadStatus.button |= GetCellValue(row, COLUMN_D_DOWN) == COLUMN_LABEL[COLUMN_D_DOWN] ? PAD_BUTTON_DOWN : 0;
	PadStatus.button |= GetCellValue(row, COLUMN_D_LEFT) == COLUMN_LABEL[COLUMN_D_LEFT] ? PAD_BUTTON_LEFT : 0;
	PadStatus.button |= GetCellValue(row, COLUMN_D_RIGHT) == COLUMN_LABEL[COLUMN_D_RIGHT] ? PAD_BUTTON_RIGHT : 0;

	return PadStatus;
}