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

// $Revision: 1.19 $

#include "truthTableManager.hpp"
#include <wx/filename.h>
#include <list>

//! Constructor.
/*!
   \param controller the main application controller.
*/
TruthTableManager::TruthTableManager(MainController *controller)
{
    currentTable = wxEmptyString; //No tables
    modified = false;
    needNewFile = false;
    fileName = wxEmptyString;
    diskManager = new TruthTableDiskManager(this);
    this->controller = controller;
}


//! Destructor
TruthTableManager::~TruthTableManager()
{
    delete diskManager;
}


//! Sets the table view.
/*!
   \param view the truth table view.
*/
void TruthTableManager::setView(TruthTableView *view)
{
    this->view = view;
}


//! Updates the table on the view to the selected one.
void TruthTableManager::updateTable()
{
    if (currentTable != wxEmptyString)
    {
        view->updateTable(*tableList[currentTable]);
    }
    else
    {
        view->updateTable(TruthTable());
    }
}


//! Creates a new table.
/*!
   \param name table name.
   \param inputs table input number.
   \param outputs table output number.
*/
void TruthTableManager::newTable(wxString name, int inputs, int outputs)
{
    if (tableList.find(name) != tableList.end())
    {
        view->errorMsg(_("This name is already taken, please select another one"));
        return;
    }
    wxStopWatch s1;
    TruthTable *newTable = new TruthTable(name, inputs, outputs);
    tableList[name] = newTable;
    currentTable = name;
    wxStopWatch s2;
    view->updateTable(*newTable);
    
    updateList();
    view->selectTable(currentTable);
    tableSelected(currentTable);
    if (fileName == wxEmptyString)
    {
        needNewFile = true;
    }
    modified = true;
    controller->tablesChanged();
    controller->elementChanged();
}


//! Updates the table list on the view.
void TruthTableManager::updateList()
{
    wxArrayString stringList;
    for (map<wxString, TruthTable*>::iterator i = tableList.begin(); i != tableList.end(); i++)
    {
        stringList.Add((*i).second->getName());
    }
    view->updateList(stringList);
}


//! Selects a table.
/*!
   \param tableId the table name (identifier).
*/
void TruthTableManager::tableSelected(wxString tableId)
{
    currentTable = tableId;
    updateTable();    
}


//! Deletes the selected table.
void TruthTableManager::removeTable()
{
    delete tableList[currentTable];
    map<wxString, TruthTable*>::iterator i = tableList.find(currentTable);
    bool first = (i == tableList.begin());
    map<wxString, TruthTable*>::iterator j = i;
    map<wxString, TruthTable*>::iterator k = i;
    j++;
    k--;
    tableList.erase(i);
    if (j != tableList.end())
    {
        currentTable = (*j).first;
    }
    else
    {
        if (first)
        {
            currentTable = wxEmptyString;//No Tables
        }
        else
        {
            currentTable = (*k).first;
        }
    } 
    updateList();
    view->selectTable(currentTable);
    tableSelected(currentTable);
    modified = true;
    controller->tablesChanged();
    controller->elementChanged();
}


//! Changes the status of the selected table.
/*!
   \param input the input combination.
   \param output the new output values to set at the given input combination.
*/
void TruthTableManager::tableChanged(vector<bool> input, int output)
{
    vector<bool> newOutput = tableList[currentTable]->getOutput(input);
    newOutput[output] = !newOutput[output];
    tableList[currentTable]->setOutput(input, newOutput);
    modified = true;
    //Optimization: since we already update the table on the
    //event we don't need to update it.
    //updateTable();
    controller->elementChanged();
}


//! Saves the table collection.
/*!
   If the collection has not been saved before, the user is asked to select
   a file name. Otherwise, the last file name is used.
   \return True iif the collection was correctly saved.
*/
bool TruthTableManager::saveFile()
{
    if (modified)
    {
        if (needNewFile)
        {
            wxString name = fileView.saveFileChoose(view, _("Select a truth table file to save"), _("*.ttb"));
    
            if (name != wxEmptyString)
            {
                fileName = name;
            }
            else
            {
                return false;
            }
        }
    
        list<TruthTable*> tables;
    
        for(map<wxString, TruthTable*>::iterator i = tableList.begin(); i != tableList.end(); i++)
        {
            tables.push_back((*i).second);
        }
    
        //Set the extension for the filename
        wxFileName extension(fileName);
        extension.SetExt(_("ttb"));
        fileName = extension.GetFullPath();
        if (diskManager->saveTables(tables, fileName))
        {
            modified = false;
            needNewFile = false;
        }
        else
        {
            view->errorMsg(_("Error truth table pattern file!"));
            return false;
        }
    }
    return true;
}


//! Saves the collection to a specified file.
/*!
   \return True iif the collection was saved correctly.
*/
bool TruthTableManager::saveFileAs()
{
    wxString name = fileView.saveFileChoose(view, _("Select a truth table file to save"), _("*.ttb"));
    
    if (name != wxEmptyString)
    {
        fileName = name;
    }
    else
    {
        return false;
    }
    
    list<TruthTable*> tables;
  
    for(map<wxString, TruthTable*>::iterator i = tableList.begin(); i != tableList.end(); i++)
    {
        tables.push_back((*i).second);
    }
    
    //Set the extension for the filename
    wxFileName extension(fileName);
    extension.SetExt(_("ttb"));
    fileName = extension.GetFullPath();
    if (diskManager->saveTables(tables, fileName))
    {
        modified = false;
        needNewFile = false;
    }
    else
    {
        view->errorMsg(_("Error saving truth table file!"));
        return false;
    }
    return true;
}


