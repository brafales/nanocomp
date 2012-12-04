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
// $Revision: 1.27 $

#include "ruleView.hpp"
#include <wx/numdlg.h>
#include <wx/statline.h>
#include "resources/forward.xpm"
#include "resources/cross.xpm"
#include "resources/fileopen.xpm"
#include "resources/filesave.xpm"
#include "resources/filesaveas.xpm"
#include "resources/new.xpm"

enum
{
    ID_BUTTON_NEW = 0,
    ID_BUTTON_OPEN,
    ID_BUTTON_SAVE,
    ID_BUTTON_REMOVE,
    ID_BUTTON_SAVEALL,
    ID_BUTTON_SAVEAS,
    ID_BUTTON_COPY,
    ID_BUTTON_CHECK,
    ID_LISTRULES,
    ID_CANVAS_INITIAL,
    ID_CANVAS_FINAL
};


BEGIN_EVENT_TABLE(RuleView, wxPanel)
    EVT_LAYOUT_CANVAS  (ID_CANVAS_INITIAL,  RuleView::OnClick)
    EVT_LAYOUT_CANVAS  (ID_CANVAS_FINAL,  RuleView::OnClick)
    EVT_BUTTON  (ID_BUTTON_NEW, RuleView::OnNewRule)
    EVT_LISTBOX (ID_LISTRULES, RuleView::OnSelection)
    EVT_BUTTON  (ID_BUTTON_REMOVE, RuleView::OnRemove)
    EVT_BUTTON  (ID_BUTTON_COPY, RuleView::OnCopy)
    EVT_BUTTON  (ID_BUTTON_SAVE, RuleView::OnSave)
    EVT_BUTTON  (ID_BUTTON_SAVEAS, RuleView::OnSaveAs)
    EVT_BUTTON  (ID_BUTTON_OPEN, RuleView::OnOpen)
    EVT_BUTTON  (ID_BUTTON_CHECK, RuleView::OnCheck)
END_EVENT_TABLE()


//! Constructor.
/*!
   \param parent the parent window.
   \param id the window identifier.
   \param controller the rule controller.
*/
RuleView::RuleView(wxWindow* parent, wxWindowID id, RuleManager *controller)
    :wxPanel(parent, id)
{
    this->controller = controller;
    initControls();
    initSizers();
    controller->setView(this);
}


//! Destructor.
RuleView::~RuleView()
{
}


