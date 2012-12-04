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

// $Revision: 1.21 $

#include "simulationManager.hpp"

//! Constructor.
/*!
   \param controller the main application controller.
   \param tableManager the truth table controller.
   \param FPManager the forbidden pattern controller.
   \param ruleManager the rule controller.
   \param layoutManager the space controller.
*/
SimulationManager::SimulationManager(MainController *controller, TruthTableManager *tableManager, ForbiddenPatternManager *FPManager, RuleManager *ruleManager, LayoutManager *layoutManager)
{
    this->tableManager = tableManager;
    this->FPManager = FPManager;
    this->ruleManager = ruleManager;
    this->controller = controller;
    this->layoutManager = layoutManager;
}


//! Destructor.
SimulationManager::~SimulationManager()
{
    delete simulation;
    delete view;
}


//! Prepares and starts a simulation.
/*!
   This method gathers all the information needed to do
   a simulation and initializes all the important data
   structures to do a simulation, and then it starts it.
*/
void SimulationManager::prepareSimulation()
{
    //Get the table, rules and patterns
    rules.clear();
    patterns.clear();
    table = *(layoutManager->getTableSelected());
    tableInputs = layoutManager->getTableInput(table.getName());
    tableOutputs = layoutManager->getTableOutput(table.getName());
    list<Rule> ruleList = layoutManager->getListRuleEnabled();
    list<ForbiddenPattern> patternList = layoutManager->getListFPEnabled();
    //Get te initial layout
    layout = layoutManager->getLayout()->getMatrix();
    initialGrid = layoutManager->getLayout()->getGrid();
    
    //Rotate rules
    for (list<Rule>::iterator i = ruleList.begin(); i != ruleList.end(); i++)
    {
        addRule((*i));
    }
    
    
    //Rotate patterns
    for (list<ForbiddenPattern>::iterator i = patternList.begin(); i != patternList.end(); i++)
    {
        addPattern((*i));
    }

    //Initialize simulation results data structures
    row = 0;
    processedLayouts.resize((int)pow(2, (double)tableInputs.size()));
    finalLayouts.resize((int)pow(2, (double)tableInputs.size()));
    forbiddenLayouts.resize((int)pow(2, (double)tableInputs.size()));
    cycles.resize((int)pow(2, (double)tableInputs.size()));
    outOfBoundsLayouts.resize((int)pow(2, (double)tableInputs.size()));
    //Initializes the simulation view and starts
    //When a simulation finishes those two objects are deleted
    view = new SimulationView(controller->getNanoFrame(), wxID_ANY);
    simulation = new Simulation(view, this, layout, &rules, &patterns);
    view->setSimulation(simulation);
    startSimulation();
}


//TODO: mirar què fer si la rotació falla
//! Adds a rule and its rotations to the rule list.
/*!
   \param newRule rule to add.
*/
void SimulationManager::addRule(Rule newRule)
{
    if (!findRule(newRule))
    {
        //printRule(newRule);
        //cout << endl;
        rules.push_back(newRule);
        
        //Rotate only if square and even size
        if ((newRule.getWidth() == newRule.getHeight()) && (newRule.getWidth() %2 != 0))
        {

            Grid tempInitial = newRule.getInitialGrid();
            Grid tempFinal = newRule.getFinalGrid();
    
            for (int i = 0; i < 5; i++)
            {
                Grid initialRotated(tempInitial.getWidth(), tempInitial.getHeight());
                Grid finalRotated(tempInitial.getWidth(), tempInitial.getHeight());
                Rule ruleRotated(newRule.getWidth(), newRule.getHeight());
                if (rotateGrid(tempInitial, &initialRotated) && rotateGrid(tempFinal, &finalRotated))
                {
                    ruleRotated.setInitialGrid(initialRotated);
                    ruleRotated.setFinalGrid(finalRotated);
                    if (!findRule(ruleRotated))
                    {
                        //printRule(ruleRotated);
                        //cout << endl;
                        rules.push_back(ruleRotated);
                    }
                }
                else
                {   
                    cout << "Error in rotation " << i << endl;
                    break;
                }
                tempInitial = ruleRotated.getInitialGrid();
                tempFinal = ruleRotated.getFinalGrid();
            }
        }
    }
}


