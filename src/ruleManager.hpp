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
 * \class RuleManager
 * \brief Main controller for the rules.
 * 
 * This class is the main controller for the rule collection. It manages it's view
 * and data persistance layers and also provides services to the
 * other controllers.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.14 $
 */

#ifndef RULEMANAGER_HPP_
#define RULEMANAGER_HPP_

#include "rule.hpp"
#include "ruleView.hpp"
#include "fileManagerView.hpp"
#include "ruleDiskManager.hpp"
#include "mainController.hpp"
#include <map>

using namespace std;

class RuleView;
class RuleDiskManager;
class MainController;

class RuleManager
{
public:
	RuleManager(MainController *controller);
	virtual ~RuleManager();
    void setView(RuleView *view);
    void newRule(int width, int height);
    void newRule(Rule *rule);
    void ruleSelected(unsigned int ruleId);
    void cellChanged(GridType grid, int x, int y);
    void removeRule();
    void copyGrids();
    bool saveFile();
    bool saveFileAs();
    void openFile();
    bool openPFile(wxString file);
    bool saveFilename(wxString file);
    wxArrayString getRuleList();
    bool isModified();
    wxString getFileName();
    void clean();
    bool checkRules();
    Rule *getRule(unsigned int id);
    
private:
    //! Rule view.
    RuleView *view;
    //! File System interation view.
    FileManagerView fileView;
    //! Data persistance.
    RuleDiskManager *diskManager;
    //! Main application controller.
    MainController *controller;
    //! The rule collection.
    map<unsigned int, Rule*> ruleList;
    //! Next identifier to be used for a new rule.
    unsigned int nextId;
    //! Current selected rule.
    unsigned int currentRule;
    //! Has been the collection been modified after saving?
    bool modified;
    //! Do we need a filename for saving the rule collection?
    bool needNewFile;
    //! FIlename of the rule collection.
    wxString fileName;
    void updateGrids(bool changed);
    void updateList();
    void deleteRules();
    bool appendRules(bool mustDeleteRules, bool newFile);
};

#endif /*RULEMANAGER_HPP_*/
