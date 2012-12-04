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

// $Revision: 1.6 $

#include "resultsView.hpp"
#include "nanoStatusBar.hpp"
#include <wx/statline.h>

enum
{
    ID_LISTROWS = 0,
    ID_LISTINFORMATION,
    ID_LISTPATH,
    ID_CANVAS, 
    ID_CANVASINITIAL,
    ID_TABLE
};

BEGIN_EVENT_TABLE(ResultsView, wxFrame)
    EVT_LISTBOX (ID_LISTROWS, ResultsView::OnRowsSelected)
    EVT_LISTBOX (ID_LISTINFORMATION, ResultsView::OnInformationSelected)
    EVT_LISTBOX (ID_LISTPATH, ResultsView::OnPathSelected)
    EVT_CLOSE    (ResultsView::OnClose)
END_EVENT_TABLE()

//! Constructor.
/*!
   \param parent the parent window.
   \param id the window id.
   \param controller the simulation controller.
*/
ResultsView::ResultsView(wxWindow* parent, wxWindowID id, SimulationManager *controller)
    :wxFrame(parent, id, _("Simulation Results"))
{
    this->controller = controller;
    Maximize();
    initControls();
    initSizers();
    NanoStatusBar *statusBar = new NanoStatusBar(this);
    SetStatusBar(statusBar);
    SetMinSize(wxSize(640, 480));
    Center();
    SetStatusText(_("Click on a row and an event to see the simulation trace"), 0);
}


//! Destructor.
ResultsView::~ResultsView()
{
}


