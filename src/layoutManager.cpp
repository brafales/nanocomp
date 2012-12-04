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

// $Revision: 1.24 $

#include "layoutManager.hpp"
#include <wx/filename.h>

//! Constructor.
/*!
   \param controller the main application controller.
   \param tableManager the truth table controller.
   \param FPManager the forbidden pattern controller.
   \param ruleManager the rule controller.
*/
LayoutManager::LayoutManager(MainController *controller, TruthTableManager *tableManager, ForbiddenPatternManager *FPManager, RuleManager *ruleManager)
{
    this->tableManager = tableManager;
    this->FPManager = FPManager;
    this->ruleManager = ruleManager;
    this->controller = controller;
    layoutModified = false;
    layoutNeedNewFile = false;
    layoutFileName = wxEmptyString;
    diskManager = new LayoutDiskManager(this);
    configuration = NULL;
    assigningInput = false;
    assigningOutput = false;
    currentTable = wxEmptyString;
    currentInput = 0; //No input
    currentOutput = 0; //No output
}


//! Destructor.
LayoutManager::~LayoutManager()
{
    delete diskManager;
}


//! View setting.
/*!
   Use this method to assign the view of the controller.
   \param view the view of the controller.
*/
void LayoutManager::setView(LayoutView *view)
{
    this->view = view;
}


//! Cell status change.
/*!
   The assigned inputs and outputs are passed to the
   space to control the (de)assignation. After the status
   has changed the (de)assignation is checked again.
   \param x the x coordinate.
   \param y the y coordinate.
*/
void LayoutManager::cellChanged(int x, int y)
{
    bool result;
    vector<coordinate> vInputs;
    vector<coordinate> vOutputs;
    //TODO: si currentTable = wxEmptyString passar vectors buits o s'assigna un vector buit al mapa i em fuckeja
    if (currentTable != wxEmptyString)
    {
        vInputs = tableInputs[currentTable];
        vOutputs = tableOutputs[currentTable];
    }
    result = configuration->cellChanged(x, y, assigningInput, assigningOutput, vInputs, vOutputs);
    layoutModified = true;
    if (assigningInput && result)
    {
        //Check if input assigned there
        if (inputAssigned(x, y))
        {
            controller->setStatusMessage(wxString::Format(_("Input already assigned in %d, %d"), x, y));
        }
        else
        {
            //Check if assignation or de-assignation
            if (tableInputs[currentTable][currentInput].x == x && tableInputs[currentTable][currentInput].y == y)
            {
                //Deassign
                tableInputs[currentTable][currentInput].x = -1;
                tableInputs[currentTable][currentInput].y = -1;
                controller->setStatusMessage(wxString::Format(_("Input %d de-assigned from %d, %d"), currentInput + 1, x, y));
            }
            else
            {
                //Assign ONLY if its non assigned 
                //(else we would be assigning one input to two cells
                if (tableInputs[currentTable][currentInput].x == -1)
                {
                    tableInputs[currentTable][currentInput].x = x;
                    tableInputs[currentTable][currentInput].y = y;
                    controller->setStatusMessage(wxString::Format(_("Input %d assigned to %d, %d"), currentInput + 1, x, y));
                }
                else
                {
                    controller->setStatusMessage(wxString::Format(_("Input %d already assigned"), currentInput + 1));
                }
            }
        }
        assigningInput = false;
        view->onlyAssign(true);
    }
    if (assigningOutput && result)
    {
        //Check if output assigned there
        if (outputAssigned(x, y))
        {
            controller->setStatusMessage(wxString::Format(_("Output already assigned in %d, %d"), x, y));
        }
        else
        {
            if (tableOutputs[currentTable][currentOutput].x == x && tableOutputs[currentTable][currentOutput].y == y)
            {
                //Deassign
                tableOutputs[currentTable][currentOutput].x = -1;
                tableOutputs[currentTable][currentOutput].y = -1;
                controller->setStatusMessage(wxString::Format(_("Output %d de-assigned from %d, %d"), currentInput + 1, x, y));
            }
            else
            {
                //Assign
                if (tableOutputs[currentTable][currentOutput].x == -1)
                {
                    tableOutputs[currentTable][currentOutput].x = x;
                    tableOutputs[currentTable][currentOutput].y = y;
                    controller->setStatusMessage(wxString::Format(_("Output %d assigned to %d, %d"), currentInput + 1, x, y));
                }
                else
                {
                    controller->setStatusMessage(wxString::Format(_("Output %d already assigned"), currentInput + 1));
                }
            }
        }
        assigningOutput = false;
        view->onlyAssign(true);
    }
    if (currentTable != wxEmptyString)
    {
        view->updateInputs(tableInputs[currentTable]);
        view->updateOutputs(tableOutputs[currentTable]);
    }
    updateGrids(false);
    view->updateCanvas();
    controller->elementChanged();
}


