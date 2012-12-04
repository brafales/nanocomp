/*
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
                                                                          */
// $Revision: 1.28 $

#include "layoutView.hpp"

#include <wx/statline.h>
#include "resources/fileopen.xpm"
#include "resources/filesave.xpm"
#include "resources/filesaveas.xpm"
#include "resources/new.xpm"
#include "resources/forward.xpm"

enum
{
    ID_TEXT_NAME = 0,
    ID_TEXT_DESCRIPTION,
    ID_BUTTON_NEW,
    ID_BUTTON_OPEN,
    ID_BUTTON_SAVE,
    ID_BUTTON_SAVEAS,
    ID_BUTTON_ASSIGNINPUT,
    ID_BUTTON_ASSIGNOUTPUT,
    ID_BUTTON_PREPARE,
    ID_BUTTON_START,
    ID_LISTTABLES,
    ID_LISTRULES,
    ID_LISTFPS,
    ID_LISTINPUTS,
    ID_LISTOUTPUTS,
    ID_CANVAS
};


BEGIN_EVENT_TABLE(LayoutView, wxPanel)
    EVT_LAYOUT_CANVAS  (ID_CANVAS,  LayoutView::OnClick)
    EVT_BUTTON  (ID_BUTTON_NEW, LayoutView::OnNew)
    EVT_BUTTON  (ID_BUTTON_OPEN, LayoutView::OnOpen)
    EVT_BUTTON  (ID_BUTTON_SAVE, LayoutView::OnSave)
    EVT_BUTTON  (ID_BUTTON_SAVEAS, LayoutView::OnSaveAs)
    EVT_BUTTON  (ID_BUTTON_ASSIGNINPUT, LayoutView::OnAssignInput)
    EVT_BUTTON  (ID_BUTTON_ASSIGNOUTPUT, LayoutView::OnAssignOutput)
    EVT_BUTTON  (ID_BUTTON_PREPARE, LayoutView::OnPrepare)
    EVT_BUTTON  (ID_BUTTON_START, LayoutView::OnStart)
    EVT_LISTBOX (ID_LISTTABLES, LayoutView::OnTableSelected)
    EVT_LISTBOX (ID_LISTINPUTS, LayoutView::OnInputSelected)
    EVT_LISTBOX (ID_LISTOUTPUTS, LayoutView::OnOutputSelected)
    EVT_CHECKLISTBOX (ID_LISTTABLES, LayoutView::OnTableChecked)
    EVT_CHECKLISTBOX (ID_LISTRULES, LayoutView::OnRuleChecked)
    EVT_CHECKLISTBOX (ID_LISTFPS, LayoutView::OnFPChecked)
    EVT_KEY_UP (LayoutView::OnKeyPressed)
END_EVENT_TABLE()


//! Constructor
/*!
   \param parent the parent window.
   \param id the window identifier.
   \param controller the space controller.
*/
LayoutView::LayoutView(wxWindow* parent, wxWindowID id, LayoutManager *controller)
    :wxPanel(parent, id)
{
    this->controller = controller;
    initControls();
    initSizers();
    controller->setView(this);
    buttonPrepareSimulation->Enable(true);
    buttonStartSimulation->Enable(false);
}


//! Destructor.
LayoutView::~LayoutView()
{
}