//! Window controls initialization.
void ResultsView::initControls()
{
    listRows = new wxListBox(this, ID_LISTROWS, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE);
    listInformation = new wxListBox(this, ID_LISTINFORMATION, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE);
    listPath = new wxListBox(this, ID_LISTPATH, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE);
    labelResult = new wxStaticText(this, wxID_ANY, _("Simulation result: "), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    labelRows = new wxStaticText(this, wxID_ANY, _("Select row"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    labelInformation = new wxStaticText(this, wxID_ANY, _("Select event"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    labelPath = new wxStaticText(this, wxID_ANY, _("Simulation trace"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    canvasResult = new LayoutCanvas(this, ID_CANVAS, Grid());
    canvasInitial = new LayoutCanvas(this, ID_CANVASINITIAL, Grid());
    tCanvas = new TruthTableCanvas(this, ID_TABLE, TruthTable());
    tCanvas->setEditable(false);
}


//! Window sizers (layout) initialization.
void ResultsView::initSizers()
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    
    mainSizer->Add(labelResult, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(new wxStaticLine(this, wxID_ANY), 
                                0, 
                                wxEXPAND | wxALL, 
                                5);
    
    wxGridSizer *gridSizer = new wxGridSizer(2, 2);
    
    wxStaticBoxSizer *listSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Simulation information"));
    
    wxBoxSizer *rowSizer = new wxBoxSizer(wxVERTICAL);
    rowSizer->Add(labelRows, 0, wxEXPAND | wxALL, 2);
    rowSizer->Add(listRows, 1, wxEXPAND | wxALL, 2);
    
    wxBoxSizer *informationSizer = new wxBoxSizer(wxVERTICAL);
    informationSizer->Add(labelInformation, 0, wxEXPAND | wxALL, 2);
    informationSizer->Add(listInformation, 1, wxEXPAND | wxALL, 2);
    
    wxBoxSizer *pathSizer = new wxBoxSizer(wxVERTICAL);
    pathSizer->Add(labelPath, 0, wxEXPAND | wxALL, 2);
    pathSizer->Add(listPath, 1, wxEXPAND | wxALL, 2);
    
    listSizer->Add(rowSizer, 1, wxEXPAND | wxALL, 2);
    listSizer->Add(informationSizer, 1, wxEXPAND | wxALL, 2);
    listSizer->Add(pathSizer, 1, wxEXPAND | wxALL, 2);
    
    gridSizer->Add(listSizer, 1, wxEXPAND | wxALL, 2);
    
    wxStaticBoxSizer *canvasSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Simulation step"));
    canvasSizer->Add(canvasResult, 1, wxEXPAND | wxALL, 2);
    gridSizer->Add(canvasSizer, 1, wxEXPAND | wxALL, 2);
    
    wxStaticBoxSizer *tableSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Truth table"));
    tableSizer->Add(tCanvas, 1, wxEXPAND | wxALL, 2);
    gridSizer->Add(tableSizer, 1, wxEXPAND | wxALL, 2);
    
    wxStaticBoxSizer *initialCanvasSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Initial configuration"));
    initialCanvasSizer->Add(canvasInitial, 1, wxEXPAND | wxALL, 2);
    gridSizer->Add(initialCanvasSizer, 1, wxEXPAND | wxALL, 2);
    
    mainSizer->Add(gridSizer, 1, wxEXPAND | wxALL, 2);
    
    SetSizer(mainSizer);
}


//! Sets the result view grid.
/*!
   \param grid the new grid.
   \param changed true if the grid is from a different space,
   false if it's the same space with changes.
*/
void ResultsView::updateGrid(Grid grid, bool changed)
{
    canvasResult->setGrid(grid, changed);
}


//! Sets the initial grid.
/*!
   \param grid the new grid.
   \param changed true if the grid is from a different space,
   false if it's the same space with changes.
*/
void ResultsView::updateInitialGrid(Grid grid, bool changed)
{
    canvasInitial->setGrid(grid, changed);
}


//! Updates the row listbox.
/*!
   \param strings the items to set.
*/
void ResultsView::updateRowList(wxArrayString strings)
{
    listRows->Set(strings);
}


//! Selects a simulation row.
/*!
   \param row the row index.
*/
void ResultsView::selectRow(unsigned int row)
{
    listRows->SetSelection(row);
}


//! Updates the event listbox.
/*!
   \param strings the string array to set on the listbox.
*/
void ResultsView::updateInformationList(wxArrayString strings)
{
    listInformation->Set(strings);
}


//! Selects an event.
/*!
   \param information the event index.
*/
void ResultsView::selectInformation(unsigned int information)
{
    listInformation->SetSelection(information);
}


//! Updates the simulation path listbox.
/*!
   \param strings the array string to be set.
*/
void ResultsView::updatePathList(wxArrayString strings)
{
    listPath->Set(strings);
}


//! Selects a simulation step.
/*!
   \param path the step index.
*/
void ResultsView::selectPath(unsigned int path)
{
    listPath->SetSelection(path);
}


//! Shows an error message.
/*!
   \param message the error message to be shown.
*/
void ResultsView::errorMsg(wxString message)
{
    
}


//! Row selection event method.
/*!
   \param event the event.
*/
void ResultsView::OnRowsSelected(wxCommandEvent &event)
{
    controller->rowSelected(event.GetInt());
}


//! Event selection event method.
/*!
   \param event the event.
*/
void ResultsView::OnInformationSelected(wxCommandEvent &event)
{
    controller->informationSelected(event.GetInt());
}


//! Simulation step selection event method.
/*!
   \param event the event.
*/
void ResultsView::OnPathSelected(wxCommandEvent &event)
{
    controller->pathSelected(event.GetInt());
}


//! Sets the truth table to be shown.
/*!
   \param table the truth table.
*/
void ResultsView::setTable(TruthTable table)
{
    tCanvas->setNewTable(table);
    Layout();
}


//! Sets the simulation result.
/*!
   \param success true if the simulation was successful, false otherwise.
*/
void ResultsView::setResult(bool success)
{
    if (success)
    {
        labelResult->SetLabel(_("Simulation result: SUCCESS"));
        labelResult->SetForegroundColour(*wxGREEN);
    }
    else
    {
        labelResult->SetLabel(_("Simulation result: FAILURE"));
        labelResult->SetForegroundColour(*wxRED);
    }
    Layout();
}


//! Sets the initial layout input coordinates.
/*!
   \param inputs the inputs coordinates.
*/
void ResultsView::setInputs(vector <coordinate> inputs)
{
    canvasInitial->setAssignedInputs(inputs);
}


//! Sets the initial layout output coordinates.
/*!
   \param outputs the outputs coordinates.
*/
void ResultsView::setOutputs(vector <coordinate> outputs)
{
    canvasInitial->setAssignedOutputs(outputs);
}


//! Window close button event.
/*!
   \param event the event.
*/
void ResultsView::OnClose(wxCloseEvent &event)
{
    //We must let the controller know that the user has ended
    //the simulation completely
    controller->endSimulation();
}


//! Adds a highlight zone to the step view.
/*!
   \param top the top topleft coordinate.
   \param left the left topleft coordinate.
   \param width the width.
   \param height the height.
*/
void ResultsView::addHighlight(int top, int left, int width, int height)
{
    canvasResult->addRectangle(top, left, width, height);
}
