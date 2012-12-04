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
/**
 * \class ForbiddenPatternView
 * \brief Presentation layer class for the forbidden patterns.
 * 
 * Main view for the forbidden patterns of the system. Receives petitions
 * from the user and asks the controller to do the job.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.15 $
 */

#ifndef FORBIDDENPATTERNVIEW_HPP_
#define FORBIDDENPATTERNVIEW_HPP_

#include <wx/wx.h>

#include "layoutCanvas.hpp"
#include "forbiddenPatternManager.hpp"

class ForbiddenPatternManager;

class ForbiddenPatternView : public wxPanel
{
public:
	ForbiddenPatternView(wxWindow* parent, wxWindowID id, ForbiddenPatternManager *controller);
	virtual ~ForbiddenPatternView();
    void updateGrids(Grid grid, bool changed);
    void updateList(wxArrayString strings);
    void selectFP(unsigned int rule);
    bool msgYesNo(wxString message);
    void errorMsg(wxString message);
    void setListMessage(int id, wxString message);
    void resetListItem(int id);
	
private:
    DECLARE_EVENT_TABLE()
    void initControls();
    void initSizers();
    void OnClick(LayoutCanvasEvent &event);
    void OnNew(wxCommandEvent &event);
    void OnSelection(wxCommandEvent &event);
    void OnRemove(wxCommandEvent &event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnCheck(wxCommandEvent& event);
    int findIndex(int index);
    
    ForbiddenPatternManager *controller;
    //Labels
    wxStaticText *labelTitle;
    wxStaticText *labelFP;
    
    //Buttons
    wxBitmapButton *buttonNew;
    wxBitmapButton *buttonOpen;
    wxBitmapButton *buttonSave;
    wxBitmapButton *buttonSaveAs;
    wxBitmapButton *buttonRemove;
    wxButton *buttonCheck;
    
    //Other controls
    wxListBox *listFP;
    LayoutCanvas *canvas;
};

#endif /*FORBIDDENPATTERNVIEW_HPP_*/
