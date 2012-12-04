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
 * \class SimulationView
 * \brief Presentation layer for the simulation.
 * 
 * This is the view for the simulation. It shows the space being simulated
 * and information about the simulation. It allows the user to play, pause 
 * and execute individual steps on the simulation, and also to control the 
 * timer of the automated simulation.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.8 $
 */

#ifndef SIMULATIONVIEW_HPP_
#define SIMULATIONVIEW_HPP_

#include <wx/wx.h>
#include "simulation.hpp"
#include "grid.hpp"
#include "layoutCanvas.hpp"

class Simulation;

class SimulationView : public wxFrame
{
public:
	SimulationView(wxWindow* parent, wxWindowID id);
	virtual ~SimulationView();
    void setGrid(Grid newGrid,bool changed);
    void setInfo(wxString info);
    void setBlankLine();
    void setFinished(bool finished);
    void setSimulation(Simulation *simulation);
    void setNextRow(bool enable);
    void setResults(bool enable);
    void enableSimulation(bool enabled);
    void stopSimulation();
    
private:
    DECLARE_EVENT_TABLE()
    void initControls();
    void initSizers();
    void OnPlayPause(wxCommandEvent &event);
    void OnStep(wxCommandEvent &event);
    void OnNext(wxCommandEvent &event);
    void OnResults(wxCommandEvent &event);
    void OnTimer(wxTimerEvent& event);
    void OnSlider(wxCommandEvent &event);
    void OnForward(wxCommandEvent &event);
    void OnClose(wxCloseEvent &event);
    wxBitmap *bitmapPlay;
    wxBitmap *bitmapPause;
    wxBitmap *bitmapStep;
    wxBitmap *bitmapForward;
    wxBitmapButton *buttonPlayPause;
    wxBitmapButton *buttonStep;
    wxBitmapButton *buttonForward;
    wxButton *buttonNextRow;
    wxButton *buttonResults;
    wxStaticText *labelTimer;
    LayoutCanvas *canvas;
    wxTextCtrl *textInfo;
    wxSlider *slider;
    //! Timer interval in ms.
    int timerInterval;
    //! Simulation.
    Simulation *simulation;
    //! Timer.
    wxTimer chrono;
    //! Is the simulation playing?
    bool playing;
    //! Is the simulation finished?
    bool finished;
};

#endif /*SIMULATIONVIEW_HPP_*/