//! Adds a pattern and its rotations to the pattern list.
/*!
   \param newPattern pattern to add.
*/
void SimulationManager::addPattern(ForbiddenPattern newPattern)
{
    if (!findPattern(newPattern))
    {
        patterns.push_back(newPattern);
        //Rotate only if square and even size
        if ((newPattern.getWidth() == newPattern.getHeight()) && (newPattern.getWidth() %2 != 0))
        {
            Grid temp = newPattern.getGrid();
    
            for (int i = 0; i < 5; i++)
            {
                Grid rotatedGrid(temp.getWidth(), temp.getHeight());
                ForbiddenPattern patternRotated(newPattern.getWidth(), newPattern.getHeight());
                if (rotateGrid(temp, &rotatedGrid))
                {
                    patternRotated.setGrid(rotatedGrid);
                    if (!findPattern(patternRotated))
                    {
                        patterns.push_back(patternRotated);
                    }
                }
                else
                {
                    break;
                }
                temp= patternRotated.getGrid();
            }
        }
    }
}


//! Finds a rule in the collection.
/*!
   \param rule the rule to be found.
   \return True if the rule was found on the collection.
*/
bool SimulationManager::findRule(Rule rule)
{
    for (list<Rule>::iterator i = rules.begin(); i != rules.end(); i++)
    {
        if (rule == (*i))
        {
            return true;
        }
    }
    return false;
}


//! Finds a pattern in the collection.
/*!
   \param pattern the pattern to be found.
   \return True if the pattern was found on the collection.
*/
bool SimulationManager::findPattern(ForbiddenPattern pattern)
{
    for (list<ForbiddenPattern>::iterator i = patterns.begin(); i != patterns.end(); i++)
    {
        if ((*i) == pattern)
        {
            return true;
        }
    }
    return false;
}


//! Prints a rule to the standard output.
/*!
   \param rule the rule to be printed.
*/
void SimulationManager::printRule(Rule rule)
{
    cout << "Initial" << endl;
    for (int i = 0; i < rule.getWidth(); i++)
    {
        for (int k = 0; k < i; k++)
        {
            cout << "  ";
        }
        for (int j = 0; j < rule.getHeight(); j++)
        {
            cout << rule.getInitialGrid()(i, j) << "  ";
        }
        cout << endl;
    }
    
    cout << endl;
    cout << "Final" << endl;
    for (int i = 0; i < rule.getWidth(); i++)
    {
        for (int k = 0; k < i; k++)
        {
            cout << "  ";
        }
        for (int j = 0; j < rule.getHeight(); j++)
        {
            cout << rule.getFinalGrid()(i, j) << "  ";
        }
        cout << endl;
    }
}


//! Prints a pattern to the standard output.
/*!
   \param pattern the pattern to be printed.
*/
void SimulationManager::printPattern(Grid pattern)
{
    for (int i = 0; i < pattern.getWidth(); i++)
    {
        for (int k = 0; k < i; k++)
        {
            cout << " ";
        }
        for (int j = 0; j < pattern.getHeight(); j++)
        {
            cout << pattern(i, j) << " ";
        }
        cout << endl;
    }
}


//! Rotates a grid 60 degrees.
/*!
   \param originalGrid the grid to be rotated.
   \param destGrid grid where the original rotated have to be copied.
   \return True if the grid is rotated correctly.
*/
bool SimulationManager::rotateGrid(Grid originalGrid, Grid *destGrid)
{
    int c;
    int center;
    int i, j;
    int ii, jj;
    int size = originalGrid.getWidth();

    
    center = size/2;

    for (i = 0; i < size; i++) 
    {
        for (j = 0; j < size; j++) 
        {
            (*destGrid)(i, j) = nDONTCARE;
        }
    }

    for (i = 0; i < size; i++) 
    {
        for (j = 0; j < size; j++) 
        {
            c = originalGrid(i, j);

            rotateHexCoordinate(center, i, j, &ii, &jj);
            
            if (0 <= ii && ii < size && 0 <= jj && jj < size) 
            {
                (*destGrid)(ii, jj) = c;
            }
            else if (c == nENABLED || c == nDISABLED)
            {
                cout << "Warning: cannot map a transformed site" << endl;
                return false;
            }
        }
    }
    return true;
}