//! Control initialization
void LayoutView::initControls()
{
    //Buttons
    wxBitmap newBitmap(wxBITMAP(new));
    wxBitmap saveBitmap(wxBITMAP(filesave));
    wxBitmap saveAsBitmap(wxBITMAP(filesaveas));
    wxBitmap openBitmap(wxBITMAP(fileopen));
    wxBitmap assignBitmap(wxBITMAP(forward));
    buttonNew = new wxBitmapButton(this, ID_BUTTON_NEW, newBitmap);
    buttonNew->SetToolTip(_("New layout"));
    buttonOpen = new wxBitmapButton(this, ID_BUTTON_OPEN, openBitmap);
    buttonOpen->SetToolTip(_("Open a layout file"));
    buttonSave = new wxBitmapButton(this, ID_BUTTON_SAVE, saveBitmap);
    buttonSave->SetToolTip(_("Save selected layout to a file"));
    buttonSaveAs = new wxBitmapButton(this, ID_BUTTON_SAVEAS, saveAsBitmap);
    buttonSaveAs->SetToolTip(_("Save selected layout to new a file"));
    buttonAssignInput = new wxBitmapButton(this, ID_BUTTON_ASSIGNINPUT, assignBitmap);
    buttonAssignInput->SetToolTip(_("Assign input to layout"));
    buttonAssignOutput = new wxBitmapButton(this, ID_BUTTON_ASSIGNOUTPUT, assignBitmap);
    buttonAssignOutput->SetToolTip(_("Assign output to layout"));
    buttonSave->Enable(false);
    buttonSaveAs->Enable(false);
    buttonAssignInput->Enable(false);
    buttonAssignOutput->Enable(false);
    buttonPrepareSimulation = new wxButton(this, ID_BUTTON_PREPARE, _("Check simulation"));
    buttonStartSimulation = new wxButton(this, ID_BUTTON_START, _("Begin simulation"));
    
    //Other controls
    listTables = new wxCheckListBox(this, ID_LISTTABLES);
    listRules = new wxCheckListBox(this, ID_LISTRULES);
    listFPs = new wxCheckListBox(this, ID_LISTFPS);
    listInputs = new wxListBox(this, ID_LISTINPUTS);
    listOutputs = new wxListBox(this, ID_LISTOUTPUTS);
    canvas = new LayoutCanvas(this, ID_CANVAS, Grid());
}