//! Updates the grid on the view.
/*!
   \param changed True iif the grid is from another space.
*/
void LayoutManager::updateGrids(bool changed)
{
    Grid grid;
    grid = configuration->getGrid();
    view->updateGrids(grid, changed);
    
}


//! Updates the truth table list.
/*!
   Some control variables have to be reset according
   to the new list: init data structures for new tables
   and remove the ones from the tables deleted.
   \param tables new table list.
*/
void LayoutManager::updateTTList(wxArrayString tables)
{
    //First we delete the ones removed
    map< wxString, vector<coordinate> >::iterator i = tableInputs.begin();
    while (i != tableInputs.end())
    {
        if (!findString(tables, (*i).first))
        //Delete this element
        {
            map< wxString, vector<coordinate> >::iterator j = i;
            i++;
            tableInputs.erase(j);
        }
        else
        {
            i++;
        }
    }
    
    map< wxString, vector<coordinate> >::iterator j = tableOutputs.begin();
    while (j != tableOutputs.end())
    {
        if (!findString(tables, (*j).first))
        //Delete this element
        {
            map< wxString, vector<coordinate> >::iterator k = j;
            j++;
            tableOutputs.erase(k);
        }
        else
        {
            j++;
        }
    }
    
    map<wxString, bool>::iterator k = tableEnabled.begin();
    while (k != tableEnabled.end())
    {
        if (!findString(tables, (*k).first))
        //Delete this element
        {
            map<wxString, bool>::iterator l = k;
            k++;
            tableEnabled.erase(l);
        }
        else
        {
            k++;
        }
    }
    
    //For the new ones we must disable them by default
    for (unsigned int i = 0; i < tables.size(); i++)
    {
        if (tableEnabled.find(tables[i]) == tableEnabled.end())
        {
            tableEnabled[tables[i]] = false;
        }
        if (tableInputs.find(tables[i]) == tableInputs.end())
        {
            //Assign non valid coordinates for io
            int inputs = tableManager->getInputs(tables[i]);
            tableInputs[tables[i]].resize(inputs);
            for (int j = 0; j < inputs; j++)
            {
                tableInputs[tables[i]][j].x = -1;
                tableInputs[tables[i]][j].y = -1;
            }
            int outputs = tableManager->getOutputs(tables[i]);
            tableOutputs[tables[i]].resize(outputs);
            for (int j = 0; j < outputs; j++)
            {
                tableOutputs[tables[i]][j].x = -1;
                tableOutputs[tables[i]][j].y = -1;
            }
        }
    }
    
     view->updateTTList(tableEnabled);
        
    //Update selected table if necessary
    if (currentTable == wxEmptyString)
    {
        if (tables.size() > 0)
        {
            currentTable = tables[tables.size() - 1];
            view->selectTable(tables[tables.size() - 1]);
        }
    }
    else
    {
        if (tableEnabled.find(currentTable) == tableEnabled.end())
        {
            //The selected table was removed
            if (tables.size() > 0)
            {
                currentTable = tables[tables.size() - 1];
                view->selectTable(tables[tables.size() - 1]);
            }
        }
    }
    
    //Update inputs and outputs
    if (currentTable != wxEmptyString)
    {
        view->updateInputs(tableInputs[currentTable]);
        view->updateOutputs(tableOutputs[currentTable]);
    }
    else
    {
        vector<coordinate> empty;
        view->updateInputs(empty);
        view->updateOutputs(empty);
    }
    controller->elementChanged();
}