//! Rotates a coordinate of a grid 60 degrees.
/*!
   \param center the central coordinate of the grid.
   \param i the x original coordinate.
   \param j the y original coordinate.
   \param ii the x destination coordinate.
   \param jj the y destination coordinate.
*/
void SimulationManager::rotateHexCoordinate(int center, int i, int j, int *ii, int *jj)
{
    int x, y;
    int xx, yy;

    x = i - center;
    y = j - center;

    xx = -y;
    yy = x + y;

    *ii = xx + center;
    *jj = yy + center;
}


//TODO: recorda, ha de ser tot vàlid:
//layout no null
//taula de veritat no nula
//inputs i outputs de la taula de veritat assignats
//això ho ha de controlar el check simulation i el 
//controlador principal habilitant i deshabilitant el botó
//que controla aquesta funció
//A MUST: abans de cridar aquesta funció s'ha de cridar
//prepareSimulation()
//! Starts a simulation.
/*!
   This MUST NOT be called if all the simulation information
   is prepared and valid.
*/
void SimulationManager::startSimulation()
{
    simulateRow();
}


//! Asks for new rows to simulate.
/*!
   \return True iif there are more rows to simulate. False
   if all rows have been already simulated.
*/
bool SimulationManager::nextRow()
{
    return (row < pow(2, (double)tableInputs.size()));
}


//! Simulates the current row.
void SimulationManager::simulateRow()
{
    if (nextRow())
    {
        //TODO aquest layout no pq es pot canviar mentre se simula
        matrix newLayout = layout;
        vector<int> result(tableInputs.size(), nDISABLED);
        int j = tableInputs.size() -1 ;
        int l = row;
        while (l > 0)
        {
            if ((l % 2) == 1) //1 = True
            {
                result[j] = nENABLED;
            }  
            else
            {
                result[j] = nDISABLED;
            }
            l /= 2;  
            j--;
    }
    list<matrix> finalLayouts;
    for (unsigned int k = 0; k < tableInputs.size(); k++)
    {
        newLayout[tableInputs[k].x][tableInputs[k].y] = result[k];
    }
    for (unsigned int k = 0; k < tableOutputs.size(); k++)
    {
        newLayout[tableOutputs[k].x][tableOutputs[k].y] = nDISABLED;
    }
    
    //Here we transform all nDONTCARE positions to
    //nDISABLED
    for (unsigned int i = 0; i < newLayout.size(); i++)
    {
        for (unsigned j = 0; j < newLayout[i].size(); j++)
        {
            if (newLayout[i][j] == nDONTCARE)
            {
                newLayout[i][j] = nDISABLED;
            }
        }
    }
    
    
    simulation->resetSimulation(view, this, newLayout, &rules, &patterns);
    view->setBlankLine();
    view->setInfo(wxString::Format(_("Simulating row %d"), row));
    view->setBlankLine();
    }
    row++;
}


//! Gathers the simulation data from the simulated row.
void SimulationManager::finishedRow()
{
    view->setInfo(wxString::Format(_("Gathering data from the row: %d"), row - 1));
    processedLayouts[row - 1] = simulation->getProcessedLayouts();
    finalLayouts[row - 1] = simulation->getStableLayouts();
    forbiddenLayouts[row - 1] = simulation->getForbiddenLayouts();
    cycles[row - 1] = simulation->getCycles();
    outOfBoundsLayouts[row - 1] = simulation->getOutOfBounds();
}