//! Sizers (window layout) initialization.
void LayoutView::initSizers()
{
   //Main Sizer
    wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);

        //TTList and I/O sizer
        wxBoxSizer *listsSizer = new wxBoxSizer(wxVERTICAL);
            
            //List sizer
            wxStaticBoxSizer *TTListSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Truth Table list"));
            TTListSizer->Add(listTables, 1, wxEXPAND | wxALL, 1);
            wxBoxSizer *IOSizer = new wxBoxSizer(wxVERTICAL);
            //wxBoxSizer *inputsSizer = new wxBoxSizer(wxVERTICAL);
            wxBoxSizer *listButtonInputSizer = new wxBoxSizer(wxHORIZONTAL);
            //inputsSizer->Add(labelInputs, 0, wxEXPAND | wxALL | wxALIGN_CENTER, 1);
            listButtonInputSizer->Add(listInputs, 1, wxEXPAND | wxALL, 1);
            listButtonInputSizer->Add(buttonAssignInput, 0, wxSHAPED | wxALL | wxALIGN_CENTER, 1);
            //inputsSizer->Add(listButtonInputSizer, 0, wxEXPAND | wxALL | wxALIGN_CENTER, 1);
            //inputsSizer->Add(listInputs, 1, wxEXPAND | wxALL, 1);
            //inputsSizer->Add(buttonAssignInput, 0, wxSHAPED | wxALL | wxALIGN_CENTER, 1);
            //IOSizer->Add(inputsSizer, 1, wxSHAPED | wxALL | wxALIGN_CENTER, 1);
            IOSizer->Add(listButtonInputSizer, 1, wxSHAPED | wxALL | wxALIGN_CENTER, 1);
            
            //wxBoxSizer *outputsSizer = new wxBoxSizer(wxVERTICAL);
            wxBoxSizer *listButtonOutputSizer = new wxBoxSizer(wxHORIZONTAL);
            //outputsSizer->Add(labelOutputs, 0, wxEXPAND | wxALL | wxALIGN_CENTER, 1);
            listButtonOutputSizer->Add(listOutputs, 1, wxEXPAND | wxALL, 1);
            listButtonOutputSizer->Add(buttonAssignOutput, 0, wxSHAPED | wxALL | wxALIGN_CENTER, 1);
            //outputsSizer->Add(listButtonOutputSizer, 0, wxEXPAND | wxALL | wxALIGN_CENTER, 1);
            //IOSizer->Add(outputsSizer, 1, wxSHAPED | wxALL | wxALIGN_CENTER, 1);
            IOSizer->Add(listButtonOutputSizer, 1, wxSHAPED | wxALL | wxALIGN_CENTER, 1);
            
            TTListSizer->Add(IOSizer, 1, wxEXPAND | wxALL | wxALIGN_CENTER, 1);
            
            wxStaticBoxSizer *RuleListSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Rule list"));
            RuleListSizer->Add(listRules, 1, wxEXPAND | wxALL, 1);
            
            wxStaticBoxSizer *FPListSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Forbidden Pattern list"));
            FPListSizer->Add(listFPs, 1, wxEXPAND | wxALL, 1);
            
        wxBoxSizer *ruleFPSizer = new wxBoxSizer(wxHORIZONTAL);
        ruleFPSizer->Add(RuleListSizer, 1, wxEXPAND | wxALL, 1);
        ruleFPSizer->Add(FPListSizer, 1, wxEXPAND | wxALL, 1);
        listsSizer->Add(TTListSizer, 1, wxEXPAND | wxALL, 1);
        listsSizer->Add(ruleFPSizer, 1, wxEXPAND | wxALL, 1);
        
            //Layouts sizer
            wxStaticBoxSizer *layoutSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Layout"));
            layoutSizer->Add(canvas, 1, wxEXPAND | wxALL | wxALIGN_CENTER, 2);
            
            //Buttons sizer
                wxBoxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);
                buttonSizer->Add(buttonNew, 0, wxEXPAND | wxALL);
                buttonSizer->Add(buttonOpen, 0, wxEXPAND | wxALL);
                buttonSizer->Add(buttonSave, 0, wxEXPAND | wxALL);
                buttonSizer->Add(buttonSaveAs, 0, wxEXPAND | wxALL);
            layoutSizer->Add(buttonSizer, 0, wxSHAPED | wxALL | wxALIGN_CENTER, 1);
            
            //Simularion sizer
            wxStaticBoxSizer *simulationSizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Simulation"));
            simulationSizer->AddStretchSpacer(1);
            simulationSizer->Add(buttonPrepareSimulation, 0, wxSHAPED | wxALIGN_CENTER, 1);
            simulationSizer->AddStretchSpacer(1);
            simulationSizer->Add(buttonStartSimulation, 0, wxSHAPED | wxALIGN_CENTER, 1);
            simulationSizer->AddStretchSpacer(1);
            
            //Right sizer
            wxBoxSizer *layoutSimulationSizer = new wxBoxSizer(wxVERTICAL);
            layoutSimulationSizer->Add(layoutSizer, 2, wxEXPAND | wxALL, 1);
            layoutSimulationSizer->Add(simulationSizer, 1, wxEXPAND | wxALL, 1);
            
        //listLayoutSizer->Add(layoutSizer, 3, wxEXPAND | wxALL, 5);
    
    mainSizer->Add(listsSizer, 1, wxEXPAND | wxALL, 1);
    mainSizer->Add(layoutSimulationSizer, 1, wxEXPAND | wxALL, 1);

    SetSizer(mainSizer);
}


//! Space canvas click event.
/*!
   \param event the event.
*/
void LayoutView::OnClick(LayoutCanvasEvent &event)
{
    controller->cellChanged(event.getX(), event.getY());
}


