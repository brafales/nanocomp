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
// $Revision: 1.10 $

#include "truthTableView.hpp"
#include "truthTableCanvas.hpp"

#include <wx/statline.h>
#include "resources/cross.xpm"
#include "resources/fileopen.xpm"
#include "resources/filesave.xpm"
#include "resources/filesaveas.xpm"
#include "resources/new.xpm"

enum
{
    ID_TEXT_NAME = 0,
    ID_TEXT_DESCRIPTION,
    ID_BUTTON_NEW,
    ID_BUTTON_OPEN,
    ID_BUTTON_SAVE,
    ID_BUTTON_SAVEAS,
    ID_BUTTON_REMOVE,
    ID_LIST_TABLES,
    ID_CANVAS
};


BEGIN_EVENT_TABLE(TruthTableView, wxPanel)
    EVT_TRUTH_TABLE_CANVAS  (ID_CANVAS,  TruthTableView::OnClick)
    EVT_BUTTON  (ID_BUTTON_NEW, TruthTableView::OnNew)
    EVT_LISTBOX (ID_LIST_TABLES, TruthTableView::OnSelection)
    EVT_BUTTON  (ID_BUTTON_REMOVE, TruthTableView::OnRemove)
    EVT_BUTTON  (ID_BUTTON_OPEN, TruthTableView::OnOpen)
    EVT_BUTTON  (ID_BUTTON_SAVE, TruthTableView::OnSave)
    EVT_BUTTON  (ID_BUTTON_SAVEAS, TruthTableView::OnSaveAs)
END_EVENT_TABLE()


//! Constructor.
/*!
   \param parent the parent window.
   \param id the window identifier.
   \param controller the truth table controller.
*/
TruthTableView::TruthTableView(wxWindow* parent, wxWindowID id, TruthTableManager *controller)
    :wxPanel(parent, id)
{
    this->controller = controller;
    initControls();
    initSizers();
    controller->setView(this);

    labelInfo->Show(false);
}


//! Destructor
TruthTableView::~TruthTableView()
{
}


//! Control initialization.
void TruthTableView::initControls()
{
    //Labels
    labelInfo = new wxStaticText(this, wxID_ANY, _("Click on an output value to change it"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    
    //Buttons
    wxBitmap newBitmap(wxBITMAP(new));
    wxBitmap saveBitmap(wxBITMAP(filesave));
    wxBitmap saveAsBitmap(wxBITMAP(filesaveas));
    wxBitmap removeBitmap(wxBITMAP(cross));
    wxBitmap openBitmap(wxBITMAP(fileopen));
    buttonNew = new wxBitmapButton(this, ID_BUTTON_NEW, newBitmap);
    buttonNew->SetToolTip(_("Add a new truth table"));
    buttonOpen = new wxBitmapButton(this, ID_BUTTON_OPEN, openBitmap);
    buttonOpen->SetToolTip(_("Open a truth table file"));
    buttonSave = new wxBitmapButton(this, ID_BUTTON_SAVE, saveBitmap);
    buttonSave->SetToolTip(_("Save selected truth table to a file"));
    buttonSaveAs = new wxBitmapButton(this, ID_BUTTON_SAVEAS, saveAsBitmap);
    buttonSaveAs->SetToolTip(_("Save selected truth table to a new file"));
    buttonRemove = new wxBitmapButton(this, ID_BUTTON_REMOVE, removeBitmap);
    buttonRemove->SetToolTip(_("Delete selected truth table"));
    buttonSave->Enable(false);
    buttonSaveAs->Enable(false);
    buttonRemove->Enable(false);
    
    //Other controls
    listTables = new wxListBox(this, ID_LIST_TABLES);
    canvas = new TruthTableCanvas(this, ID_CANVAS, TruthTable());
}


//! Sizer (layout) initialization.
void TruthTableView::initSizers()
{
    //Main Sizer
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    //mainSizer->Add(labelTitle, 0, wxEXPAND | wxALL, 2);
    
        //Line separator
        //wxBoxSizer *separatorSizer = new wxBoxSizer(wxHORIZONTAL);
        //separatorSizer->AddStretchSpacer();
        //separatorSizer->Add(new wxStaticLine(this), 2, wxEXPAND | wxALL);
        //separatorSizer->AddStretchSpacer();
    
    //mainSizer->Add(separatorSizer, 0, wxEXPAND | wxALL);
    
        //List and table sizer
        wxBoxSizer *listTableSizer = new wxBoxSizer(wxHORIZONTAL);
            
            //List sizer
            wxStaticBoxSizer *listSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Truth Table list"));
            listSizer->Add(listTables, 1, wxEXPAND | wxALL, 2);
            
                //Buttons sizer
                wxBoxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);
                buttonSizer->Add(buttonNew, 0, wxEXPAND | wxALL);
                buttonSizer->Add(buttonOpen, 0, wxEXPAND | wxALL);
                buttonSizer->Add(buttonRemove, 0, wxEXPAND | wxALL);
                buttonSizer->Add(buttonSave, 0, wxEXPAND | wxALL);
                buttonSizer->Add(buttonSaveAs, 0, wxEXPAND | wxALL);
                
            listSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 2);
            
        listTableSizer->Add(listSizer, 1, wxEXPAND | wxALL, 2);
        
            //Table sizer
            wxStaticBoxSizer *tableSizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Truth Table"));
            tableSizer->Add(canvas, 1, wxSHAPED | wxALL | wxALIGN_CENTER, 2);
            tableSizer->Add(labelInfo, 0, wxSHAPED | wxALL | wxALIGN_CENTER, 2);
            
        //listLayoutSizer->AddStretchSpacer();
        listTableSizer->Add(tableSizer, 3, wxEXPAND | wxALL, 2);
        
    mainSizer->Add(listTableSizer, 3, wxEXPAND | wxALL, 2);
        
    SetSizer(mainSizer);
}