//! Shows the simulation results.
void SimulationManager::results()
{
    //This view must be deleted when the simulation finishes too
    resultsView = new ResultsView(controller->getNanoFrame(), wxID_ANY, this);
    rRow = 0;
    rInformation = 0;
    rPath = 0;
    updateRowList();
    updateInformationList();
    updatePathList();
    updateGrid();
    resultsView->updateInitialGrid(initialGrid, true);
    resultsView->setTable(table);
    resultsView->setInputs(tableInputs);
    resultsView->setOutputs(tableOutputs);
    resultsView->Show(true);
}


//! Selects a row to show it's results.
/*!
   \param row the row to select.
*/
void SimulationManager::rowSelected(int row)
{
    rRow = row;
    rInformation = 0;
    rPath = 0;
    updateRowList();
    updateInformationList();
    updatePathList();
    updateGrid();
}


//! Selects a row event to show it's results.
/*!
   \param information the event to select.
*/
void SimulationManager::informationSelected(int information)
{
    rInformation = information;
    rPath = 0;
    updateInformationList();
    updatePathList();
    updateGrid();
}


//! Selects a simulation step to show it's results.
/*!
   \param path the simulation step.
*/
void SimulationManager::pathSelected(int path)
{
    rPath = path;
    updatePathList();
    updateGrid();
}


//! Updates the results view row list.
void SimulationManager::updateRowList()
{
    //We must check here if the row verifies the
    //table or not
    bool verifies = true;
    wxArrayString s;
    for (int i = 0; i < (int)pow(2, (double)tableInputs.size()); i++)
    {
        if (verifyRow(i))
        {
            s.Add(wxString::Format(_("Row %d: OK"), i + 1));
        }
        else
        {
            s.Add(wxString::Format(_("Row %d: KO"), i + 1));
            verifies = false;
        }
    }
    resultsView->updateRowList(s);
    resultsView->selectRow(rRow);
    resultsView->setResult(verifies);
}


//! Updates the results view event list.
void SimulationManager::updateInformationList()
{
    wxArrayString s;
    //First we must know if we must update
    //forbidden patterns or stable layouts
    if (forbiddenLayouts[rRow].size() > 0) //FPs
    {
        for (unsigned int i = 0; i < forbiddenLayouts[rRow].size(); i++)
        {
            s.Add(wxString::Format(_("Pattern %d"), i + 1));
        }
    }
    else if (outOfBoundsLayouts[rRow].size() > 0) //OOB
    {
        for (unsigned int i = 0; i < outOfBoundsLayouts[rRow].size(); i++)
        {
            s.Add(wxString::Format(_("Out of bounds %d"), i + 1));
        }
    }
    else 
    {
        if (finalLayouts[rRow].size() > 0)
        {
            for (unsigned int i = 0; i < finalLayouts[rRow].size(); i++)
            {
                s.Add(wxString::Format(_("Stable %d"), i + 1));
            }
        }
        if (cycles[rRow].size() > 0)
        {
            for (unsigned int i = 0; i < cycles[rRow].size(); i++)
            {
                s.Add(wxString::Format(_("Cycle %d"), i + 1));
            }   
        }
    }
    
    resultsView->updateInformationList(s);
    resultsView->selectInformation(rInformation);
}


