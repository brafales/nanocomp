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
 * \class RuleView
 * \brief Presentation class for the rules.
 * 
 * This is the presentation layer class for the rule collection.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.18 $
 */
 
 #ifndef RULEVIEW_HPP_
#define RULEVIEW_HPP_

#include <wx/wx.h>
#include "layoutCanvas.hpp"
#include "ruleManager.hpp"

class RuleManager;

class RuleView : public wxPanel
{
public:
	RuleView(wxWindow* parent, wxWindowID id, RuleManager *controller);
	virtual ~RuleView();
    void updateGrids(Grid initial, Grid final, bool changed);
    void updateList(wxArrayString strings);
    void selectRule(unsigned int rule);
    void errorMsg(wxString message);
    bool msgYesNo(wxString message);
    void setListMessage(int id, wxString message);
    void resetListItem(int id);
    
 private:
    DECLARE_EVENT_TABLE()
    void initControls();
    void initSizers();
    void OnClick(LayoutCanvasEvent &event);
    void OnSelection(wxCommandEvent &event);
    void OnNewRule(wxCommandEvent& event);
    void OnRemove(wxCommandEvent& event);
    void OnCopy(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnCheck(wxCommandEvent& event);
    int findIndex(int index);
    
    //! Main application controller.
    RuleManager *controller;
    
    //GUI Controls
    //Labels
    wxStaticText *labelTitle;
    wxStaticText *labelRule;
    wxStaticText *labelInitialLayout;
    wxStaticText *labelFinalLayout;

    //Buttons
    wxBitmapButton *buttonNew;
    wxBitmapButton *buttonSave;
    wxBitmapButton *buttonSaveAs;
    wxBitmapButton *buttonRemove;
    wxBitmapButton *buttonOpen;
    wxBitmapButton *buttonCopy;
    wxButton *buttonCheck;
    
    //Other controls
    wxListBox *listRules;
    LayoutCanvas *canvasInitial;
    LayoutCanvas *canvasFinal;
};

#endif /*RULEVIEW_HPP_*/
