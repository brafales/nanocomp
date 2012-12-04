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

// $Revision: 1.23 $

#include "nanoFrame.hpp"
#include "nanocompAboutDialog.hpp"
#include "ruleView.hpp"
#include "forbiddenPatternView.hpp"
#include "layoutView.hpp"
#include "truthTableView.hpp"
#include "resources/fileopen.xpm"
#include "resources/filesave.xpm"
#include "resources/filesaveas.xpm"
#include "resources/new.xpm"
#include "resources/icon.xpm"
#include <wx/notebook.h>
#include "nanoStatusBar.hpp"


enum
{
    ID_BUTTON_NEW,
    ID_BUTTON_OPEN,
    ID_BUTTON_SAVE,
    ID_BUTTON_SAVEAS,
    ID_LEGEND_ENABLED,
    ID_LEGEND_DISABLED,
    ID_LEGEND_INPUT,
    ID_LEGEND_OUTPUT,
    ID_LEGEND_NOSPACE,
    ID_MENU_ABOUT
};


BEGIN_EVENT_TABLE(NanoFrame, wxFrame)
    EVT_TOOL  (ID_BUTTON_SAVE, NanoFrame::OnSave)
    EVT_TOOL  (ID_BUTTON_SAVEAS, NanoFrame::OnSaveAs)
    EVT_TOOL  (ID_BUTTON_NEW, NanoFrame::OnNew)
    EVT_TOOL  (ID_BUTTON_OPEN, NanoFrame::OnOpen)
    EVT_MENU  (ID_MENU_ABOUT, NanoFrame::OnAbout)
END_EVENT_TABLE()


//! Constructor.
/*!
   \param controller the main application controller.
*/
NanoFrame::NanoFrame(MainController *controller)
: wxFrame( (wxFrame *) NULL, -1, _("NanoComp"), wxPoint(50, 50), wxSize(450, 300) ), nanoController(controller)
{
    this->nanoController = controller;
	initMenubar();
	initToolbar();
    initStatusbar();
    wxBoxSizer *mainSizer = new wxBoxSizer( wxVERTICAL );
    //LayoutCanvas *canvas = new LayoutCanvas(this, 10, 10);
    SetSizer(mainSizer);
    //wxNotebook *notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_LEFT);
    wxNotebook *notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
    RuleView *ruleTab = new RuleView(notebook, wxID_ANY, nanoController->getRules());
    ForbiddenPatternView *FPTab = new ForbiddenPatternView(notebook, wxID_ANY, nanoController->getForbiddenPatterns());
    LayoutView *LayoutTab = new LayoutView(notebook, wxID_ANY, nanoController->getLayouts());
    TruthTableView *TTTab = new TruthTableView(notebook, wxID_ANY, nanoController->getTruthTables());
    
    notebook->AddPage(ruleTab, wxT("Rules"), true);
    notebook->AddPage(FPTab, wxT("Forbidden Patterns"), false);
    notebook->AddPage(TTTab, wxT("Truth Tables"), false);
    notebook->AddPage(LayoutTab, wxT("Layouts and Simulation"), false);
    
    mainSizer->Add(notebook, 1, wxEXPAND | wxALL, 0);
    
    SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);
    setTitleBar(_("Untitled"));
    SetIcon(wxIcon(icon_xpm));
    Center();
}


//! Destructor.
NanoFrame::~NanoFrame()
{
}


//! Initializes the menu bar.
void NanoFrame::initMenubar()
{
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(ID_MENU_ABOUT, _("&About..."), _("About NanoComp"));
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(helpMenu, _("&Help"));

    SetMenuBar(menuBar);
}


//! Creates and sets the tool bar.
void NanoFrame::initToolbar()
{
	wxToolBar *toolBar = new wxToolBar(this,
         wxID_ANY, 
         wxDefaultPosition, 
         wxDefaultSize);
     wxBitmap bmpOpen(fileopen_xpm);
     wxBitmap bmpNew(new_xpm);
     wxBitmap bmpSave(filesave_xpm);
     wxBitmap bmpSaveAs(filesaveas_xpm);

     toolBar->AddTool(ID_BUTTON_NEW, bmpNew, _("New"));
     toolBar->AddTool(ID_BUTTON_OPEN, bmpOpen, _("Open"));
     toolBar->AddTool(ID_BUTTON_SAVE, bmpSave, _("Save"));
     toolBar->AddTool(ID_BUTTON_SAVEAS, bmpSaveAs, _("Save As"));
     
     toolBar->Realize();
     SetToolBar(toolBar);
}


//! Creates and sets the status bar.
void NanoFrame::initStatusbar()
{
    NanoStatusBar *statusBar = new NanoStatusBar(this);
    SetStatusBar(statusBar);
    setStatusMessage(_T("NanoComp"));
}


//! Sets a message on the status bar.
/*!
   \param message the message.
*/
void NanoFrame::setStatusMessage(wxString message)
{
    SetStatusText(message, 0);
}


//! Save project button click event.
/*!
   \param event the event.
*/
void NanoFrame::OnSave(wxCommandEvent &event)
{
    nanoController->save();
}


//! Save As project button click event.
/*!
   \param event the event.
*/
void NanoFrame::OnSaveAs(wxCommandEvent &event)
{
    nanoController->saveAs();
}


//! Open project button click event.
/*!
   \param event the event.
*/
void NanoFrame::OnOpen(wxCommandEvent &event)
{
    nanoController->open();
}


//! New project button click event.
/*!
   \param event the event.
*/
void NanoFrame::OnNew(wxCommandEvent &event)
{
    nanoController->newFile();
}


//! Shows an error message.
/*!
   \param message the error message.
*/
void NanoFrame::errorMsg(wxString message)
{
    wxMessageDialog dialog(this,
            message,
            _("Error"), 
            wxOK | wxICON_ERROR); 
    dialog.ShowModal();
}


//! Sets a message on the window title bar.
/*!
   \param title message.
*/
void NanoFrame::setTitleBar(wxString title)
{
    SetTitle(title + _(" - NanoComp"));
}


//! About menu bar click event.
/*!
   \param event the event.
*/
void NanoFrame::OnAbout(wxCommandEvent &event)
{
    NanocompAboutDialog dialog(this);
    dialog.ShowModal();
}


//! Shows an information message.
/*!
   \param message the error message.
*/
void NanoFrame::showMessage(wxString message)
{
    wxMessageDialog dialog(this,
            message,
            _("Information"), 
            wxOK | wxICON_INFORMATION); 
    dialog.ShowModal();
}