//! Updates the forbidden pattern list.
/*!
   Some control variables have to be reset according
   to the new list: init data structures for new patterns
   and remove the ones from the patterns deleted.
   \param FPs new pattern list.
*/
void LayoutManager::updateFPList(wxArrayString FPs)
{
    //First we delete the ones removed
    map<unsigned int, bool>::iterator i = FPEnabled.begin();
    while (i != FPEnabled.end())
    {
        if (!findString(FPs, wxString::Format(_("Pattern %d"), (*i).first)))
        //Delete this element
        {
            map<unsigned int, bool>::iterator j = i;
            i++;
            FPEnabled.erase(j);
        }
        else
        {
            i++;
        }
    }
    
   //For the new ones we must enable them by default
   unsigned long int temp;
    for (unsigned int i = 0; i < FPs.size(); i++)
    {
        FPs[i].Mid(8).ToULong(&temp);
        if (FPEnabled.find(temp) == FPEnabled.end())
        {
            FPEnabled[temp] = true;
        }
    }
    
    view->updateFPList(FPEnabled);
    controller->elementChanged();
}


//! Updates the rule list.
/*!
   Some control variables have to be reset according
   to the new list: init data structures for new rules
   and remove the ones from the rules deleted.
   \param rules new rule list.
*/
void LayoutManager::updateRuleList(wxArrayString rules)
{
    //First we delete the ones removed
    map<unsigned int, bool>::iterator i = ruleEnabled.begin();
    while (i != ruleEnabled.end())
    {
        if (!findString(rules, wxString::Format(_("Rule %d"), (*i).first)))
        //Delete this element
        {
            map<unsigned int, bool>::iterator j = i;
            i++;
            ruleEnabled.erase(j);
        }
        else
        {
            i++;
        }
    }
    
   //For the new ones we must enable them by default
   unsigned long int temp;
    for (unsigned int i = 0; i < rules.size(); i++)
    {
        rules[i].Mid(4).ToULong(&temp);
        if (ruleEnabled.find(temp) == ruleEnabled.end())
        {
            ruleEnabled[temp] = true;
        }
    }
    
    view->updateRuleList(ruleEnabled);
    controller->elementChanged();
}


//! Sets a new space.
/*!
   \param width width of the space.
   \param height height of the space.
   \param inputs space input number.
   \param outputs space output number.
*/
void LayoutManager::newLayout(int width, int height, int inputs, int outputs)
{
    //TODO: check for memory leak
    //LayoutConfig *temp = configuration;
    configuration = new LayoutConfig(width, height, inputs, outputs);
    //if (temp)
    //{
    //    delete temp;
    //}
    view->updateGrids(configuration->getGrid(), true);
    if (layoutFileName == wxEmptyString)
    {
        layoutNeedNewFile = true;
    }
    layoutModified = true;
    tablesChanged();
    resetIO();
    if (currentTable != wxEmptyString)
    {
        view->updateInputs(tableInputs[currentTable]);
        view->updateOutputs(tableOutputs[currentTable]);
    }
    else
    {
        vector<coordinate> empty;
        view->updateInputs(empty);
        view->updateOutputs(empty);
    }
    controller->elementChanged();
}


