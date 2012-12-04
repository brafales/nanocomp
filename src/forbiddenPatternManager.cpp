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

// $Revision: 1.20 $

#include "forbiddenPatternManager.hpp"
#include <wx/filename.h>


//! Constructor.
/*!
  \param controller the main application controller.
*/
ForbiddenPatternManager::ForbiddenPatternManager(MainController *controller)
{
    nextId = 1;
    currentFP = 0; //No FP
    modified = false;
    needNewFile = false;
    fileName = wxEmptyString;
    diskManager = new ForbiddenPatternDiskManager(this);
    this->controller = controller;
}


//! Destructor.
ForbiddenPatternManager::~ForbiddenPatternManager()
{
    delete diskManager;
}


//! Sets the forbidden pattern view.
/*!
  \param view the forbidden pattern view.
*/
void ForbiddenPatternManager::setView(ForbiddenPatternView *view)
{
    this->view = view;
}


//! Changes the status of a cell of the pattern.
/*!
  It changes the status of the cell at coordinates x and y.
  The status graph is ENABLED->DISABLED->DON'T CARE->ENABLED
  \param x the x coordinate.
  \param y the y coordinate.
*/
void ForbiddenPatternManager::cellChanged(int x, int y)
{
    FPList[currentFP]->cellChanged(x,  y);
    updateGrids(false);
    modified = true;
    controller->elementChanged();
}


//! Updates the grid of the view to the current FP.
/*!
   \param changed indicates if the update is due to a status change
   or due to a pattern select event, the view needs this information
   to know if it has to mantain the scrollbar positions or not.
*/
void ForbiddenPatternManager::updateGrids(bool changed)
{
    Grid grid;
    if (currentFP > 0)
    {
        grid = FPList[currentFP]->getGrid();
    }
    view->updateGrids(grid, changed); 
}


//! Creates a new pattern and adds it to the collection.
/*!
   \param width the pattern width.
   \param height the pattern height.
*/
void ForbiddenPatternManager::newFP(int width, int height)
{
    ForbiddenPattern *newFP = new ForbiddenPattern(width, height);
    //Add the pattern to the collection
    FPList[nextId] = newFP;
    //Assign the new pattern as the selected one
    currentFP = nextId;
    nextId++;
    //Update the view because of the new selection
    view->updateGrids(newFP->getGrid(), true);
    updateList();
    view->selectFP(currentFP);
    FPSelected(currentFP);
    if (fileName == wxEmptyString)
    {
        needNewFile = true;
    }
    modified = true;
    //Notify the controller that the patterns have changed
    controller->FPsChanged();
    controller->elementChanged();
}


//! Updates the pattern list for the view.
void ForbiddenPatternManager::updateList()
{
    wxArrayString stringList;
    for (map<unsigned int, ForbiddenPattern*>::iterator i = FPList.begin(); i != FPList.end(); i++)
    {
        stringList.Add(wxString::Format(_("Pattern %d"), (*i).first));
    }
    view->updateList(stringList);
}

//! Selects a pattern.
/*!
   \param FPId the identifier of the pattern to be selected.
*/
void ForbiddenPatternManager::FPSelected(unsigned int FPId)
{
    currentFP = FPId;
    updateGrids(true);
}


//! Deletes the pattern currently selected.
void ForbiddenPatternManager::removeFP()
{
    //Delete the pattern
    delete FPList[currentFP];
    //Here we choose the new selected pattern
    map<unsigned int, ForbiddenPattern*>::iterator i = FPList.find(currentFP);
    bool first = (i == FPList.begin());
    //Create two iterators because we have to delete an element
    //pointed by this one and then the iterator comes invalid
    map<unsigned int, ForbiddenPattern*>::iterator j = i;
    map<unsigned int, ForbiddenPattern*>::iterator k = i;
    j++;
    k--;
    FPList.erase(i);
    if (j != FPList.end())
    {
        currentFP = (*j).first;
    }
    else
    {
        //We deleted the only pattern left
        if (first)
        {
            currentFP = 0;//No FPs
        }
        else
        {
            currentFP = (*k).first;
        }
    } 
    //Update view
    updateList();
    view->selectFP(currentFP);
    FPSelected(currentFP);
    modified = true;
    //Notify main controller that patterns have changed
    controller->FPsChanged();
    controller->elementChanged();
}


