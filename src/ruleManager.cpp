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

// $Revision: 1.22 $

#include "ruleManager.hpp"
#include <wx/filename.h>


//! Constructor.
/*!
   \param controller the main application controller.
*/
RuleManager::RuleManager(MainController *controller)
{
    nextId = 1;
    currentRule = 0; //No current rule
    modified = false;
    needNewFile = false;
    fileName = wxEmptyString;
    diskManager = new RuleDiskManager(this);
    this->controller = controller;
}


//! Destructor.
RuleManager::~RuleManager()
{
    delete diskManager;
}


//! Creates a new rule.
/*!
   \param width the rule width.
   \param height the rule height.
*/
void RuleManager::newRule(int width, int height)
{
    Rule *newRule = new Rule(width, height);
    ruleList[nextId] = newRule;
    currentRule = nextId;
    nextId++;
    view->updateGrids(newRule->getInitialGrid(), newRule->getFinalGrid(), true);
    
    updateList();
    view->selectRule(currentRule);
    ruleSelected(currentRule);
    modified = true;
    if (fileName == wxEmptyString)
    {
        needNewFile = true;
    }
    controller->rulesChanged();
    controller->elementChanged();
}


//! Sets the rule view.
/*!
   \param view the rule view.
*/
void RuleManager::setView(RuleView *view)
{
    this->view = view;
}


//! Selects a rule.
/*!
   \param ruleId the rule identifier.
*/
void RuleManager::ruleSelected(unsigned int ruleId)
{
    currentRule = ruleId;
    updateGrids(true);
}


//! Cell status change.
/*!
   \param grid the grid to be changed, can be the initial grid or the final grid.
   \param x the x coordinate.
   \param y the y coordinate.
*/
void RuleManager::cellChanged(GridType grid, int x, int y)
{
    switch (grid)
    {
        case nINITIAL:
        {
            ruleList[currentRule]->cellChanged(x,  y, grid);
            break;
        }
        case nFINAL:
        {
            ruleList[currentRule]->cellChanged(x,  y, grid);
            break;
        }
    }

    updateGrids(false);
    modified = true;
    controller->elementChanged();
}


//! Updates the grids of the view to the selected rule.
/*!
   \param changed true if the selected rule has changed, false if
   only the grids have changed.
*/
void RuleManager::updateGrids(bool changed)
{
    Grid initial;
    Grid final;
    if (currentRule > 0)
    {
        initial = ruleList[currentRule]->getInitialGrid();
        final = ruleList[currentRule]->getFinalGrid();
    }
    view->updateGrids(initial, final, changed);
}


//! Deletes the selected rule from the collection.
void RuleManager::removeRule()
{
    delete ruleList[currentRule];
    map<unsigned int, Rule*>::iterator i = ruleList.find(currentRule);
    bool first = (i == ruleList.begin());
    map<unsigned int, Rule*>::iterator j = i;
    map<unsigned int, Rule*>::iterator k = i;
    j++;
    k--;
    ruleList.erase(i);
    
    if (j != ruleList.end())
    {
        currentRule = (*j).first;
    }
    else
    {
        if (first)
        {
            currentRule = 0;//No rules
        }
        else
        {
            currentRule = (*k).first;
        }
    } 
    
    updateList();
    view->selectRule(currentRule);
    ruleSelected(currentRule);
    modified = true;
    controller->rulesChanged();
    controller->elementChanged();
}


//! Updates the view rule list.
void RuleManager::updateList()
{
    wxArrayString stringList;
    
    for (map<unsigned int, Rule*>::iterator i = ruleList.begin(); i != ruleList.end(); i++)
    {
        stringList.Add(wxString::Format(_("Rule %d"), (*i).first));
    }
    
    view->updateList(stringList);
}


//! Copies the initial configuration of the selected rule to its final configuration.

void RuleManager::copyGrids()
{
    if (currentRule > 0)
    {
        ruleList[currentRule]->setFinalGrid(ruleList[currentRule]->getInitialGrid());
        updateGrids(false);
    }
}