//! New space button click event.
/*!
   \param event the event.
*/
void LayoutView::OnNew(wxCommandEvent &event)
{
    int width;
    int height;
    int inputs;
    int outputs;
    
    //The user is asked for the information of the new space
    wxNumberEntryDialog widthDialog(this,
    wxEmptyString,
        _("Insert the layout width"),
        _("New Layout"),
        1, 1, 100);
    if (widthDialog.ShowModal() == wxID_OK)
    {
        width = (int)(widthDialog.GetValue());
    }
    else
    {
        return;
    }
    
    wxNumberEntryDialog heightDialog(this,
    wxEmptyString,
        _("Insert the layout height"),
        _("New Layout"),
        1, 1, 100);
    
    if (heightDialog.ShowModal() == wxID_OK)
    {
        height = (int)(heightDialog.GetValue());
    }
    else
    {
        return;
    }
    
    //TODO: comprovar inputs i outputs vàlids
    wxNumberEntryDialog inputsDialog(this,
    wxEmptyString,
        _("Insert the layout input number"),
        _("New Layout"),
        1, 1, 100);
    
    if (inputsDialog.ShowModal() == wxID_OK)
    {
        inputs = (int)(inputsDialog.GetValue());
    }
    else
    {
        return;
    }
    
    wxNumberEntryDialog outputsDialog(this,
    wxEmptyString,
        _("Insert the layout output number"),
        _("New Layout"),
        1, 1, 100);
    
    if (outputsDialog.ShowModal() == wxID_OK)
    {
        outputs = (int)(outputsDialog.GetValue());
        controller->newLayout(width, height, inputs, outputs);
        buttonSave->Enable(true);
        buttonSaveAs->Enable(true);
    }
    else
    {
        return;
    }
}


//! Updates the space canvas.
/*!
   \param grid the new grid.
   \param changed True if the new grid is from a new space, false if it's a
   status change of the same space.
*/
void LayoutView::updateGrids(Grid grid, bool changed)
{
    canvas->setGrid(grid, changed);
    buttonSave->Enable(true);
    buttonSaveAs->Enable(true);
}


//! Updates the truth table checklistbox
/*!
   \param tables the new table list and check status.
*/
void LayoutView::updateTTList(map<wxString, bool> tables)
{
    wxArrayString strings;
    //Add strings
    for (map<wxString, bool>::iterator i = tables.begin(); i != tables.end(); i++)
    {
        strings.Add((*i).first);
    }
    listTables->Set(strings);
    
    //Check strings
    for (int i = 0; i < listTables->GetCount(); i++)
    {
        listTables->Check(i, tables[listTables->GetString(i)]);
    }
}


//! Updates the forbidden pattern checklistbox
/*!
   \param FPs the new pattern list and check status.
*/
void LayoutView::updateFPList(map<unsigned int, bool> FPs)
{
    wxArrayString strings;
    //Add strings
    for (map<unsigned int, bool>::iterator i = FPs.begin(); i != FPs.end(); i++)
    {
        strings.Add(wxString::Format(_("Pattern %d"), (*i).first));
    }
    listFPs->Set(strings);
    
    unsigned long int temp;
    //Check strings
    for (int i = 0; i < listFPs->GetCount(); i++)
    {
        listFPs->GetString(i).Mid(8).ToULong(&temp);
        listFPs->Check(i, FPs[temp]);
    }
}


//! Updates the rule checklistbox
/*!
   \param rules the new table list and check status.
*/
void LayoutView::updateRuleList(map<unsigned int, bool> rules)
{
    wxArrayString strings;
    //Add strings
    for (map<unsigned int, bool>::iterator i = rules.begin(); i != rules.end(); i++)
    {
        strings.Add(wxString::Format(_("Rule %d"), (*i).first));
    }
    listRules->Set(strings);
    
    unsigned long int temp;
    //Check strings
    for (int i = 0; i < listRules->GetCount(); i++)
    {
        listRules->GetString(i).Mid(4).ToULong(&temp);
        listRules->Check(i, rules[temp]);
    }
}


//! Selects a truth table.
/*!
   \param table the truth table to be selected.
*/
void LayoutView::selectTable(wxString table)
{
    int item = listTables->FindString(table);
    if (item != wxNOT_FOUND)
    {
        listTables->SetSelection(item);
    }
}


