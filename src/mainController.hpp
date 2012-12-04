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

#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

/**
 * \class MainController
 * \brief Main controller application.
 * 
 * This is the main controller for the application. It controls the main flow 
 * of the program and, more importantly, comunicates all the other
 * controllers between them. It has also the control over the main window
 * class and the project persistance data.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.14 $
 */
 
 class NanoFrame;
#include "nanoFrame.hpp"
#include "ruleManager.hpp"
#include "layoutManager.hpp"
#include "truthTableManager.hpp"
#include "forbiddenPatternManager.hpp"
#include "fileManagerView.hpp"
#include "simulationManager.hpp"
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
 
class RuleManager;
class LayoutManager;
class TruthTableManager;
class ForbiddenPatternManager;
class SimulationManager;
 
class MainController 
{
public:
    MainController();
    ~MainController();
    void initControllers();
    RuleManager* getRules();
    LayoutManager* getLayouts();
    ForbiddenPatternManager* getForbiddenPatterns();
    TruthTableManager* getTruthTables();
    void setNanoFrame(NanoFrame *frame);
    void setRules(RuleManager *rules);
    void setLayouts(LayoutManager *layouts);
    void setForbiddenPatterns(ForbiddenPatternManager *forbiddenPatterns);
    void setTruthTables(TruthTableManager *truthTables);
    void tablesChanged();
    void FPsChanged();
    void rulesChanged();
    void setStatusMessage(wxString message);
    void save();
    void saveAs();
    void open();
    void newFile();
    void startSimulation();
    void prepareSimulation();
    void elementChanged();
    NanoFrame* getNanoFrame();
    void endSimulation();

private:
    //! Main window.
     NanoFrame *frame;
     //! Rule controller.
     RuleManager *rules;
     //! Space controller.
     LayoutManager *layouts;
     //! Forbidden pattern controller.
     ForbiddenPatternManager *forbiddenPatterns;
     //! Truth table controller.
     TruthTableManager *truthTables;
     //! Simulation controller.
     SimulationManager *simulation;
     //! File system presentation layer.
     FileManagerView fileView;
     //! Project file name.
     wxString fileName;
     //! Flag to know if a simulation is in progress
     bool simulating;
     void saveFileNames(wxTextOutputStream *stream);
     void saveTables(wxTextOutputStream *stream);
     void saveRules(wxTextOutputStream *stream);
     void savePatterns(wxTextOutputStream *stream);
     bool openFile(wxString file);
     bool openTables(wxFileInputStream *fileStream, wxTextInputStream *inStream);
     bool openRules(wxFileInputStream *fileStream, wxTextInputStream *inStream);
     bool openFPs(wxFileInputStream *fileStream, wxTextInputStream *inStream);
};
#endif //MAINCONTROLLER_H