//! Sets a new space.
/*!
   \param layoutConfig the new space.
*/
void LayoutManager::newLayout(LayoutConfig *layoutConfig)
{
    //TODO: check pointers when opening a file
    //LayoutConfig *temp = configuration;
    configuration = layoutConfig;
    //if (temp)
    //{
    //    delete temp;
    //}
    view->updateGrids(configuration->getGrid(), true);
    if (layoutFileName == wxEmptyString)
    {
        layoutNeedNewFile = true;
    }
    //layoutModified = true;
    tablesChanged();
    resetIO();
    if (currentTable != wxEmptyString)
    {
        view->updateInputs(tableInputs[currentTable]);
        view->updateOutputs(tableOutputs[currentTable]);
    }
    else
    {
        vector<coordinate> empty;
        view->updateInputs(empty);
        view->updateOutputs(empty);
    }
    controller->elementChanged();
}


//! Updates the truth table data.
/*!
   This method must be called when the truth table
   collection changes to update important information.
*/
void LayoutManager::tablesChanged()
{
    wxArrayString tableList = tableManager->getTableList();
    wxArrayString validList;
    if (configuration)
    {
        for (unsigned int i = 0; i < tableList.size(); i++)
        {
            if ((tableManager->getInputs(tableList[i]) <= configuration->getInputs()) && (tableManager->getOutputs(tableList[i]) <= configuration->getOutputs()))
            {
                validList.Add(tableList[i]);
            }
        }
    }
    updateTTList(validList);
    updateIOList();
    controller->elementChanged();
}


//! Member accessor.
/*!
   \return The space.
*/
LayoutConfig* LayoutManager::getLayout()
{
    return configuration;
}


//! Opens a space file.
void LayoutManager::openFile()
{
    //Ask for save
    if (layoutModified)
    {
        if (view->msgYesNo(_("Do you want to save modified space?")))
        {
            saveFile();
        }
    }
    
    wxString name =fileView.openFileChoose(view, _("Select a space file to open"), _("*.space"));
    
    if (name != wxEmptyString)
    {
        //Set te new filename if necessary
        layoutFileName = name;
        diskManager->openLayout(name);
    }
    controller->elementChanged();
}


//! Opens a space from a project.
/*!
   \param file file to be opened.
   \return True iif the file was correctly opened.
*/
bool LayoutManager::openPFile(wxString file)
{
    layoutFileName = file;
    controller->elementChanged();
    return diskManager->openLayout(file);
    layoutModified = false;
    layoutNeedNewFile = false;
}


//! Saves the space.
/*!
   The user is asked for a file name if the space
   has not been saved previously.
*/
bool LayoutManager::saveFile()
{
    if (layoutModified)
    {
        if (layoutNeedNewFile)
        {
            wxString name = fileView.saveFileChoose(view, _("Select a space file to save"), _("*.space"));
    
            if (name != wxEmptyString)
            {
                layoutFileName = name;
            }
            else
            {
                return false;
            }
        }
    
        //Set the extension for the filename
        wxFileName extension(layoutFileName);
        extension.SetExt(_("space"));
        layoutFileName = extension.GetFullPath();
        if (diskManager->saveLayout(configuration, layoutFileName))
        {
            layoutModified = false;
            layoutNeedNewFile = false;
        }
        else
        {
            view->errorMsg(_("Error saving space file!"));
            return false;
        }
    }
    return true;
}


//! Saves the space to a new file.
bool LayoutManager::saveFileAs()
{
    wxString name = fileView.saveFileChoose(view, _("Select a space file to save"), _("*.space"));
    
    if (name != wxEmptyString)
    {
        layoutFileName = name;
    }
    else
    {
        return false;
    }
    
    //Set the extension for the filename
    wxFileName extension(layoutFileName);
    extension.SetExt(_("space"));
    layoutFileName = extension.GetFullPath();
    if (diskManager->saveLayout(configuration, layoutFileName))
    {
        layoutModified = false;
        layoutNeedNewFile = false;
    }
    else
    {
        view->errorMsg(_("Error saving space file!"));
        return false;
    }
    return true;
}