//! Shows an error message.
/*!
   \param message the error message.
*/
void LayoutView::errorMsg(wxString message)
{
    wxMessageDialog dialog(this,
            message,
            _("Error"), 
            wxOK | wxICON_ERROR); 
    dialog.ShowModal();
}


//! Open space button click event.
/*!
   \param event the event.
*/
void LayoutView::OnOpen(wxCommandEvent& event)
{
    controller->openFile();
}


//! Shows a yes/no selection dialog.
/*!
   \param message the question message.
   \return True if the user selected yes, false otherwise.
*/
bool LayoutView::msgYesNo(wxString message)
{
    wxMessageDialog dialog(this,
            message,
            _("Question"), 
            wxYES | wxNO); 
    return (dialog.ShowModal() == wxID_YES);
}


//! Save button click event.
/*!
   \param event the event.
*/
void LayoutView::OnSave(wxCommandEvent& event)
{
    controller->saveFile();
}


//! Save As button click event.
/*!
   \param event the event.
*/
void LayoutView::OnSaveAs(wxCommandEvent& event)
{
    controller->saveFileAs();
}


//! Truth table checklistbox selection event.
/*!
   \param event the event.
*/
void LayoutView::OnTableSelected(wxCommandEvent &event)
{
    if (event.IsSelection())
    {
       controller->tableSelected(event.GetString());
    }
}


//! Truth table checklistbox check event.
/*!
   \param event the event.
*/
void LayoutView::OnTableChecked(wxCommandEvent &event)
{
   controller->tableChecked(listTables->GetString(event.GetSelection()), listTables->IsChecked(event.GetSelection()));
}


//! Updates the input listbox.
/*!
   \param strings the string list.
*/
void LayoutView::updateInputList(wxArrayString strings)
{
    listInputs->Set(strings);
    buttonAssignInput->Enable(strings.size() > 0);
    if (strings.size() > 0)
    {
        listInputs->SetSelection(0);
    }
}


//! Updates the output listbox.
/*!
   \param strings the string list.
*/
void LayoutView::updateOutputList(wxArrayString strings)
{
    listOutputs->Set(strings);
    buttonAssignOutput->Enable(strings.size() > 0);
    if (strings.size() > 0)
    {
        listOutputs->SetSelection(0);
    }
}


//! Assign input button click event.
/*!
   \param event the event.
*/
void LayoutView::OnAssignInput(wxCommandEvent &event)
{
    canvas->setVisible(false, false, true, false, false);
    onlyAssign(false);
    controller->assignInput();
    //This needs to be done for the escape event
    SetFocusIgnoringChildren();
}


//! Assign output button click event.
/*!
   \param event the event.
*/
void LayoutView::OnAssignOutput(wxCommandEvent &event)
{
    canvas->setVisible(false, false, false, true, false);
    onlyAssign(false);
    controller->assignOutput();
    //This needs to be done for the escape event
    SetFocusIgnoringChildren();
}


//! Customizes the view for the assigning mode.
/*!
   Disables everything but the canvas window and sets it
   to show only inputs or outputs.
   \param enable true if the assigning mode has to be set, false if it has to
   be unset.
*/
void LayoutView::onlyAssign(bool enable)
{
    buttonNew->Enable(enable);
    buttonOpen->Enable(enable);
    buttonSave->Enable(enable);
    buttonSaveAs->Enable(enable);
    buttonAssignInput->Enable(enable);
    buttonAssignOutput->Enable(enable);
    listTables->Enable(enable);
    listRules->Enable(enable);
    listFPs->Enable(enable);
    listInputs->Enable(enable);
    listOutputs->Enable(enable);
    
    if (enable)
    {
        canvas->SetCursor(*wxSTANDARD_CURSOR);
        canvas->setVisible(true, true, true, true, true);
    }
    else
    {
        canvas->SetCursor(*wxCROSS_CURSOR);
    }
}