//! TruthTableCanvas click event method.
/*!
   \param event the event.
*/
void TruthTableView::OnClick(TruthTableCanvasEvent &event)
{
    controller->tableChanged(event.getInput(), event.getOutput());
}


//! New table button click event method.
/*!
   \param event the event.
*/
void TruthTableView::OnNew(wxCommandEvent &event)
{
    wxString name;
    int inputs;
    int outputs;
    
    wxTextEntryDialog nameDialog(this,
        _("Insert the name of the table"),
        _("New Truth Table"));
                        
    if (nameDialog.ShowModal() == wxID_OK)
    {
        name = nameDialog.GetValue();
        if (name.IsEmpty())
        {
            wxMessageDialog dialog(this,
            _("You must type a non empty name"),
            _("Error"), 
            wxOK | wxICON_ERROR);
            dialog.ShowModal();
            return;
        }
    }
    else
    {
        return;
    }
                        
    wxNumberEntryDialog inputDialog(this,
    wxEmptyString,
        _("Insert the number of inputs"),
        _("New Truth Table"),
        1, 1, 100);
    if (inputDialog.ShowModal() == wxID_OK)
    {
        inputs = (int)(inputDialog.GetValue());
    }
    else
    {
        return;
    }
    
    wxNumberEntryDialog outputDialog(this,
    wxEmptyString,
        _("Insert the number of outputs"),
        _("New Truth Table"),
        1, 1, 100);
    
    if (outputDialog.ShowModal() == wxID_OK)
    {
        outputs = (int)(outputDialog.GetValue());
            

        controller->newTable(name, inputs, outputs);
    }
    
}


//! Listbox selection event method.
/*!
   \param event the event.
*/
void TruthTableView::OnSelection(wxCommandEvent &event)
{
    //New TT selected, we must let the controller know
    //to update us with new info
    if (event.IsSelection())
    {
        wxString selection = event.GetString();
        controller->tableSelected(selection);
        buttonRemove->Enable(true);
        buttonSave->Enable(true);
        buttonSaveAs->Enable(true);
    }
}


//! Remove button click event method.
/*!
   \param event the event.
*/
void TruthTableView::OnRemove(wxCommandEvent &event)
{
    controller->removeTable();
}


//! Updates the table canvas.
/*!
   \param table the table to update.
*/
void TruthTableView::updateTable(TruthTable table)
{
    canvas->setNewTable(table);
    Layout();
}


//! Updates the items of the table listbox.
/*!
   \param strings the string to be set.
*/
void TruthTableView::updateList(wxArrayString strings)
{
    listTables->Set(strings);
    buttonRemove->Enable(strings.size() > 0);
    buttonSave->Enable(strings.size() > 0);
    buttonSaveAs->Enable(strings.size() > 0);
    labelInfo->Show(strings.size() > 0);
}


//! Selects a table on the listbox.
/*!
   \param table the table name to be selected.
*/
void TruthTableView::selectTable(wxString table)
{
    int item = listTables->FindString(table);
    if (item != wxNOT_FOUND)
    {
        listTables->SetSelection(item);
        buttonRemove->Enable(true);
        buttonSave->Enable(true);
        buttonSaveAs->Enable(true);
        labelInfo->Show(true);
    }
}


//! Shows an error message.
/*!
   \param message the message to show.
*/
void TruthTableView::errorMsg(wxString message)
{
    wxMessageDialog dialog(this,
            message,
            _("Error"), 
            wxOK | wxICON_ERROR); 
    dialog.ShowModal();
}


//! Save button click event method.
/*!
   \param event the event.
*/
void TruthTableView::OnSave(wxCommandEvent& event)
{
    controller->saveFile();
}


//! Save As click event method.
/*!
   \param event the event.
*/
void TruthTableView::OnSaveAs(wxCommandEvent& event)
{
    controller->saveFileAs();
}


//! Open button click event method.
/*!
   \param event the event.
*/
void TruthTableView::OnOpen(wxCommandEvent& event)
{
    controller->openFile();
}


//! Shows a yes/no selection dialog.
/*!
   \param message the question to be asked.
   \return True if the user selectes yes, false otherwise.
*/
bool TruthTableView::msgYesNo(wxString message)
{
    wxMessageDialog dialog(this,
            message,
            _("Question"), 
            wxYES | wxNO); 
    return (dialog.ShowModal() == wxID_YES);
}
