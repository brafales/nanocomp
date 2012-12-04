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
 * \class LayoutManager
 * \brief Main controller for space. 
 * 
 * It manages the current space,
 * and comunicates with the other controllers to get information.
 * It also manages the data persistance and presentation layer classes.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.21 $
 */

#ifndef LAYOUTMANAGER_HPP_
#define LAYOUTMANAGER_HPP_

#include "layoutView.hpp"
#include "layoutConfig.hpp"
#include "truthTableManager.hpp"
#include "ruleManager.hpp"
#include "forbiddenPatternManager.hpp"
#include "layoutDiskManager.hpp"
#include "mainController.hpp"
#include <map>
#include <list>
#include <wx/wx.h>

class LayoutView;
class LayoutConfig;
class LayoutDiskManager;

using namespace std;

class LayoutManager
{
public:
	LayoutManager(MainController *controller, TruthTableManager *tableManager, ForbiddenPatternManager *FPManager, RuleManager *ruleManager);
	virtual ~LayoutManager();
    void setView(LayoutView *view);
    void newLayout(int width, int height, int inputs, int outputs);
    void newLayout(LayoutConfig *layoutConfig);
    void tableSelected(wxString tableId);
    void inputSelected(int input);
    void outputSelected(int output);
    bool tableChecked(wxString tableId, bool checked);
    bool ruleChecked(unsigned int rule, bool checked);
    bool FPChecked(unsigned int FP, bool checked);
    void cellChanged(int x, int y);
    void tablesChanged();
    void rulesChanged();
    void FPsChanged();
    LayoutConfig* getLayout();
    bool saveFile();
    bool saveFileAs();
    void openFile();
    bool openPFile(wxString file);
    bool saveFilename(wxString file);
    void assignInput();
    void assignOutput();
    void escapePressed();
    bool isLayoutModified();
    bool setInput(wxString table, int input, int x, int y);
    bool setOutput(wxString table, int output, int x, int y);
    wxString getFileName();
    map< wxString, vector<coordinate> > getTableInputs();
    map< wxString, vector<coordinate> > getTableOutputs();
    vector<coordinate> getTableInput(wxString table);
    vector<coordinate> getTableOutput(wxString table);
    map< wxString, bool> getTableEnabled();
    map<unsigned int, bool> getRuleEnabled();
    map<unsigned int, bool> getFPEnabled();
    int getTables();
    int getRules();
    int getFPs();
    void clean();
    void prepareSimulation();
    void startSimulation();
    list <ForbiddenPattern> getListFPEnabled();
    list <Rule> getListRuleEnabled();
    void elementChanged();
    TruthTable *getTableSelected();
    bool checkLayout();
    void enableSimulation();

private:
    //! Presentation class.
    LayoutView *view;
    //! Truth table manager.
    TruthTableManager *tableManager;
    //! Forbidden pattern manager.
    ForbiddenPatternManager *FPManager;
    //! Rule manager.
    RuleManager *ruleManager;
    //! The current space.
    LayoutConfig* configuration;
    //! File system interaction presentation class.
    FileManagerView fileView;
    //! Data persistance class.
    LayoutDiskManager *diskManager;
    //! Main application controller.
    MainController *controller;
    //! Filename of the space.
    wxString layoutFileName;
    //! Has the space been modified after saving?
    bool layoutModified;
    //! Do we need a filename to save the space or not?
    bool layoutNeedNewFile;
    //! Assigned input coordinates for the truth tables.
    map< wxString, vector<coordinate> > tableInputs;
    //! Assigned output coordinates for the truth tables.
    map< wxString, vector<coordinate> > tableOutputs;
    //! Tables selected.
    map< wxString, bool> tableEnabled;
    //! Rules selected.
    map<unsigned int, bool> ruleEnabled;
    //! Forbidden patterns selected.
    map<unsigned int, bool> FPEnabled;
    //! Current table selected.
    wxString currentTable;
    //! Current input selected.
    int currentInput;
    //! Current output selected.
    int currentOutput;
    //! Are we assigning an input?
    bool assigningInput;
    //! Are we assigning an output?
    bool assigningOutput;
    void updateGrids(bool changed);
    void updateTTList(wxArrayString tables);
    void updateFPList(wxArrayString FPs);
    void updateRuleList(wxArrayString rules);
    void updateTable();
    void updateIOList();
    bool findString(wxArrayString list, wxString item);
    bool inputAssigned(int x,int  y);
    bool outputAssigned(int x,int  y);
    bool findInputAssignment(vector<coordinate> list, int x, int y);
    bool findOutputAssignment(vector<coordinate> list, int x, int y);
    void resetIO();
};

#endif /*LAYOUTMANAGER_HPP_*/