//! Updates the results view simulation step list.
void SimulationManager::updatePathList()
{
    wxArrayString s;
    if (forbiddenLayouts[rRow].size() > 0) //FPs
    {
        list <simulationStep>::iterator it = forbiddenLayouts[rRow].begin();
        for (int i = 0; i < rInformation; i++)
        {
            it++;
        }
        simulationStep ss = *it;
        //We must know the path length of the FP
        for (unsigned int i =0; i < ss.path.size() + 1; i++)
        {
            s.Add(wxString::Format(_("Step %d"), i + 1));
        }
    }
    else if (outOfBoundsLayouts[rRow].size() > 0) //FPs
    {
        list <simulationStep>::iterator it = outOfBoundsLayouts[rRow].begin();
        for (int i = 0; i < rInformation; i++)
        {
            it++;
        }
        simulationStep ss = *it;
        //We must know the path length of the FP
        for (unsigned int i =0; i < ss.path.size() + 1; i++)
        {
            s.Add(wxString::Format(_("Step %d"), i + 1));
        }
    }
    else if (finalLayouts[rRow].size() > 0)
    {
        //If rInformation > the number of patterns means it is
        //a cycle. Because we show the cycles last.
        if (rInformation <= (int)finalLayouts[rRow].size() - 1)
        {
            list <simulationStep>::iterator it = finalLayouts[rRow].begin();
            for (int i = 0; i < rInformation; i++)
            {
                it++;
            }
            simulationStep ss = *it;
            //We must know the path length of the FP
            for (unsigned int i =0; i < ss.path.size() + 1; i++)
            {
                s.Add(wxString::Format(_("Step %d"), i + 1));
            }
        }
        else //Cycle
        {
            int newInformation = rInformation - finalLayouts[rRow].size();
            list <simulationStep>::iterator it = cycles[rRow].begin();
            for (int i = 0; i < newInformation; i++)
            {
                it++;
            }
            simulationStep ss = *it;
            //We must know the path length of the Cycle
            //We have to indicate where the cycle begins
            list <spaceHighlighted>::iterator cycleStart = ss.path.begin();
            for (unsigned int i = 0; i < ss.path.size(); i++)
            {
                if (((*cycleStart).g) == ss.space.g)
                {
                    s.Add(wxString::Format(_("Step %d-Cycle beginning"), i + 1));
                }
                else
                {
                    s.Add(wxString::Format(_("Step %d"), i + 1));
                }
                cycleStart++;
            }
            //Final step
            s.Add(wxString::Format(_("Step %d-Cycle end"), ss.path.size() + 1));
        }
    }
    else
    {
        list <simulationStep>::iterator it = cycles[rRow].begin();
        for (int i = 0; i < rInformation; i++)
        {
            it++;
        }
        simulationStep ss = *it;
        //We must know the path length of the Cycle
        //We have to indicate where the cycle begins
        list <spaceHighlighted>::iterator cycleStart = ss.path.begin();
        for (unsigned int i =0; i < ss.path.size(); i++)
        {
            if (((*cycleStart).g) == ss.space.g)
            {
                s.Add(wxString::Format(_("Step %d-Cycle beginning"), i + 1));
            }
            else
            {
                s.Add(wxString::Format(_("Step %d"), i + 1));
            }
            cycleStart++;
        }
        //Final step
        s.Add(wxString::Format(_("Step %d-Cycle end"), ss.path.size() + 1));
    }
    resultsView->updatePathList(s);
    resultsView->selectPath(rPath);
}


//! Sets the results view grid corresponding to the simulation step selected.
void SimulationManager::updateGrid()
{
    //Rememeber. A simulation step is a list of
    //grids (path) and the final grid (space).
    simulationStep ss;
    if (forbiddenLayouts[rRow].size() > 0) //FPs
    {
        list <simulationStep>::iterator it = forbiddenLayouts[rRow].begin();
        for (int i = 0; i < rInformation; i++)
        {
            it++;
        }
        ss = *it;
    }
    else if (outOfBoundsLayouts[rRow].size() > 0) //FPs
    {
        list <simulationStep>::iterator it = outOfBoundsLayouts[rRow].begin();
        for (int i = 0; i < rInformation; i++)
        {
            it++;
        }
        ss = *it;
    }
    else if (finalLayouts[rRow].size() > 0) //Stable Layouts
    {
        //If rInformation > the number of patterns means it is
        //a cycle. Because we show the cycles last.
        if (rInformation <= (int)finalLayouts[rRow].size() - 1)
        {
            list <simulationStep>::iterator it = finalLayouts[rRow].begin();
            for (int i = 0; i < rInformation; i++)
            {
                it++;
            }
            ss = *it;
        }
        else //Cycle
        {
            int newInformation = rInformation - finalLayouts[rRow].size();
            list <simulationStep>::iterator it = cycles[rRow].begin();
            for (int i = 0; i < newInformation; i++)
            {
                it++;
            }
            ss = *it;
        }
    }
    
    else
    {
        list <simulationStep>::iterator it = cycles[rRow].begin();
        for (int i = 0; i < rInformation; i++)
        {
            it++;
        }
        ss = *it;
    }
    
    //Now we must know if the step is on the path or on the top
    if (rPath > (int)ss.path.size() - 1) //Top
    {
        Grid g = ss.space.g;
        resultsView->updateGrid(g, false);
    }
    else
    {
        list<spaceHighlighted>::iterator it = ss.path.begin();
        for (int i = 0; i < rPath; i++)
        {
            it++;
        }
        Grid g = (*it).g;
        resultsView->updateGrid(g, false);
        if ((*it).h.width > 0) //Highlight zone
        {
            resultsView->addHighlight((*it).h.top, (*it).h.left, (*it).h.width, (*it).h.height);
        }
    }
}