//! Key press event.
/*!
   \param event the event.
*/
void LayoutView::OnKeyPressed(wxKeyEvent &event)
{
    //The escape key is used to escape the assigning mode
    if (event.GetKeyCode() == WXK_ESCAPE)
    {
        controller->escapePressed();
    }
}


//! Input listbox selection event.
/*!
   \param event the event.
*/
void LayoutView::OnInputSelected(wxCommandEvent &event)
{
    if (event.IsSelection())
    {
        unsigned long int temp;
        event.GetString().Mid(6).ToULong(&temp);
       controller->inputSelected(temp - 1);
    }
}


//! Output listbox selection event.
/*!
   \param event the event.
*/
void LayoutView::OnOutputSelected(wxCommandEvent &event)
{
    if (event.IsSelection())
    {
       unsigned long int temp;
        event.GetString().Mid(7).ToULong(&temp);
       controller->outputSelected(temp - 1);
    }
}


//! Refreshes the canvas window.
void LayoutView::updateCanvas()
{
    canvas->drawScene();
}


//! Sets the assigned inputs on the canvas.
/*!
   \param inputs the assigned inputs.
*/
void LayoutView::updateInputs(vector<coordinate> inputs)
{
    canvas->setAssignedInputs(inputs);
}


//! Sets the assigned outputs on the canvas.
/*!
   \param outputs the assigned outputs.
*/
void LayoutView::updateOutputs(vector<coordinate> outputs)
{
    canvas->setAssignedOutputs(outputs);
}


//! Rule checklistbox check event.
/*!
   \param event the event.
*/
void LayoutView::OnRuleChecked(wxCommandEvent &event)
{
    unsigned long int temp;
    wxString selection = listRules->GetString(event.GetSelection());
    selection.Mid(4).ToULong(&temp);
    controller->ruleChecked(temp, listRules->IsChecked(event.GetSelection()));
}


//! Forbidden pattern checklistbox check event.
/*!
   \param event the event.
*/
void LayoutView::OnFPChecked(wxCommandEvent &event)
{
   unsigned long int temp;
    wxString selection = listFPs->GetString(event.GetSelection());
    selection.Mid(8).ToULong(&temp);
   controller->FPChecked(temp, listFPs->IsChecked(event.GetSelection()));
}


//! Checks or unchecks a truth table.
/*!
   \param table the truth table.
   \param enabled true if the table has to be checked, false if it has to be
   unchecked.
*/
void LayoutView::checkTable(wxString table, bool enabled)
{
    listTables->Check(listTables->FindString(table), enabled);
}


//! Checks or unchecks a rule.
/*!
   \param rule the rule.
   \param enabled true if the rule has to be checked, false if it has to be
   unchecked.
*/
void LayoutView::checkRule(wxString rule, bool enabled)
{
    listRules->Check(listRules->FindString(rule), enabled);
}


//! Checks or unchecks a pattern.
/*!
   \param FP the pattern.
   \param enabled true if the pattern has to be checked, false if it has to be
   unchecked.
*/
void LayoutView::checkFP(wxString FP, bool enabled)
{
    listFPs->Check(listFPs->FindString(FP), enabled);
}


//! Prepare simulation button click event.
/*!
   \param event the event.
*/
void LayoutView::OnPrepare(wxCommandEvent &event)
{
    controller->prepareSimulation();
}


//! Start simulation button click event.
/*!
   \param event the event.
*/
void LayoutView::OnStart(wxCommandEvent &event)
{
    controller->startSimulation();
}


//! Enables or disables the prepare simulation button.
/*!
   \param enable true if the button has to be enabled, false if it has to be
   disabled.
*/
void LayoutView::enableCheckSimulation(bool enable)
{
    buttonPrepareSimulation->Enable(enable);
}


//! Enables or disables the start simulation button.
/*!
   \param enable true if the button has to be enabled, false if it has to be
   disabled.
*/
void LayoutView::enableStartSimulation(bool enable)
{
    buttonStartSimulation->Enable(enable);
}