//! Updates the input/output data.
/*!
   This method must be called when a truth table
   is selected to update i/o information.
*/
void LayoutManager::updateIOList()
{
    wxArrayString inputs;
    wxArrayString outputs;
    if (configuration)
    {
        if (currentTable != wxEmptyString)
        {     
            for (int i = 0; i < tableManager->getInputs(currentTable); i++)
            {
                inputs.Add(wxString::Format(_("Input %d"), i + 1));
            }
    
            for (int i = 0; i < tableManager->getOutputs(currentTable); i++)
            {
                outputs.Add(wxString::Format(_("Output %d"), i + 1));
            }
        }
    
        currentInput = 0;
        currentOutput = 0;
        view->updateInputList(inputs);
        view->updateOutputList(outputs);
        controller->elementChanged();
    }
}


//! Updates the rule data.
/*!
   This method must be called when the rule
   collection changes to update important information.
*/
void LayoutManager::rulesChanged()
{
    wxArrayString ruleList = ruleManager->getRuleList();
    wxArrayString validList;
    for (unsigned int i = 0; i < ruleList.size(); i++)
    {
           validList.Add(ruleList[i]);
    }
    updateRuleList(validList);
    controller->elementChanged();
}


//! Updates the forbidden pattern data.
/*!
   This method must be called when the forbidden pattern
   collection changes to update important information.
*/
void LayoutManager::FPsChanged()
{
    wxArrayString FPList = FPManager->getFPList();
    wxArrayString validList;
    for (unsigned int i = 0; i < FPList.size(); i++)
    {
       validList.Add(FPList[i]);
    }
    updateFPList(validList);
    controller->elementChanged();
}


//! Sets the assigning input mode on.
 void LayoutManager::assignInput()
 {
    assigningInput = true;
 }
 
 
 //! Sets the assigning output mode on.
 void LayoutManager::assignOutput()
 {
    assigningOutput = true;
 }

 
 //! Method called when the user presses the escape key.
/*!
   This key binding is used to get out of the assigning input/output mode
*/
 void LayoutManager::escapePressed()
 {
    if (assigningInput)
    {
        assigningInput = false;
        view->onlyAssign(true);
    }
    else if (assigningOutput)
    {
        assigningOutput = false;
        view->onlyAssign(true);
    }
 }

 
 //! Selects a truth table.
/*!
   \param table truth table selected.
*/
 void LayoutManager::tableSelected(wxString table)
 {
    currentTable = table;
    updateIOList();
    view->updateInputs(tableInputs[currentTable]);
    view->updateOutputs(tableOutputs[currentTable]);
    view->updateCanvas();
 }


//! Checks or unchecks a truth table.
/*!
   Only one table can be checked, so if checked is true, all other tables
   are unchecked.
   \param table truth table to be (un)checked.
   \param checked true if the table has to be checked, false if has to be
   unchecked.
   \return True if the (un)check was made. False if the item wasn't found.
*/
bool LayoutManager::tableChecked(wxString table, bool checked)
 {
    if (tableEnabled.find(table) == tableEnabled.end())
    {
        return false;
    }
    for (map<wxString, bool>::iterator i = tableEnabled.begin(); i != tableEnabled.end(); i++)
    {
        (*i).second = false;
        view->checkTable((*i).first, false);
    }
    tableEnabled[table] = checked;
    view->checkTable(table, checked);
    controller->elementChanged();
    return true;
 }
 
 
 //! Checks or unchecks a rule.
/*!
   \param rule rule to be (un)checked.
   \param checked true if the rule has to be checked, false if has to be
   unchecked.
   \return True if the (un)check was made. False if the item wasn't found.
*/
 bool LayoutManager::ruleChecked(unsigned int rule, bool checked)
 {
    if (ruleEnabled.find(rule) == ruleEnabled.end())
    {
        return false;
    }
    ruleEnabled[rule] = checked;
    view->checkRule(wxString::Format(_("Rule %d"), rule), checked);
    return true;
 }
 
 
 //! Checks or unchecks a pattern.