//! Opens a truth table file.
void TruthTableManager::openFile()
{
    bool deleteTables = false;;
    //We should ask if the user wants to replace
    //the tables or if he wants to append them to
    //existing
    if (tableList.size() > 0)
    {
        if (!view->msgYesNo(_("Do you want to append the new truth tables into the existing ones? (Selecting no will delete them)")))
        {
            deleteTables = true;
            //We have to delete existing tables
            //Check if the user wants to save modified tables
            if (modified)
            {
                if (view->msgYesNo(_("Do you want to save modified truth tables?")))
                {
                    saveFile();
                }
            }
        }
        else
        {
            deleteTables = false;
        }
        
        if (!appendTables(deleteTables, false))
        {
            view->errorMsg(_("Error opening truth table file"));
            clean();
        }
    }
    else
    {
        if (!appendTables(deleteTables, true))
        {
            view->errorMsg(_("Error opening truth table file"));
            clean();
        }
    }
    controller->elementChanged();
}


//! Opens a truth table file for a new project.
/*!
   \param file the file name to open.
   \return True iif the file was opened correctly.
*/
bool TruthTableManager::openPFile(wxString file)
{
    bool result;
    deleteTables();
    fileName = file;
    result = diskManager->openTables(file);
    updateList();
    view->selectTable(currentTable);
    tableSelected(currentTable);
    controller->elementChanged();
    return result;
}


//! Deletes all the tables from the collection.
void TruthTableManager::deleteTables()
{
    tableList.clear();
    currentTable = wxEmptyString; //No current table
    controller->elementChanged();
}


//! Opens the tables from a file, adding them to the collection.
/*!
   The actual tables are conserved or deleted.
   \param mustDeleteTables if true, the tables are deleted before 
   adding the new ones.
   \param newFile if true, the file opened will be set as the tables file name.
*/
bool TruthTableManager::appendTables(bool mustDeleteTables, bool newFile)
{
    wxString name =fileView.openFileChoose(view, _("Select a truth table file to open"), _("*.ttb"));
    
    if (name != wxEmptyString)
    {
        if (mustDeleteTables)
        {
            deleteTables();
        }
        //Set te new filename if necessary
        if (newFile)
        {
            fileName = name;
        }
        
        diskManager->openTables(name);
    }
    
    updateList();
    view->selectTable(currentTable);
    tableSelected(currentTable);
    modified = true;
    controller->elementChanged();
    return true;    
}


//TODO: check if the table name is available
//! Creates a new table.
/*!
   \param table the new table.
*/
void TruthTableManager::newTable(TruthTable *table)
{
    if (tableList.find(table->getName()) != tableList.end())
    {
        view->errorMsg(_("A table with name ") + table->getName() + _(" is already taken, it won't be added"));
        return;
    }
    tableList[table->getName()] = table;
    currentTable = table->getName();
    controller->tablesChanged();
    modified = true;
    controller->elementChanged();
}


//! Returns the table names list.
/*!
   \return The table names list.
*/
wxArrayString TruthTableManager::getTableList()
{
    wxArrayString result;
    
    for (map<wxString, TruthTable*>::iterator i = tableList.begin(); i != tableList.end(); i++)
    {
        result.Add((*i).first);
    }
    
    return result;  
}


//! Returns the input number of a table.
/*!
   \param tableId the table name.
   \return The number of inputs of the table
*/
int TruthTableManager::getInputs(wxString tableId)
{
    if (tableList.find(tableId) == tableList.end())
    {
        return 0;
    }
    else
    {
        return tableList[tableId]->getInputs();
    }
}


//! Returns the output number of a table.
/*!
   \param tableId the table name.
   \return The number of outputs of the table
*/
int TruthTableManager::getOutputs(wxString tableId)
{
    if (tableList.find(tableId) == tableList.end())
    {
        return 0;
    }
    else
    {
        return tableList[tableId]->getOutputs();
    }
}


//! Modified status.
/*!
   \return True if the collectin has not been modified since last save.
   False otherwise.
*/
bool TruthTableManager::isModified()
{
    return modified;
}


//! Returns the tables file name.
/*!
   \return The tables file name.
*/
wxString TruthTableManager::getFileName()
{
    return fileName;
}


//! Saves the table collection to the file.
/*!
   \param file the file to save.
   \return True iif the tables were saved correctly.
*/
bool TruthTableManager::saveFilename(wxString file)
{
    list<TruthTable*> tables;
    
    for(map<wxString, TruthTable*>::iterator i = tableList.begin(); i != tableList.end(); i++)
    {
        tables.push_back((*i).second);
    }
    
    if (diskManager->saveTables(tables, file))
    {
        fileName = file;
        return true;
    }
    else
    {
        return false;
    }
}


//! Deletes all the tables and information.
void TruthTableManager::clean()
{
    fileName = wxEmptyString;
    deleteTables();
    updateList();
    view->selectTable(currentTable);
    tableSelected(currentTable);
    controller->tablesChanged();
    modified = false;
}


//! Returns a truth table.
/*!
   \param id the table name.
   \return The table identfied by id.
*/
TruthTable * TruthTableManager::getTable(wxString id)
{
    if (tableList.find(id) != tableList.end())
    {
        return tableList[id];
    }
    else
    {
        return NULL;
    }
}
