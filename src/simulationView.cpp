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

// $Revision: 1.12 $

#include "simulationView.hpp"
#include "resources/play.xpm"
#include "resources/step.xpm"
#include "resources/pause.xpm"
#include "resources/fforward.xpm"
#include "nanoStatusBar.hpp"
#include <wx/statline.h>

#include <iostream>

using namespace std;

enum
{
    ID_BUTTON_PLAYPAUSE = 0,
    ID_BUTTON_STEP,
    ID_BUTTON_NEXT,
    ID_BUTTON_FORWARD,
    ID_BUTTON_RESULTS,
    ID_TIMER,
    ID_SLIDER
};


BEGIN_EVENT_TABLE(SimulationView, wxFrame)
    EVT_BUTTON  (ID_BUTTON_PLAYPAUSE, SimulationView::OnPlayPause)
    EVT_BUTTON  (ID_BUTTON_STEP, SimulationView::OnStep)
    EVT_BUTTON  (ID_BUTTON_NEXT, SimulationView::OnNext)
    EVT_BUTTON  (ID_BUTTON_FORWARD, SimulationView::OnForward)
    EVT_BUTTON  (ID_BUTTON_RESULTS, SimulationView::OnResults)
    EVT_TIMER (ID_TIMER, SimulationView::OnTimer)
    EVT_SLIDER  (ID_SLIDER, SimulationView::OnSlider)
    EVT_CLOSE  (SimulationView::OnClose)
END_EVENT_TABLE()

//! Constructor.
/*!
   \param parent the parent window.
   \param id the window identifier.
*/
SimulationView::SimulationView(wxWindow* parent, wxWindowID id)
    :wxFrame(parent, id, _("Simulation")), chrono(this, ID_TIMER)
{
    playing = false;
    finished = false;
    Maximize();
    initControls();
    initSizers();
    NanoStatusBar *statusBar = new NanoStatusBar(this);
    SetStatusBar(statusBar);
    timerInterval = 1000;
    SetMinSize(wxSize(640, 480));
}


//! Destructor.
SimulationView::~SimulationView()
{
}