/*!
   \param FP pattern to be (un)checked.
   \param checked true if the pattern has to be checked, false if has to be
   unchecked.
   \return True if the (un)check was made. False if the item wasn't found.
*/
 bool LayoutManager::FPChecked(unsigned int FP, bool checked)
 {
    if (FPEnabled.find(FP) == FPEnabled.end())
    {
        return false;
    }
    FPEnabled[FP] = checked;
    view->checkFP(wxString::Format(_("Pattern %d"), FP), checked);
    return true;
 }

 
 //! Finds a string on a list of strings.
/*!
   \param list list of strings.
   \param item string to be found.
   \return True iif the string was found on the list.
*/
 bool LayoutManager::findString(wxArrayString list, wxString item)
 {
    for (unsigned int i = 0; i < list.size(); i++)
    {
        if (list[i] == item)
        {
            return true;
        }
    }
    return false;
 }

 
 //! Selects an input.
/*!
   \param input input to be selected.
*/
  void LayoutManager::inputSelected(int input)
 {
    currentInput = input;
 }

 
 //! Selects an output.
/*!
   \param output output to be selected.
*/
  void LayoutManager::outputSelected(int output)
 {
    currentOutput = output;
 }

 
 //! Modification consult.
/*!
   \return True iif the space has been modified since last save.
*/
 bool LayoutManager::isLayoutModified()
{
    return layoutModified;
}


//! Input assignation check.
/*!
   \param x x coordinate of the input.
   \param y y coordinate of the input.
   \return True iif the input at coordinates x, y is assigned. False otherwise.
*/
bool LayoutManager::inputAssigned(int x,int  y)
{
    return findInputAssignment(tableInputs[currentTable], x, y);
}


//! Output assignation check.
/*!
   \param x x coordinate of the output.
   \param y y coordinate of the output.
   \return True iif the output at coordinates x, y is assigned. False otherwise.
*/
bool LayoutManager::outputAssigned(int x,int  y)
{
    return findOutputAssignment(tableOutputs[currentTable], x, y);
}


//! Finds an input assignation on a list of coordinates.
/*!
   This method is used to know if an input other than the current one
   has been assigned at x, y.
   \param list the coordinate list.
   \param x x coordinate.
   \param y y coordinate.
   \return True if an input other than the current one has been assigned at x, y.
*/
bool LayoutManager::findInputAssignment(vector<coordinate> list, int x, int y)
{
    for (unsigned int i = 0; i < list.size();i++ )
    {
        if ((int)i != currentInput)
        {
            if ((list[i].x == x) && (list[i].y == y))
            {
                return true;
            }
        }
    }
    return false;
}


//! Finds an output assignation on a list of coordinates.
/*!
   This method is used to know if an output other than the current one
   has been assigned at x, y.
   \param list the coordinate list.
   \param x x coordinate.
   \param y y coordinate.
   \return True if an output other than the current one has been assigned at x, y.
*/
bool LayoutManager::findOutputAssignment(vector<coordinate> list, int x, int y)
{
    for (unsigned int i = 0; i < list.size();i++ )
    {
        if ((int)i != currentOutput)
        {
            if ((list[i].x == x) && (list[i].y == y))
            {
                return true;
            }
        }
    }
    return false;
}


//! Returns the space file name.
/*!
   \return The space file name. An empty string if the space has not been
   saved previously.
*/
wxString LayoutManager::getFileName()
{
    return layoutFileName;
}


//! Returns the input assignations.
/*!
   \return A map of the assigned inputs of all the truth tables.
*/
map< wxString, vector<coordinate> > LayoutManager::getTableInputs()
{
    return tableInputs;
}


