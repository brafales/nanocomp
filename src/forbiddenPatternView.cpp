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
// $Revision: 1.21 $

#include "forbiddenPatternView.hpp"
#include <wx/statline.h>
#include "resources/cross.xpm"
#include "resources/fileopen.xpm"
#include "resources/filesave.xpm"
#include "resources/filesaveas.xpm"
#include "resources/new.xpm"

//! Widgets enum.
enum
{
    ID_BUTTON_NEW = 0,
    ID_BUTTON_OPEN,
    ID_BUTTON_SAVE,
    ID_BUTTON_SAVEAS,
    ID_BUTTON_REMOVE,
    ID_BUTTON_CHECK,
    ID_LISTFP,
    ID_CANVAS
};


BEGIN_EVENT_TABLE(ForbiddenPatternView, wxPanel)
    EVT_LAYOUT_CANVAS  (ID_CANVAS,  ForbiddenPatternView::OnClick)
    EVT_BUTTON  (ID_BUTTON_NEW, ForbiddenPatternView::OnNew)
    EVT_LISTBOX (ID_LISTFP, ForbiddenPatternView::OnSelection)
    EVT_BUTTON  (ID_BUTTON_REMOVE, ForbiddenPatternView::OnRemove)
    EVT_BUTTON  (ID_BUTTON_SAVE, ForbiddenPatternView::OnSave)
    EVT_BUTTON  (ID_BUTTON_SAVEAS, ForbiddenPatternView::OnSaveAs)
    EVT_BUTTON  (ID_BUTTON_OPEN, ForbiddenPatternView::OnOpen)
        EVT_BUTTON  (ID_BUTTON_CHECK, ForbiddenPatternView::OnCheck)
END_EVENT_TABLE()


//! Constructor.
/*!
  \param parent the parent window.
  \param id the window identifier.
  \param controller the forbidden pattern controller.
*/
ForbiddenPatternView::ForbiddenPatternView(wxWindow* parent, wxWindowID id, ForbiddenPatternManager *controller)
    :wxPanel(parent, id)
{
    this->controller = controller;
    initControls();
    initSizers();
    controller->setView(this);
}


//! Destructor.
ForbiddenPatternView::~ForbiddenPatternView()
{
}


//! Window controls initialization.
/*!
    All the window controls are initialized in this method.
*/
void ForbiddenPatternView::initControls()
{
    //Buttons
    wxBitmap newBitmap(wxBITMAP(new));
    wxBitmap saveBitmap(wxBITMAP(filesave));
    wxBitmap saveAsBitmap(wxBITMAP(filesaveas));
    wxBitmap removeBitmap(wxBITMAP(cross));
    wxBitmap openBitmap(wxBITMAP(fileopen));
    buttonNew = new wxBitmapButton(this, ID_BUTTON_NEW, newBitmap);
    buttonNew->SetToolTip(_("Add a new forbidden pattern"));
    buttonOpen = new wxBitmapButton(this, ID_BUTTON_OPEN, openBitmap);
    buttonOpen->SetToolTip(_("Open a forbidden pattern file"));
    buttonSave = new wxBitmapButton(this, ID_BUTTON_SAVE, saveBitmap);
    buttonSave->SetToolTip(_("Save selected forbidden pattern to a file"));
    buttonSaveAs = new wxBitmapButton(this, ID_BUTTON_SAVEAS, saveAsBitmap);
    buttonSaveAs->SetToolTip(_("Save selected forbidden pattern to a new file"));
    buttonRemove = new wxBitmapButton(this, ID_BUTTON_REMOVE, removeBitmap);
    buttonRemove->SetToolTip(_("Delete selected forbidden pattern"));
    buttonSave->Enable(false);
    buttonSaveAs->Enable(false);
    buttonRemove->Enable(false);
    buttonCheck = new wxButton(this, ID_BUTTON_CHECK, _("Check patterns"));
    
    //Other controls
    listFP = new wxListBox(this, ID_LISTFP);
    canvas = new LayoutCanvas(this, ID_CANVAS, Grid());
}

//! Sizers initialization.
/*!
    All the sizers (the window layout) is done in this method.
*/
void ForbiddenPatternView::initSizers()
{
    //Main Sizer
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    //mainSizer->Add(labelTitle, 0, wxEXPAND | wxALL, 2);
    
        //List and layout sizer
        wxBoxSizer *listLayoutSizer = new wxBoxSizer(wxHORIZONTAL);
            
            //List sizer
            wxStaticBoxSizer *listVerticalSizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Pattern list"));
            wxBoxSizer *listSizer = new wxBoxSizer(wxHORIZONTAL);
            listSizer->Add(listFP, 1, wxEXPAND | wxALL, 1);
            
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
            wxStaticBoxSizer *layoutSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Layout"));
            layoutSizer->Add(canvas, 1, wxEXPAND | wxALL | wxALIGN_CENTER, 1);
            
        //listLayoutSizer->AddStretchSpacer();
        listLayoutSizer->Add(layoutSizer, 3, wxEXPAND | wxALL, 1);
        
    mainSizer->Add(listLayoutSizer, 3, wxEXPAND | wxALL, 1);

    SetSizer(mainSizer);
}


//! Canvas click event.
/*!
   Method called when a click event on the layout canvas is emitted.
  \param event the event.
*/
void ForbiddenPatternView::OnClick(LayoutCanvasEvent &event)
{
    controller->cellChanged(event.getX(), event.getY());
}


