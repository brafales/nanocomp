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
 * \class ResultsView
 * \brief View for showing the simulation results.
 * 
 * This view gives information of the simulation results. It also
 * allows the user to select different rows, events and simulation
 * steps to be shown.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.5 $
 */

#ifndef RESULTSVIEW_HPP_
#define RESULTSVIEW_HPP_

#include <wx/wx.h>
#include "layoutCanvas.hpp"
#include "simulationManager.hpp"

class SimulationManager;

class ResultsView : public wxFrame
{
public:
	ResultsView(wxWindow* parent, wxWindowID id, SimulationManager * controller);
	virtual ~ResultsView();
    void updateGrid(Grid grid, bool changed);
    void updateInitialGrid(Grid grid, bool changed);
    void updateRowList(wxArrayString strings);
    void selectRow(unsigned int row);
    void updateInformationList(wxArrayString strings);
    void selectInformation(unsigned int information);
    void updatePathList(wxArrayString strings);
    void selectPath(unsigned int path);
    void errorMsg(wxString message);
    void setTable(TruthTable table);
    void setResult(bool success);
    void setInputs(vector <coordinate> inputs);
    void setOutputs(vector <coordinate> outputs);
    void addHighlight(int top, int left, int width, int height);

private:
    DECLARE_EVENT_TABLE()
    void initControls();
    void initSizers();
    void OnRowsSelected(wxCommandEvent &event);
    void OnInformationSelected(wxCommandEvent &event);
    void OnPathSelected(wxCommandEvent &event);
    void OnClose(wxCloseEvent &event);
    //! Simulation manager
    SimulationManager *controller;
    wxListBox *listRows;
    wxListBox *listInformation;
    wxListBox *listPath;
    wxStaticText *labelResult;
    wxStaticText *labelRows;
    wxStaticText *labelInformation;
    wxStaticText *labelPath;
    LayoutCanvas *canvasResult;
    LayoutCanvas *canvasInitial;
    TruthTableCanvas *tCanvas;
};

#endif /*RESULTSVIEW_HPP_*/