//! Returns the output assignations.
/*!
   \return A map of the assigned outputs of all the truth tables.
*/
map< wxString, vector<coordinate> > LayoutManager::getTableOutputs()
{
    return tableOutputs;
}


//! Returns the table selection map.
/*!
   \return A map containing the selection status of the tables.
*/
map< wxString, bool> LayoutManager::getTableEnabled()
{
    return tableEnabled;
}


//! Returns the rule selection map.
/*!
   \return A map containing the selection status of the rules.
*/
map<unsigned int, bool> LayoutManager::getRuleEnabled()
{
    return ruleEnabled;
}


//! Returns the pattern selection map.
/*!
   \return A map containing the selection status of the patterns.
*/
map<unsigned int, bool> LayoutManager::getFPEnabled()
{
    return FPEnabled;
}


//! Saves the space to a file.
/*!
   \param file the file to be saved.
   \return True iif the space was correctly saved.
*/
bool LayoutManager::saveFilename(wxString file)
{
    if (diskManager->saveLayout(configuration, file))
    {
        layoutFileName = file;
        return true;
    }
    else
    {
        return false;
    }
}


//! Returns the number of tables.
/*!
   \return The number of tables that can be assigned to the space.
*/
int LayoutManager::getTables()
{
    return tableEnabled.size();
}


//! Returns the number of rules.
/*!
   \return The number of rules that can be assigned to the space.
*/
int LayoutManager::getRules()
{
    return ruleEnabled.size();
}


//! Returns the number of patterns.
/*!
   \return The number of patterns that can be assigned to the space.
*/
int LayoutManager::getFPs()
{
    return FPEnabled.size();
}


//! Sets an input.
/*!
   \param table truth table of the input.
   \param input input number.
   \param x x coordinate of the assignation.
   \param y y coordinate of the assignation.
*/
bool LayoutManager::setInput(wxString table, int input, int x, int y)
{
    if (tableInputs.find(table) == tableInputs.end())
    {
        return false;
    }
    tableInputs[table][input].x = x;
    tableInputs[table][input].y = y;
    if (currentTable != wxEmptyString)
    {
        view->updateInputs(tableInputs[currentTable]);
        view->updateOutputs(tableOutputs[currentTable]);
    }
    view->updateCanvas();
    controller->elementChanged();
    return true;
}


//! Sets an output.
/*!
   \param table truth table of the output.
   \param output output number.
   \param x x coordinate of the assignation.
   \param y y coordinate of the assignation.
*/
bool LayoutManager::setOutput(wxString table, int output, int x, int y)
{
    if (tableOutputs.find(table) == tableOutputs.end())
    {
        return false;
    }
    tableOutputs[table][output].x = x;
    tableOutputs[table][output].y = y;
    if (currentTable != wxEmptyString)
    {
        view->updateInputs(tableInputs[currentTable]);
        view->updateOutputs(tableOutputs[currentTable]);
    }
    view->updateCanvas();
    controller->elementChanged();
    return true;
}


//! Erases all space information.
void LayoutManager::clean()
{
    //TODO: check for memory leak
    //LayoutConfig *temp = configuration;
    configuration = NULL;
    //if (temp)
    //{
    //    delete temp;
    //}
    view->updateGrids(Grid(), true);
    controller->elementChanged();
}


//! Returns the patterns checked.
/*!
   \return A list of the forbidden patterns checked.
*/
list <ForbiddenPattern> LayoutManager::getListFPEnabled()
{
    list <ForbiddenPattern> result;
    for (map<unsigned int, bool>::iterator i = FPEnabled.begin(); i != FPEnabled.end(); i++)
    {
        if ((*i).second)
        {
            result.push_back(*FPManager->getFP((*i).first));
        }
    }
    return result;
}