//! Saves the pattern collection to a file.
/*!
    If the pattern collection has been saved previously
    it's saved on the same file. If not, the user is asked 
    to enter a file name to save the collection to.
    \return True iif the file was correctly saved.
*/
bool ForbiddenPatternManager::saveFile()
{
    if (modified)
    {
        if (needNewFile)
        {
            wxString name = fileView.saveFileChoose(view, _("Select a forbidden pattern file to save"), _("*.rule"));
    
            if (name != wxEmptyString)
            {
                fileName = name;
            }
            else
            {
                return false;
            }
        }
    
        list<ForbiddenPattern*> FPs;
    
        for(map<unsigned int, ForbiddenPattern*>::iterator i = FPList.begin(); i != FPList.end(); i++)
        {
            FPs.push_back((*i).second);
        }
        
        //Set the extension for the filename
        wxFileName extension(fileName);
        extension.SetExt(_("rule"));
        fileName = extension.GetFullPath();
        if (diskManager->saveFPs(FPs, fileName))
        {
            modified = false;
            needNewFile = false;
        }
        else
        {
            view->errorMsg(_("Error saving forbidden pattern file!"));
            return false;
        }
    }
    return true;
}


//! Saves the pattern collection to a file.
/*!
    The user is asked 
    to enter a file name to save the collection to. 
    \return True iif the file was correctly saved.
*/
bool ForbiddenPatternManager::saveFileAs()
{
    wxString name = fileView.saveFileChoose(view, _("Select a forbidden pattern file to save"), _("*.rule"));
    
    if (name != wxEmptyString)
    {
        fileName = name;
    }
    else
    {
        return false;
    }
    
    list<ForbiddenPattern*> FPs;
  
    for(map<unsigned int, ForbiddenPattern*>::iterator i = FPList.begin(); i != FPList.end(); i++)
    {
        FPs.push_back((*i).second);
    }
   
    //Set the extension for the filename
    wxFileName extension(fileName);
    extension.SetExt(_("rule"));
    fileName = extension.GetFullPath();
    if (diskManager->saveFPs(FPs, fileName))
    {
        modified = false;
        needNewFile = false;
    }
    else
    {
        view->errorMsg(_("Error saving forbidden pattern file!"));
        return false;
    }
    return true;
}


//! Opens a file containing forbidden patterns.
/*!
    The user is asked to select the file.
*/
void ForbiddenPatternManager::openFile()
{
    bool deleteFPs = false;
    //We should ask if the user wants to replace
    //the FPs or if he wants to append them to
    //existing
    if (FPList.size() > 0)
    {
        if (!view->msgYesNo(_("Do you want to append the new forbidden patterns into the existing ones? (Selecting no will delete them)")))
        {
            deleteFPs = true;
            //We have to delete existing FPs
            //Check if the user wants to save modified FPs
            if (modified)
            {
                if (view->msgYesNo(_("Do you want to save modified forbidden patterns?")))
                {
                    saveFile();
                }
            }
        }
        else
        {
            deleteFPs = false;
        }
        
        if (!appendFPs(deleteFPs, false))
        {
            view->errorMsg(_("Error opening forbidden pattern file"));
            clean();
        }
    }
    else
    {
        if (!appendFPs(deleteFPs, true))
        {
            view->errorMsg(_("Error opening forbidden pattern file"));
            clean();
        }
    }
    controller->elementChanged();
}


//! Deletes all the patterns of the collection.
void ForbiddenPatternManager::deleteFPs()
{
    FPList.clear();
    nextId = 1;
    currentFP = 0; //No current FP
    controller->elementChanged();
}


//! Deletes all the patterns of the collection and opens a file.
/*!
    This method is used when a new project is opened. The controller
    must delete all the patterns before opening the ones in the file.
    \param file pattern file to be opened.
    \return True iif the file was correctly opened.
*/
bool ForbiddenPatternManager::openPFile(wxString file)
{
    bool result;
    fileName = file;
    deleteFPs();
    result = diskManager->openFPs(file);
    updateList();
    view->selectFP(currentFP);
    FPSelected(currentFP);
    controller->elementChanged();
    return result;
}