//! New button event.
/*!
   Method called when the user clicks on the new button.
  \param event the event.
*/
void ForbiddenPatternView::OnNew(wxCommandEvent &event)
{
    //The user is asked for the width and height of the new pattern
    //and then the controller is called to do the actual work.
    int width;
    int height;
    wxNumberEntryDialog widthDialog(this,
                        wxEmptyString,
                        _("Insert the layout width"),
                        _("New Forbidden Pattern"),
                        1, 1, 100);
    if (widthDialog.ShowModal() == wxID_OK)
    {
        width = (int)(widthDialog.GetValue());
    
        wxNumberEntryDialog heightDialog(this,
                        wxEmptyString,
                        _("Insert the layout height"),
                        _("New ForbiddenPattern"),
                        1, 1, 100);
        if (heightDialog.ShowModal() == wxID_OK)
        {
            height = (int)(heightDialog.GetValue());
            controller->newFP(width, height);
        }
    }
    
}


//! Pattern selection event.
/*!
   Method called when the user selects a pattern.
  \param event the event.
*/
void ForbiddenPatternView::OnSelection(wxCommandEvent &event)
{
    //New FP selected, we must let the controller know
    //to update us with new info
    if (event.IsSelection())
    {
        wxString selection = event.GetString();
        unsigned long int temp;
        //TODO: this does not work when the names are
        //with the repeated information.
        selection.BeforeFirst('-').Mid(7).ToULong(&temp);
        unsigned int FPId = temp;
        controller->FPSelected(FPId);
        buttonRemove->Enable(true);
        buttonSave->Enable(true);
        buttonSaveAs->Enable(true);
    }
}


//! Remove button event.
/*!
   Method called when the remove button is clicked.
  \param event the event.
*/
void ForbiddenPatternView::OnRemove(wxCommandEvent &event)
{
    controller->removeFP();
}


//! Grid update method.
/*!
   This method updates the canvas of the pattern grid.
  \param grid the grid to be shown.
  \param changed true if the grid is of the same pattern or another one.
*/
void ForbiddenPatternView::updateGrids(Grid grid, bool changed)
{
    canvas->setGrid(grid, changed);
}


//! Sets the items of the pattern listbox.
/*!
   Method called when the remove button is clicked.
  \param strings the items to be listed.
*/
void ForbiddenPatternView::updateList(wxArrayString strings)
{
    listFP->Set(strings);
    //Enable/Disable modify buttons if the list is (not) empty
    buttonRemove->Enable(strings.size() > 0);
    buttonSave->Enable(strings.size() > 0);
    buttonSaveAs->Enable(strings.size() > 0);
}


//! Selects a pattern.
/*!
   Selects the pattern on the listbox.
  \param FP the pattern identifier to be selected.
*/
void ForbiddenPatternView::selectFP(unsigned int FP)
{
    int item = listFP->FindString(wxString::Format(_("Pattern %d"), FP));
    if (item != wxNOT_FOUND)
    {
        listFP->SetSelection(item);
        buttonRemove->Enable(true);
        buttonSave->Enable(true);
        buttonSaveAs->Enable(true);
    }
}


//! Save button event.
/*!
   Method called when the save button is clicked.
  \param event the event.
*/
void ForbiddenPatternView::OnSave(wxCommandEvent& event)
{
    controller->saveFile();
}


//! Save As button event.
/*!
   Method called when the save as button is clicked.
  \param event the event.
*/
void ForbiddenPatternView::OnSaveAs(wxCommandEvent& event)
{
    controller->saveFileAs();
}


//! Open button event.
/*!
   Method called when the open button is clicked.
  \param event the event.
*/
void ForbiddenPatternView::OnOpen(wxCommandEvent& event)
{
    controller->openFile();
}


//! User selection method.
/*!
   The user is asked for a yes/no selection.
  \param message the question to be asked.
  \return True if the user selected Yes, false otherwise.
*/
bool ForbiddenPatternView::msgYesNo(wxString message)
{
    wxMessageDialog dialog(this,
            message,
            _("Question"), 
            wxYES | wxNO); 
    return (dialog.ShowModal() == wxID_YES);
}


//! Shows an error message.
/*!
  \param message the error message to be shown.
*/
void ForbiddenPatternView::errorMsg(wxString message)
{
    wxMessageDialog dialog(this,
            message,
            _("Error"), 
            wxOK | wxICON_ERROR); 
    dialog.ShowModal();
}


//! Check button event.
/*!
   Method called when the check button is clicked.
  \param event the event.
*/
void ForbiddenPatternView::OnCheck(wxCommandEvent& event)
{
    controller->checkPatterns();
}


//! Set a string for an item of the listbox.
/*!
   Allows to set a specific string to an item of the listbox.
  \param id the index of the item of the listbox.
  \param message the string to be set on the item.
*/
void ForbiddenPatternView::setListMessage(int id, wxString message)
{
    int index = findIndex(id);
    if (index >= 0)
    {
        listFP->SetString(index, listFP->GetString(index) + message);
    }
}


//! Sets the default string to a listbox item.
/*!
   Resets the default message to be shown on an item of the listbox.
  \param id the index of the item to be reset.
*/
void ForbiddenPatternView::resetListItem(int id)
{
    int index = findIndex(id);
    if (index >= 0)
    {
        listFP->SetString(index, wxString::Format(_("Pattern %d"), id));
    }
}


//! Finds the listbox index of a given pattern.
/*!
  \param index the pattern identifier.
  \return The listbox index of the pattern.
*/
int ForbiddenPatternView::findIndex(int index)
{
    int result = -1;
    for (int i = 0; i < listFP->GetCount(); i++)
    {
        unsigned long int temp;
        wxString item = listFP->GetString(i);
        item.BeforeFirst('-').Mid(7).ToULong(&temp);
        if ((int)temp == index)
        {
            return i;
        }
    }
    return result;
}