//! Saves the rule collection.
/*! 
   If the collection has not been saved before,
   the user is asked for a file name.
   \return True if the file is saved correctly.
*/
bool RuleManager::saveFile()
{
    if (modified)
    {
        if (needNewFile)
        {
            wxString name = fileView.saveFileChoose(view, _("Select a rule file to save"), _("*.rule"));
    
            if (name != wxEmptyString)
            {
                fileName = name;
            }
            else
            {
                return false;
            }
        }
    
        list<Rule*> rules;
    
        for(map<unsigned int, Rule*>::iterator i = ruleList.begin(); i != ruleList.end(); i++)
        {
            rules.push_back((*i).second);
        }
    
        //Set the extension for the filename
        wxFileName extension(fileName);
        extension.SetExt(_("rule"));
        fileName = extension.GetFullPath();
        if (diskManager->saveRules(rules, fileName))
        {
            modified = false;
            needNewFile = false;
        }
        else
        {
            view->errorMsg(_("Error saving rule file!"));
            return false;
        }
    }
    return true;
}


//! Saves the rule collection to a specified file.
/*!
   \return True if the file is saved correctly.
*/   
bool RuleManager::saveFileAs()
{
    wxString name = fileView.saveFileChoose(view, _("Select a rule file to save"), _("*.rule"));
    
    if (name != wxEmptyString)
    {
        fileName = name;
    }
    else
    {
        return false;
    }
    
    list<Rule*> rules;
  
    for(map<unsigned int, Rule*>::iterator i = ruleList.begin(); i != ruleList.end(); i++)
    {
        rules.push_back((*i).second);
    }
    
    //Set the extension for the filename
    wxFileName extension(fileName);
    extension.SetExt(_("rule"));
    fileName = extension.GetFullPath();
    if (diskManager->saveRules(rules, fileName))
    {
        modified = false;
        needNewFile = false;
    }
    else
    {
        view->errorMsg(_("Error saving rule file!"));
        return false;
    }
    return true;
}


//! Opens a rule collection file.
/*!
   The user is asked to append the rules to the existing ones
   or overwrite them.
*/
void RuleManager::openFile()
{
    bool deleteRules = false;;
    //We should ask if the user wants to replace
    //the rules or if he wants to append them to
    //existing
    if (ruleList.size() > 0)
    {
        if (!view->msgYesNo(_("Do you want to append the new rules into the existing ones? (Selecting no will delete them)")))
        {
            deleteRules = true;
            //We have to delete existing rules
            //Check if the user wants to save modified rules
            if (modified)
            {
                if (view->msgYesNo(_("Do you want to save modified rules?")))
                {
                    saveFile();
                }
            }
        }
        else
        {
            deleteRules = false;
        }
        
        if (!appendRules(deleteRules, false))
        {
            view->errorMsg(_("Error opening rule file"));
            clean();
        }
    }
    else
    {
        if (!appendRules(deleteRules, true))
        {
            view->errorMsg(_("Error opening rule file"));
            clean();
        }
    }
    controller->elementChanged();
}


//! Opens a rule collection file for a new project.
/*!
   \param file the file name.
   \return True if the file is opened correctly.
*/
bool RuleManager::openPFile(wxString file)
{
    bool result;
    deleteRules();
    fileName = file;
    result = diskManager->openRules(file);
    updateList();
    view->selectRule(currentRule);
    ruleSelected(currentRule);
    controller->elementChanged();
    return result;
}


//! Deletes all the rules from the collection.
void RuleManager::deleteRules()
{
    ruleList.clear();
    nextId = 1;
    currentRule = 0; //No current rule
    controller->elementChanged();
}