//! Appends the patterns of a file to the collection.
/*!
    This method asks the user for a pattern file to be opened
    and, after deleting the patterns of the collection depending
    on a parameter, adds the patterns of the file to the collection.
    This method is called when the user wants to open a pattern file
    from the pattern view.
    \param mustDeleteFPs true if the patterns of the collection have to be deleted before adding the new ones.
    \param newFile true if we have to set the current pattern file name to the file selected by the user.
    \return True iif the file was correctly opened.
*/
bool ForbiddenPatternManager::appendFPs(bool mustDeleteFPs, bool newFile)
{
    bool result = true;
    wxString name =fileView.openFileChoose(view, _("Select a forbidden pattern file to open"), _("*.rule"));
    
    if (name != wxEmptyString)
    {
        if (mustDeleteFPs)
        {
            deleteFPs();
        }
        //Set te new filename if necessary
        if (newFile)
        {
            fileName = name;
        }
        
        result = diskManager->openFPs(name);
    }
    
    updateList();
    view->selectFP(currentFP);
    FPSelected(currentFP);
    modified = true;
    
    controller->elementChanged();
    return result;    
}


//! Adds a pattern to the collection.
/*!
    \param fp the pattern to be added.
*/
void ForbiddenPatternManager::newFP(ForbiddenPattern *fp)
{
    FPList[nextId] = fp;
    currentFP = nextId;
    nextId++;
    modified = true;
    controller->FPsChanged();
    controller->elementChanged();
}


//! Returns a string list of the patterns of the collection.
/*!
    \return A string list of the patterns of the collection.
*/
wxArrayString ForbiddenPatternManager::getFPList()
{
    wxArrayString result;
    
    for (map<unsigned int, ForbiddenPattern*>::iterator i = FPList.begin(); i != FPList.end(); i++)
    {
        result.Add(wxString::Format(_("Pattern %d"), (*i).first));
    }
    
    return result;  
}


//! Modified status.
/*!
    \return True iif the collection was modified after last save or after it was opened.
*/
bool ForbiddenPatternManager::isModified()
{
    return modified;
}


//! Member accessor.
/*!
    \return The file name of the collection.
*/
wxString ForbiddenPatternManager::getFileName()
{
    return fileName;
}


//! Saves the collection to the file.
/*!
    \param file the file to be saved.
    \return True iif the collection was correctly saved.
*/
bool ForbiddenPatternManager::saveFilename(wxString file)
{
    list<ForbiddenPattern*> FPs;
    for(map<unsigned int, ForbiddenPattern*>::iterator i = FPList.begin(); i != FPList.end(); i++)
        {
            FPs.push_back((*i).second);
        }
        
        if (diskManager->saveFPs(FPs, file))
        {
            fileName = file;
            return true;
        }
        else
        {
            return false;
        }
}


//! Resets the controller.
/*!
    Deletes all the patterns and resets all the members.
    Used when creating a new project.
*/
void ForbiddenPatternManager::clean()
{
    fileName = wxEmptyString;
    deleteFPs();
    updateList();
    view->selectFP(currentFP);
    FPSelected(currentFP);
    controller->FPsChanged();
    controller->elementChanged();
    modified = false;
}


//! Checks the validity of the pattern collection.
/*!
    \return True iif the collection is valid. The collection is valid if it does not contain any repeated pattern.
*/
bool ForbiddenPatternManager::checkPatterns()
{
    map<int, bool> errors;
        
    for (map<unsigned int, ForbiddenPattern*>::iterator i = FPList.begin(); i != FPList.end(); i++)
    {
        errors[(*i).first] = true;
    }
    
    //error[index] is the error or no error of the index pattern
    for (map<unsigned int, ForbiddenPattern*>::iterator i = FPList.begin(); i != FPList.end(); i++)
    {
        map<unsigned int, ForbiddenPattern*>::iterator j = i;
        j++;
        while (j != FPList.end())
        {
            if ((*i).second->equals((*j).second))
            {
                cout << (*i).first << " is equal to " << (*j).first << endl;
                view->setListMessage((*j).first, wxString::Format(_("-Repeated (%d)"), (*i).first));
                errors[(*j).first] = false;
            }
            j++;
        }
    }
    
    bool result = true;
    for (map<unsigned int, ForbiddenPattern*>::iterator i = FPList.begin(); i != FPList.end(); i++)
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


//! Pattern access.
/*!
    \param id the pattern identifier.
    \return The forbidden pattern identified by the parameter.
*/
ForbiddenPattern* ForbiddenPatternManager::getFP(unsigned int id)
{
    if (FPList.find(id) != FPList.end())
    {
        return FPList[id];
    }
    else
    {
        return NULL;
    }
}