//! Returns the rules checked.
/*!
   \return A list of the rules checked.
*/
list <Rule> LayoutManager::getListRuleEnabled()
{
    list <Rule> result;
    for (map<unsigned int, bool>::iterator i = ruleEnabled.begin(); i != ruleEnabled.end(); i++)
    {
        if ((*i).second)
        {
            result.push_back(*ruleManager->getRule((*i).first));
        }
    }
    return result;
}


//! Checks the simulation data.
/*!
   This method checks the validity of the data to start a simulation.
*/
void LayoutManager::prepareSimulation()
{
    controller->prepareSimulation();
}


//! Starts the simulation.
void LayoutManager::startSimulation()
{
    controller->startSimulation();
}


//! Observable update method.
/*!
   This method is called whenever an element of the simulation
   have changed and updates the view.
*/
void LayoutManager::elementChanged()
{
    view->enableCheckSimulation(true);
    view->enableStartSimulation(false);
}


//! Returns the truth table checked.
/*!
   \return The truth table checked.
*/
TruthTable * LayoutManager::getTableSelected()
{
    for (map<wxString, bool>::iterator i = tableEnabled.begin(); i != tableEnabled.end(); i++)
    {
        if ((*i).second)
        {
            return (tableManager->getTable((*i).first));
        }
    }
    
    //We should never reach this code
    //If we do... we're doomed :)
    return NULL;
}


//! Returns the input assignation of a table.
/*!
   \param table the truth table.
   \return The assigned coordinates for the table inputs.
*/
vector<coordinate> LayoutManager::getTableInput(wxString table)
{
    vector<coordinate> result;
    if (tableInputs.find(table) != tableInputs.end())
    {
        result = tableInputs[table];
    }
    return result;
}


//! Returns the output assignation of a table.
/*!
   \param table the truth table.
   \return The assigned coordinates for the table outputs.
*/
vector<coordinate> LayoutManager::getTableOutput(wxString table)
{
    vector<coordinate> result;
    if (tableOutputs.find(table) != tableOutputs.end())
    {
        result = tableOutputs[table];
    }
    return result;
}


//! Checks the validity of the space to be simulated.
/*!
   \return True iif the space is ready to be simulated. This is, all the inputs
   and outputs of the truth table checked are assigned, and there is a
   truth table checked.
*/
bool LayoutManager::checkLayout()
{
    bool result = false;
    wxString tableChecked;
    //Check there's a table selected
    for (map<wxString, bool>::iterator i = tableEnabled.begin(); i != tableEnabled.end(); i++)
    {
        if ((*i).second)
        {
            result = true;
            tableChecked = (*i).first;
        }
    }
    
    //Check the IO of the selected table
    if (result)
    {
        for (unsigned int i = 0; i < tableInputs[tableChecked].size(); i++)
        {
            if ((tableInputs[tableChecked][i].x == -1) || (tableInputs[tableChecked][i].y == -1))
            {
                result = false;
            }
        }
        for (unsigned int i = 0; i < tableOutputs[tableChecked].size(); i++)
        {
            if ((tableOutputs[tableChecked][i].x == -1) || (tableOutputs[tableChecked][i].y == -1))
            {
                result = false;
            }
        }
    }
    return result;
}


//! Enables the simulation GUI access
void LayoutManager::enableSimulation()
{
    view->enableCheckSimulation(false);
    view->enableStartSimulation(true);
}


//! Initializes the input/output information
void LayoutManager::resetIO()
{
    for (map< wxString, vector<coordinate> >::iterator i = tableInputs.begin(); i != tableInputs.end(); i++)
    {
        for (unsigned int j = 0; j < (*i).second.size(); j++)
        {
            (*i).second[j].x = -1;
            (*i).second[j].y = -1;
        }
    }
    for (map< wxString, vector<coordinate> >::iterator i = tableOutputs.begin(); i != tableOutputs.end(); i++)
    {
        for (unsigned int j = 0; j < (*i).second.size(); j++)
        {
            (*i).second[j].x = -1;
            (*i).second[j].y = -1;
        }
    }
}