//! Appends a rule file
/*!
   \param mustDeleteRules True if the existing rules hve to be deleted.
   \param newFile If true, the opened filename is used as the new rule
   collection file name, if not, the current file name is mantained.
   \return True if the rules were correctly opened.
*/
bool RuleManager::appendRules(bool mustDeleteRules, bool newFile)
{
    bool result = true;
    wxString name =fileView.openFileChoose(view, _("Select a rule file to open"), _("*.rule"));
    
    if (name != wxEmptyString)
    {
        if (mustDeleteRules)
        {
            deleteRules();
        }
        //Set te new filename if necessary
        if (newFile)
        {
            fileName = name;
        }
        
        result = diskManager->openRules(name);
    }
    
    updateList();
    view->selectRule(currentRule);
    ruleSelected(currentRule);
    modified = true;
    
    controller->elementChanged();
    return result;    
}


//! Adds a new rule to the collection.
/*!
   \param rule the rule to add.
*/
void RuleManager::newRule(Rule *rule)
{
    ruleList[nextId] = rule;
    currentRule = nextId;
    nextId++;
    modified = true;
    controller->rulesChanged();
    controller->elementChanged();
}


//! Member accessor.
/*!
   \return The rule collection list strings.
*/
wxArrayString RuleManager::getRuleList()
{
    wxArrayString result;
    
    for (map<unsigned int, Rule*>::iterator i = ruleList.begin(); i != ruleList.end(); i++)
    {
        result.Add(wxString::Format(_("Rule %d"), (*i).first));
    }
    
    return result;  
}


//! Member accessor.
/*!
   \return True if the collection was changed since last save.
*/
bool RuleManager::isModified()
{
    return modified;
}


//! Member accessor.
/*!
   \return The rule collection file name.
*/
wxString RuleManager::getFileName()
{
    return fileName;
}


//! Saves the rule collection to a file.
/*!
   \param file the file to save.
   \return True if the rules were saved correctly.
*/
bool RuleManager::saveFilename(wxString file)
{
    list<Rule*> rules;
    
    for(map<unsigned int, Rule*>::iterator i = ruleList.begin(); i != ruleList.end(); i++)
    {
        rules.push_back((*i).second);
    }
    
    if (diskManager->saveRules(rules, file))
    {
        fileName = file;
        return true;
    }
    else
    {
        return false;
    }
}


//! Deletes the rule collection and cleans all the controller
//! information.
void RuleManager::clean()
{
    fileName = wxEmptyString;
    deleteRules();
    updateList();
    view->selectRule(currentRule);
    ruleSelected(currentRule);
    controller->rulesChanged();
    controller->elementChanged();
    modified = false;
}


//! Checks the rule collection for invalid or repeated rules.
/*!
   \return True iif all the rules are valid and unique.
*/
bool RuleManager::checkRules()
{
    map<int, bool> errors;
    
    for (map<unsigned int, Rule*>::iterator i = ruleList.begin(); i != ruleList.end(); i++)
    {
        errors[(*i).first] = true;
    }
    
    //error[index] is the error or no error of the index pattern
    for (map<unsigned int, Rule*>::iterator i = ruleList.begin(); i != ruleList.end(); i++)
    {
        if ((*i).second->sameInitialFinal())
        {
            view->setListMessage((*i).first, _(" - Initial equals to final"));
            errors[(*i).first] = false;
        }
        else if (!(*i).second->valid())
        {
            view->setListMessage((*i).first, _(" - Invalid"));
            errors[(*i).first] = false;
        }
        map<unsigned int, Rule*>::iterator j = i;
        j++;
        while (j != ruleList.end())
        {
            if ((*i).second->equals((*j).second))
            {
                view->setListMessage((*j).first, wxString::Format(_(" - Repeated (%d)"), (*i).first));
                errors[(*j).first] = false;
            }
            j++;
        }
    }
    
    bool result = true;
    for (map<unsigned int, Rule*>::iterator i = ruleList.begin(); i != ruleList.end(); i++)
    {
        if (errors[(*i).first])
        {
            view->resetListItem((*i).first);
        }
        else
        {
            result = false;
        }
    }
    return result;
}


//! Returns a rule from the collection.
/*!
   \param id the rule identifier.
   \return A pointer to the rule identified by id.
*/
Rule * RuleManager::getRule(unsigned int id)
{
    if (ruleList.find(id) != ruleList.end())
    {
        return ruleList[id];
    }
    else
    {
        return NULL;
    }
}