//! Checks if a row simulation verifies the truth table.
/*!
   \param row the row to check.
   \return True if the row verifies the table, false otherwise.
*/
bool SimulationManager::verifyRow(int row)
{
    bool result = true;
    if (forbiddenLayouts[row].size() > 0)
    {
        result = false;
    }
    if (outOfBoundsLayouts[row].size() > 0)
    {
        result = false;
    }
    if (finalLayouts[row].size() > 0)//Verify all the stable layouts
    {
        for (list<simulationStep>::iterator  i = finalLayouts[row].begin(); i != finalLayouts[row].end(); i++)
        {
            result = result && verifyGrid(row, (*i).space.g);
        }        
    }   
    if (cycles[row].size() > 0) //Cycles
    {
        for (list<simulationStep>::iterator  i = cycles[row].begin(); i != cycles[row].end(); i++)
        {
            result = result && verifyCycle(row, (*i));
        }
    }
    return result;
}


//! Verifies if a grid has the correct output values for a table and a row.
/*!
   \param row the row to check.
   \param g the grid to check.
   \return True if the grid verifies the row, false otherwise.
*/
bool SimulationManager::verifyGrid(int row, Grid g)
{
    //Get the outputs values of the row
    vector<bool> result(tableInputs.size(), false);
    int j = tableInputs.size() -1 ;
    int k = row;
    while (k > 0)
    {
        if ((k % 2) == 1) //1 = True
        {
            result[j] = true;
        }  
        else
        {
            result[j] = false;
        }
        k /= 2;  
        j--;
    }
    vector<bool> outputs = table.getOutput(result);
    
    //Check the outputs are ok
    for (unsigned int i = 0; i < tableOutputs.size(); i++)
    {
        if (outputs[i]) //the position must be nENABLED
        {
            if (g(tableOutputs[i].x, tableOutputs[i].y) != nENABLED)
            {
                return false;
            }
        }
        else //the position must be nDISABLED
        {
            if (g(tableOutputs[i].x, tableOutputs[i].y) != nDISABLED)
            {
                return false;
            }
        }
    }
    return true;
}


//! Verifies if a cycle has the correct output values for a table and a row.
/*!
   \param row the row to check.
   \param st the cycle to check.
   \return True if the cycle verifies the row, false otherwise.
*/
bool SimulationManager::verifyCycle(int row, simulationStep st)
{
    bool result = true;
    
    //We must find where the cycle begins and then that all the
    //grids corresponding to the cycle verify the row.
    bool layoutFound = false;
    int j = 0;
    for (list<spaceHighlighted>::iterator i = st.path.begin(); i != st.path.end(); i++)
    {
        if (((*i).g) == st.space.g) //We found the grid, we start to check here
        {
            layoutFound = true;
        }
        if (layoutFound) //Check the grid here
        {
            cout << "Layout Cycle found: " << j << endl;
            result = result && verifyGrid(row, ((*i).g));
        }
        j++;
    }
    
    return result;
}


//! Cleans all the simulation information.
void SimulationManager::endSimulation()
{
    delete simulation;
    view->Destroy();
    resultsView->Destroy();
    controller->endSimulation();
}