//! Control initialization.
void SimulationView::initControls()
{
    canvas = new LayoutCanvas(this, wxID_ANY, Grid());
    bitmapPlay = new wxBitmap(play_xpm);
    buttonPlayPause = new wxBitmapButton(this, ID_BUTTON_PLAYPAUSE, *bitmapPlay, wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
    buttonPlayPause->SetToolTip(_("Start automated simulation"));
    bitmapStep = new wxBitmap(step_xpm);
    bitmapPause = new wxBitmap(pause_xpm);
    bitmapForward = new wxBitmap(fforward_xpm);
    buttonStep = new wxBitmapButton(this, ID_BUTTON_STEP, *bitmapStep, wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
    buttonStep->SetToolTip(_("Advance one step"));
    buttonForward = new wxBitmapButton(this, ID_BUTTON_FORWARD, *bitmapForward, wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
    buttonForward->SetToolTip(_("Simulate in one step"));
    textInfo = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);    
    buttonNextRow = new wxButton(this, ID_BUTTON_NEXT, _("Simulate next row"));
    buttonResults = new wxButton(this, ID_BUTTON_RESULTS, _("Show results"));
    buttonNextRow->Enable(false);
    buttonResults->Enable(false);
    slider = new wxSlider(this, ID_SLIDER, 1000, 100, 3000);
    labelTimer = new wxStaticText(this, wxID_ANY, _("Timer interval: 1000ms"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
}


//! Sizer (layout) initialization.
void SimulationView::initSizers()
{
    //TODO: make it better (design)
    //Center the control buttons
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    //mainSizer->Add(labelTitle, 0, wxSHAPED | wxALIGN_CENTER, 2);
    //mainSizer->Add(new wxStaticLine(this, wxID_ANY), 
    //                            0, 
    //                            wxEXPAND | wxALL, 
    //                            2);
    wxStaticBoxSizer *simulationControlsSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Controls"));
    wxBoxSizer *canvasButtonsSizer = new wxBoxSizer(wxVERTICAL);
    canvasButtonsSizer->Add(canvas, 2, wxEXPAND | wxALL, 2);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(buttonPlayPause, 
                                  0, 
                                  wxSHAPED | wxALIGN_CENTER, 
                                  2);
    buttonSizer->Add(buttonStep, 
                                  0, 
                                  wxSHAPED | wxALIGN_CENTER, 
                                  2);
     buttonSizer->Add(buttonForward, 
                                  0, 
                                  wxSHAPED | wxALIGN_CENTER, 
                                  2);
    canvasButtonsSizer->Add(buttonSizer, 
                                               0, 
                                               wxSHAPED | wxALIGN_CENTER, 
                                               2);
    canvasButtonsSizer->Add(labelTimer, 
                                               0, 
                                               wxEXPAND | wxALL, 
                                               2);
    canvasButtonsSizer->Add(slider, 
                                               0, 
                                               wxEXPAND | wxALL, 
                                               2);
    wxBoxSizer *controlButtonSizer = new wxBoxSizer(wxHORIZONTAL);
    controlButtonSizer->Add(buttonNextRow, 
                                             0, 
                                             wxSHAPED | wxALIGN_CENTER, 
                                             2);
    controlButtonSizer->AddStretchSpacer();                                          
    controlButtonSizer->Add(buttonResults, 
                                             0, 
                                             wxSHAPED | wxALIGN_CENTER, 
                                             2);
    simulationControlsSizer->Add(canvasButtonsSizer, 
                                                     3, 
                                                     wxEXPAND | wxALL, 
                                                     2);
    simulationControlsSizer->Add(textInfo, 
                                                     2, 
                                                     wxEXPAND | wxALL, 
                                                     2);
    //simulationControlsSizer->Add(controlButtonSizer, 
    //                                                 0, 
    //                                                 wxSHAPED | wxALIGN_CENTER, 
    //                                                 2);
    mainSizer->Add(simulationControlsSizer, 
                               2, 
                               wxEXPAND | wxALL, 
                               2);
    mainSizer->Add(controlButtonSizer, 
                               0, 
                               wxEXPAND | wxALL, 
                               2);
    //mainSizer->Add(new wxStaticLine(this, wxID_ANY), 
    //                            0, 
    //                            wxEXPAND | wxALL, 
    //                            2);
    //mainSizer->Add(textInfo, 
    //                           1, 
    //                           wxEXPAND | wxALL, 
    //                           2);
    SetSizer(mainSizer);
    //mainSizer->SetSizeHints(this);
}


//! Play/Pause button click event.
/*!
   \param event the event.
*/
void SimulationView::OnPlayPause(wxCommandEvent &event)
{
    if (playing)
    {
        buttonPlayPause->SetBitmapLabel(*bitmapPlay);
        buttonPlayPause->SetToolTip(_("Start automated simulation"));
        chrono.Stop();
    }
    else
    {
        buttonPlayPause->SetBitmapLabel(*bitmapPause);
        buttonPlayPause->SetToolTip(_("Pause automated simulation"));
        chrono.Start(timerInterval, false);
    }
    playing = !playing;
}


//! Step button click event.
/*!
   \param event the event.
*/
void SimulationView::OnStep(wxCommandEvent &event)
{
    if (!playing)
    {
        simulation->nextStep(true);
    }
}


//! Sets the grid to display.
/*!
   \param newGrid the grid to be displayed.
   \param changed if true, the cell transitions are disabled.
*/
void SimulationView::setGrid(Grid newGrid, bool changed)
{
    canvas->setGrid(newGrid, changed);
    Layout();
}


//! Appends a text line to the information box.
/*!
   \param info the message to display.
*/
void SimulationView::setInfo(wxString info)
{
    textInfo->AppendText(info + _("\n"));
    wxYield();
}


//! Appends a blank line in the information box.
void SimulationView::setBlankLine()
{
    textInfo->AppendText(_("\n"));
    wxYield();
}


//! Member accessor.
/*!
   \param finished the finished simulation status.
*/
void SimulationView::setFinished(bool finished)
{
    this->finished = finished;
}


//! Timer tick event.
/*!
   In here the simulation step is taken if the automated simulation is on.
   \param event the event.
*/
void SimulationView::OnTimer(wxTimerEvent& event)
{
    if (playing)
    {
        simulation->nextStep(true);
    }
}


//! Sets the simulation.
/*!
   \param simulation the simulation.
*/
void SimulationView::setSimulation(Simulation *simulation)
{
    this->simulation = simulation;
}


//! Enables/disables the next row button.
/*!
   \param enable if true, the button is enabled, otherwise is disabled.
*/
void SimulationView::setNextRow(bool enable)
{
    buttonNextRow->Enable(enable);
    if (enable)
    {
        setInfo(_("Press the \"Next row\" button to continue simulating"));
    }
}


//! Enables/disables the results button.
/*!
   \param enable if true, the button is enabled, otherwise is disabled.
*/
void SimulationView::setResults(bool enable)
{
    buttonResults->Enable(enable);
    if (enable)
    {
        setInfo(_("Press the \"Show results\" button to see the simulation results"));
    }
}


//! Next row button click event.
/*!
   \param event the event.
*/
void SimulationView::OnNext(wxCommandEvent &event)
{
    simulation->nextRow();
}


//! Results button click event.
/*!
   \param event the event.
*/
void SimulationView::OnResults(wxCommandEvent &event)
{
    simulation->results();
    Show(false);
}


//! Enabled/disables the simulation controls.
/*!
   \param enabled if true, the controls are enabled, otherwise are disabled.
*/
void SimulationView::enableSimulation(bool enabled)
{
    buttonPlayPause->Enable(enabled);
    buttonStep->Enable(enabled);
    buttonForward->Enable(enabled);
    if (!enabled)
    {
        buttonPlayPause->SetBitmapLabel(*bitmapPlay);
        buttonPlayPause->SetToolTip(_("Start automated simulation"));
        chrono.Stop();
        playing = false;
    }
    Update();
}


//! Stops the simulation.
void SimulationView::stopSimulation()
{
    playing = false;
}


//! Timer slider change event.
/*!
   \param event the event.
*/
void SimulationView::OnSlider(wxCommandEvent &event)
{
    timerInterval = slider->GetValue();
    labelTimer->SetLabel(wxString::Format(_("Timer interval: %ldms"), timerInterval));
    Layout();
    //Reset the timer if its changed when playing
    if (playing)
    {
        chrono.Start(timerInterval, false);
    }
}


//! Forward button change event.
/*!
   \param event the event.
*/
void SimulationView::OnForward(wxCommandEvent &event)
{
    simulation->simulateAll();
}


//! Window close button event.
/*!
   \param event the event.
*/
void SimulationView::OnClose(wxCloseEvent &event)
{
    //We do not allow this window to be closed.
    //It'll be hidden when the show results button is pressed.
}