//! Control initialization.
void RuleView::initControls()
{
    //Labels
    labelInitialLayout = new wxStaticText(this, wxID_ANY, _("Initial layout"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    labelFinalLayout = new wxStaticText(this, wxID_ANY, _("Final layout"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    
    //Buttons
    wxBitmap newBitmap(wxBITMAP(new));
    wxBitmap saveBitmap(wxBITMAP(filesave));
    wxBitmap saveAsBitmap(wxBITMAP(filesaveas));
    wxBitmap removeBitmap(wxBITMAP(cross));
    wxBitmap openBitmap(wxBITMAP(fileopen));
    buttonNew = new wxBitmapButton(this, ID_BUTTON_NEW, newBitmap);
    buttonNew->SetToolTip(_("Add a new rule"));
    buttonOpen = new wxBitmapButton(this, ID_BUTTON_OPEN, openBitmap);
    buttonOpen->SetToolTip(_("Open a rule file"));
    buttonSave = new wxBitmapButton(this, ID_BUTTON_SAVE, saveBitmap);
    buttonSave->SetToolTip(_("Save rules into a file"));
    buttonSaveAs = new wxBitmapButton(this, ID_BUTTON_SAVEAS, saveAsBitmap);
    buttonSave->SetToolTip(_("Save rules into a new file"));
    buttonRemove = new wxBitmapButton(this, ID_BUTTON_REMOVE, removeBitmap);
    buttonRemove->SetToolTip(_("Delete selected rule"));
    buttonSave->Enable(false);
    buttonSaveAs->Enable(false);
    buttonRemove->Enable(false);
    wxBitmap copyBitmap(wxBITMAP(forward));
    buttonCopy = new wxBitmapButton(this, ID_BUTTON_COPY, copyBitmap);
    buttonCopy->SetToolTip(_("Copy initial layout to final layout"));
    buttonCheck = new wxButton(this, ID_BUTTON_CHECK, _("Check Rules"));
    
    //Other controls
    listRules = new wxListBox(this, ID_LISTRULES, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE);
    canvasInitial = new LayoutCanvas(this, ID_CANVAS_INITIAL, Grid());
    canvasFinal = new LayoutCanvas(this, ID_CANVAS_FINAL, Grid());
 }


//! Sizers (layout) initialization.
void RuleView::initSizers()
{
    //Main Sizer
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    
        //List and layout sizer
        wxBoxSizer *listLayoutSizer = new wxBoxSizer(wxHORIZONTAL);
            
            //List sizer
            wxStaticBoxSizer *listVerticalSizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Rule list"));
            wxBoxSizer *listSizer = new wxBoxSizer(wxHORIZONTAL);
            listSizer->Add(listRules, 1, wxEXPAND | wxALL, 1);
            
                //Buttons sizer
                wxBoxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);
                buttonSizer->Add(buttonNew, 0, wxEXPAND | wxALL);
                buttonSizer->Add(buttonOpen, 0, wxEXPAND | wxALL);
                buttonSizer->Add(buttonRemove, 0, wxEXPAND | wxALL);
                buttonSizer->Add(buttonSave, 0, wxEXPAND | wxALL);
                buttonSizer->Add(buttonSaveAs, 0, wxEXPAND | wxALL);
                
            listSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 1);
        
        listVerticalSizer->Add(listSizer, 1, wxEXPAND | wxALL, 1);

        //Check button sizer
        wxBoxSizer *checkButtonSizer = new wxBoxSizer(wxVERTICAL);
        checkButtonSizer->Add(buttonCheck, 0, wxSHAPED | wxALIGN_CENTER, 1);
            
        listVerticalSizer->Add(checkButtonSizer, 0, wxSHAPED | wxALIGN_CENTER, 1);
        
        listLayoutSizer->Add(listVerticalSizer, 2, wxEXPAND | wxALL, 1);
        
            //Layouts sizer
            wxStaticBoxSizer *layoutSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Layouts"));
            wxBoxSizer *initialLayoutSizer = new wxBoxSizer(wxVERTICAL);
            wxBoxSizer *finalLayoutSizer = new wxBoxSizer(wxVERTICAL);
            initialLayoutSizer->Add(labelInitialLayout, 0, wxEXPAND | wxALL | wxALIGN_CENTER, 0);
            initialLayoutSizer->Add(canvasInitial, 1, wxEXPAND | wxALL | wxALIGN_CENTER, 1);
            layoutSizer->Add(initialLayoutSizer, 1, wxSHAPED | wxALL | wxALIGN_CENTER, 1);
            layoutSizer->Add(buttonCopy, 0, wxSHAPED | wxALL | wxALIGN_CENTER, 0);
            finalLayoutSizer->Add(labelFinalLayout, 0, wxEXPAND | wxALL | wxALIGN_CENTER, 0);
            finalLayoutSizer->Add(canvasFinal, 1, wxEXPAND | wxALL | wxALIGN_CENTER, 1);
            layoutSizer->Add(finalLayoutSizer, 1, wxSHAPED | wxALL | wxALIGN_CENTER, 1);
            
        listLayoutSizer->Add(layoutSizer, 3, wxEXPAND | wxALL, 1);
        
    mainSizer->Add(listLayoutSizer, 3, wxEXPAND | wxALL, 1);
        
    SetSizer(mainSizer);
}


//! LayoutCanvas click event method.
/*!
   The event canvas is identified and passed as a parameter
   to the controller.
   \param event the event.
*/
void RuleView::OnClick(LayoutCanvasEvent &event)
{
    switch (event.GetId())
    {
        case ID_CANVAS_INITIAL:
        {
            controller->cellChanged(nINITIAL, event.getX(), event.getY());
            break;
        }
        case ID_CANVAS_FINAL:
        {
            controller->cellChanged(nFINAL, event.getX(), event.getY());
            break;
        }
    }
}


//! New rule button click method.
/*!
   \param event the event.
*/
void RuleView::OnNewRule(wxCommandEvent& event)
{
    int width;
    int height;
    wxNumberEntryDialog widthDialog(this,
                        wxEmptyString,
                        _("Insert the layout width"),
                        _("New Rule"),
                        1, 1, 100);
    if (widthDialog.ShowModal() == wxID_OK)
    {
        width = (int)(widthDialog.GetValue());
    
        wxNumberEntryDialog heightDialog(this,
                        wxEmptyString,
                        _("Insert the layout height"),
                        _("New Rule"),
                        1, 1, 100);
        if (heightDialog.ShowModal() == wxID_OK)
        {
            height = (int)(heightDialog.GetValue());
            controller->newRule(width, height);
        }
    }
}


//! Updates the grids of the canvas.
/*!
   \param initial the grid for the initial layout.
   \param final the grid for the final layout.
   \param changed if true, cell transitions are disabled.
*/
void RuleView::updateGrids(Grid initial, Grid final, bool changed)
{
    canvasInitial->setGrid(initial, changed);
    canvasFinal->setGrid(final, changed);
    //ONLY FOR TESTING LALALALALALA
    /*
    if ((initial.getHeight() > 4) && (initial.getWidth() > 4))
    {
        canvasInitial->addRectangle(1, 1, 3, 3);
    }
    */
}


//! Updates the rule listbox.
/*!
   \param strings the string array.
*/
void RuleView::updateList(wxArrayString strings)
{
    listRules->Set(strings);
    buttonRemove->Enable(strings.size() > 0);
    buttonSave->Enable(strings.size() > 0);
    buttonSaveAs->Enable(strings.size() > 0);
}


//! Listbox selection method.
/*!
   \param event the event.
*/
void RuleView::OnSelection(wxCommandEvent &event)
{
    //New rule selected, we must let the controller know
    //to update us with new info
    if (event.IsSelection())
    {
        wxString selection = event.GetString();
        unsigned long int temp;
        selection.Mid(4).ToULong(&temp);
        unsigned int ruleId = temp;
        controller->ruleSelected(ruleId);
        buttonRemove->Enable(true);
        buttonSave->Enable(true);
    }
}


//! Remove rule button click method.
/*!
   \param event the event.
*/
void RuleView::OnRemove(wxCommandEvent &event)
{
    controller->removeRule();
}


//! Selects a rule from the listbox.
/*!
   \param rule the rule identifier.
*/
void RuleView::selectRule(unsigned int rule)
{
    int item = listRules->FindString(wxString::Format(_("Rule %d"), rule));
    if (item != wxNOT_FOUND)
    {
        listRules->SetSelection(item);
        buttonRemove->Enable(true);
        buttonSave->Enable(true);
    }
}


//! Copy grid button click method.
/*!
   \param event the event.
*/
void RuleView::OnCopy(wxCommandEvent& event)
{
    controller->copyGrids();  
}


//! Save button click method.
/*!
   \param event the event.
*/
void RuleView::OnSave(wxCommandEvent& event)
{
    controller->saveFile();
}


//! Save as button click method.
/*!
   \param event the event.
*/
void RuleView::OnSaveAs(wxCommandEvent& event)
{
    controller->saveFileAs();
}


//! Shows an error message.
/*!
   \param message the error message.
*/
void RuleView::errorMsg(wxString message)
{
    wxMessageDialog dialog(this,
            message,
            _("Error"), 
            wxOK | wxICON_ERROR); 
    dialog.ShowModal();
}


//! Open button click method.
/*!
   \param event the event.
*/
void RuleView::OnOpen(wxCommandEvent& event)
{
    controller->openFile();
}


//! Shows a yes/no selection dialog.
/*!
   \param message the question to be asked.
   \return True if the user selectes yes, false otherwise.
*/
bool RuleView::msgYesNo(wxString message)
{
    wxMessageDialog dialog(this,
            message,
            _("Question"), 
            wxYES | wxNO); 
    return (dialog.ShowModal() == wxID_YES);
}


//! Check rules button click method.
/*!
   \param event the event.
*/
void RuleView::OnCheck(wxCommandEvent& event)
{
    controller->checkRules();
}


//! Sets a special message for a rule in a listbox.
/*!
   \param id the rule identifier.
   \param message the special message to add.
*/
void RuleView::setListMessage(int id, wxString message)
{
    int index = findIndex(id);
    if (index >= 0)
    {
        listRules->SetString(index, listRules->GetString(index) + message);
    }
}


//! Sets the default message for a listbox item.
/*!
   \param id the rule identifier.
*/
void RuleView::resetListItem(int id)
{
    int index = findIndex(id);
    if (index >= 0)
    {
        listRules->SetString(index, wxString::Format(_("Rule %d"), id));
    }
}


//! Finds the listbox index of a given rule.
/*!
  \param index the rule identifier.
  \return The listbox index of the rule.
*/
int RuleView::findIndex(int index)
{
    int result = -1;
    for (int i = 0; i < listRules->GetCount(); i++)
    {
        unsigned long int temp;
        wxString item = listRules->GetString(i);
        item.BeforeFirst('-').Mid(4).ToULong(&temp);
        if ((int)temp == index)
        {
            return i;
        }
    }
    return result;
}
