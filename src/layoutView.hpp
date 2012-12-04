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
 * \class LayoutView
 * \brief Presentation layer class for spaces.
 * 
 * This class manages a space view. It not only has to show the current space
 * but the truth tables, rules and forbidden pattern so the user can
 * select and unselect them, and also can assign the inputs and outputs
 * of the truth tables to the space.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.21 $
 */

#ifndef LAYOUTVIEW_HPP_
#define LAYOUTVIEW_HPP_

#include <wx/wx.h>

#include "layoutCanvas.hpp"
#include "layoutManager.hpp"
#include "truthTableCanvas.hpp"
#include <map>

using namespace std;

class LayoutManager;

class LayoutView : public wxPanel
{
public:
	LayoutView(wxWindow* parent, wxWindowID id, LayoutManager *controller);
	virtual ~LayoutView();
    void updateGrids(Grid grid, bool changed);
    void OnNew(wxCommandEvent &event);
    void OnSave(wxCommandEvent &event);
    void OnSaveAs(wxCommandEvent &event);
    void OnOpen(wxCommandEvent &event);
    void OnClick(LayoutCanvasEvent &event);
    void updateTTList(map<wxString, bool> tables);
    void updateFPList(map<unsigned int, bool> FPs);
    void updateRuleList(map<unsigned int, bool> rules);
    void updateInputList(wxArrayString strings);
    void updateOutputList(wxArrayString strings);
    void selectTable(wxString table);
    void errorMsg(wxString message);
    bool msgYesNo(wxString message);
    void OnTableSelected(wxCommandEvent &event);
    void OnInputSelected(wxCommandEvent &event);
    void OnOutputSelected(wxCommandEvent &event);
    void OnTableChecked(wxCommandEvent &event);
    void OnRuleChecked(wxCommandEvent &event);
    void OnFPChecked(wxCommandEvent &event);
    void OnAssignInput(wxCommandEvent &event);
    void OnAssignOutput(wxCommandEvent &event);
    void OnPrepare(wxCommandEvent &event);
    void OnStart(wxCommandEvent &event);
    void onlyAssign(bool enable);
    void OnKeyPressed(wxKeyEvent &event);
    void updateCanvas();
    void updateInputs(vector<coordinate> inputs);
    void updateOutputs(vector<coordinate> outputs);
    void checkTable(wxString table, bool enabled);
    void checkRule(wxString rule, bool enabled);
    void checkFP(wxString FP, bool enabled);
    void enableCheckSimulation(bool enable);
    void enableStartSimulation(bool enable);
	
private:
    DECLARE_EVENT_TABLE()
    void initControls();
    void initSizers();
    
    LayoutManager *controller;
    
    //Labels
    wxStaticText *labelTitle;

    //Buttons
    wxBitmapButton *buttonNew;
    wxBitmapButton *buttonOpen;
    wxBitmapButton *buttonSave;
    wxBitmapButton *buttonSaveAs;
    wxBitmapButton *buttonAssignInput;
    wxBitmapButton *buttonAssignOutput;
    wxButton *buttonPrepareSimulation;
    wxButton *buttonStartSimulation;
    
    //Other controls
    wxCheckListBox *listTables;
    wxCheckListBox *listRules;
    wxCheckListBox *listFPs;
    wxListBox *listInputs;
    wxListBox *listOutputs;
    LayoutCanvas *canvas;
};

#endif /*LAYOUTVIEW_HPP_*/
