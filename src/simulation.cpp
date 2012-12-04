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

#include "simulation.hpp"
#include <iostream>
#include <map>

using namespace std;

//! Constructor.
/*!
   \param view the simulation view.
   \param controller the simulation controller.
   \param initialLayout the initial space to simulate.
   \param rules the rule list to be applied.
   \param patterns the forbidden pattern list to be found.
*/
Simulation::Simulation(SimulationView *view, SimulationManager *controller, Grid initialLayout, list<Rule> *rules, list<ForbiddenPattern> *patterns)
{
    finalLayouts = new list<simulationStep>;
    patternsSimulated = new list<Grid>;
    patternsToSimulate = new list<simulationStep>;
    forbiddenPatternsFound = new list<simulationStep>;
    cycles = new list<simulationStep>;
    outOfBoundsFound = new list<simulationStep>;
    simulationStep s;
    s.space.g = initialLayout;
    patternsToSimulate->push_back(s);
    this->rules = rules;
    this->patterns = patterns;
    this->view = view;
    this->controller = controller;
    view->setGrid(initialLayout, true);
    view->Show(true);
    finished = false;
    simulating = false;
    stillRows = true;
}


//! Destructor.
Simulation::~Simulation()
{
    delete finalLayouts;
    delete patternsSimulated;
    delete patternsToSimulate;
    delete forbiddenPatternsFound;
    delete cycles;
    delete outOfBoundsFound;
}


//! Finds a rule in a space.
/*!
   \param layout the space.
   \param rule the rule to be found.
   \return A list of all the coordinates where the rule
   has been found.
*/
list<coordinate> Simulation::findRule(Grid layout, Rule rule)
{
    list<coordinate> result;
    
    //We create a bigger space to find rules in it. The new space
    //must be rule.width - 1 and rule.height - 1 more wider/heighter
    //on every side (that's 2 for the width and 2 for the height)
    //to be able to find the rule applicability out of the bounds
    Grid newSpace(layout.getWidth() + (rule.getWidth() - 1) * 2, layout.getHeight() + (rule.getHeight() - 1) * 2);
    //We set all cells to disabled
    for (int i = 0; i < newSpace.getWidth(); i++)
    {
        for (int j = 0; j < newSpace.getHeight(); j++)
        {
            newSpace(i, j) = nDISABLED;
        }
    }
    
    //Now we copy the layout into the new grid
    for (int i = 0; i < layout.getWidth(); i++)
    {
        for (int j = 0; j < layout.getHeight(); j++)
        {
            newSpace(i + rule.getWidth() - 1, j + rule.getWidth() - 1) = layout(i, j);
        }
    }
    
    //We try to find the rule at any coordinate
    for (int i = 0; i < newSpace.getWidth(); i++)
    {
        for (int j = 0; j < newSpace.getHeight(); j++)
        {
            coordinate c;
            c.x = i;
            c.y = j;
            if (ruleApplicable(newSpace, c, rule))
            {
                result.push_back(c);
                //cout << "Applies!!!!" << endl;
            }
        }
    }
    return result;
}


//! Finds a forbidden in a space.
/*!
   \param layout the space.
   \param pattern the pattern to be found.
   \return A list of all the coordinates where the pattern
   has been found.
*/
list<coordinate> Simulation::findPattern(Grid layout, ForbiddenPattern pattern)
{
    list<coordinate> result;
    
    //We create a bigger space to find rules in it. The new space
    //must be rule.width - 1 and rule.height - 1 more wider/heighter
    //on every side (that's 2 for the width and 2 for the height)
    //to be able to find the rule applicability out of the bounds
    Grid newSpace(layout.getWidth() + (pattern.getWidth() - 1) * 2, layout.getHeight() + (pattern.getHeight() - 1) * 2);
    //We set all cells to disabled
    for (int i = 0; i < newSpace.getWidth(); i++)
    {
        for (int j = 0; j < newSpace.getHeight(); j++)
        {
            newSpace(i, j) = nDISABLED;
        }
    }
    
    //Now we copy the layout into the new grid
    for (int i = 0; i < layout.getWidth(); i++)
    {
        for (int j = 0; j < layout.getHeight(); j++)
        {
            newSpace(i + pattern.getWidth() - 1, j + pattern.getWidth() - 1) = layout(i, j);
        }
    }
    
    //Find the pattern at any coordinate
    for (int i = 0; i < newSpace.getWidth(); i++)
    {
        for (int j = 0; j < newSpace.getHeight(); j++)
        {
            coordinate c;
            c.x = i;
            c.y = j;
            if (patternApplicable(newSpace, c, pattern))
            {
                result.push_back(c);
                //cout << "Applies!!!!" << endl;
            }
        }
    }
    return result;
}


//! Applies a rule to a space.
/*!
   \param layout the space where to apply the rule.
   \param rule the rule to be applied.
   \param position coordinate where to apply the rule.
   \param absolute bool false if the coordinates of applying are absolute
   respect to the layout. True if they're relative to the
   layout and the rule width and height.
   \return A new grid with the rule applied.
*/
Grid Simulation::applyRule(Grid layout, Rule rule, coordinate position, bool absolute)
{
    //TODO: check the nDONTCARE match with nDISABLED thing
    Grid result(layout);
    
    int widthShift = 0;
    int heightShift = 0;
    
    if (absolute)
    {
        widthShift = rule.getWidth() - 1;
        heightShift = rule.getHeight() - 1;
    }
    
    //We must use the normalized coordinates
    for (int i = 0; i < rule.getWidth(); i++)
    {
        for (int j = 0; j < rule.getHeight(); j++)
        {
            if (rule.getFinalGrid()(i, j) != nDONTCARE) //if it's don't care the actual value should stay unchanged
            {
                result(position.x - widthShift + i, position.y - heightShift + j) = rule.getFinalGrid()(i, j);
            }
        }
    } 
    return result;
}


//! Finds if a rule is applicable in a certain coordinate of a space.
/*!
   \param layout the space.
   \param position the coordinate to find the rule applicability.
   \param rule the rule to be found.
   \return True if the rule is applicable at te given coordinate. That is, if the
   initial configuration matches the sub grid at the space coordinate.
*/
bool Simulation::ruleApplicable(Grid layout, coordinate position, Rule rule)
{
    if (rule.getWidth() > layout.getWidth() - position.x)
    {
        return false;
    }
    if (rule.getHeight() > layout.getHeight() - position.y)
    {
        return false;
    }

    for (int i = 0; i < rule.getWidth(); i++)
    {
        for (int j = 0; j < rule.getHeight(); j++)
        {
            if (rule.getInitialGrid()(i, j) == nENABLED)
            {
                if (layout(position.x + i, position.y + j) != nENABLED)
                {
                    return false;
                }
            }
            else if (rule.getInitialGrid()(i, j) == nDISABLED)
            {
                if ((layout(position.x + i, position.y + j) != nDISABLED) && (layout(position.x + i, position.y + j) != nNOSPACE))
                {
                    return false;
                }
            }
        }
    }
    return true;
}


//! Prints a rule to the standard output.
/*!
   This is for debugging purposes only.
   \param rule the rule to be printed.
*/
void Simulation::printRule(Rule rule)
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


//! Prints a grid to the standard output.
/*!
   This is for debugging purposes only.
   \param layout the grid to be printed.
*/
void Simulation::printLayout(Grid layout)
{
    for (int i = 0; i < layout.getWidth(); i++)
    {
        for (int k = 0; k < i; k++)
        {
            cout << " ";
        }
        for (int j = 0; j < layout.getHeight(); j++)
        {
            cout << layout(i, j) << " ";
        }
        cout << endl;
    }
}


//! Finds a grid on a grid list.
/*!
   \param initialLayout the grid to find.
   \param processedLayouts the grid list.
   \return True iif the grid was found on the list.
*/
bool Simulation::find(Grid initialLayout, list<Grid> processedLayouts)
{
    for (list<Grid>::iterator i = processedLayouts.begin(); i != processedLayouts.end(); i++)
    {
        if (initialLayout == (*i))
        {
            return true;
        }
    }
    return false;
}


//! Finds if a pattern is applicable in a certain coordinate of a space.
/*!
   \param layout the space.
   \param position the coordinate to find the pattern applicability.
   \param pattern the pattern to be found.
   \return True if the pattern is applicable at te given coordinate.
*/
bool Simulation::patternApplicable(Grid layout, coordinate position, ForbiddenPattern pattern)
{
    if (pattern.getWidth() > layout.getWidth() - position.x)
    {
        return false;
    }
    if (pattern.getHeight() > layout.getHeight() - position.y)
    {
        return false;
    }

    for (int i = 0; i < pattern.getWidth(); i++)
    {
        for (int j = 0; j < pattern.getHeight(); j++)
        {
            if (pattern.getGrid()(i, j) == nENABLED)
            {
                if (layout(position.x + i, position.y + j) != nENABLED)
                {
                    return false;
                }
            }
            else if (pattern.getGrid()(i, j) == nDISABLED)
            {
                if (layout(position.x + i, position.y + j) != nDISABLED)
                {
                    return false;
                }
            }
            else //DON'T CARE
            {
                if (layout(position.x + i, position.y + j) == nNOSPACE)
                {
                    return false;
                }
            }
        }
    }
    return true;
}


//! Simulation status.
/*!
   \return True if the simulation has finished, false otherwise.
*/
bool Simulation::isFinished()
{
    return finished;
}


//IMPORTANT: this function must NOT BE CALLED
//if the stack is empty
//TODO: check cicles
//! Performs a simulation step.
/*!
   It gets a space to be simulated, checks if it's a valid space,
   that is, it does not have any forbidden pattern. Then searches for
   rules to apply to it. If it does not find any rule, it saves the space
   as a stable layout. Otherwise, it applies the rules it finds, and queue
   the new spaces to be simulated if they're not repeated.
   \param gui True if the view has to be updated with the changes and
   simulation information.
   \return True if the simulation step was performed without errors.
*/
bool Simulation::nextStep(bool gui)
{
    //"Semaphore" for not calling this method reentrant
    if (simulating)
    {
        return true;
    }
    simulating = true;
    wxStopWatch sw;
    bool result = true;
    bool stable = false;
    if (!finished)
    {
        //We take the layout we have to process from
        //the queue
        simulationStep s = patternsToSimulate->back();
        Grid layout = s.space.g;
        //Then we eliminate it because it's considered already processed
        //by this step ofc and add it to the processed list
        patternsToSimulate->pop_back();
        patternsSimulated->push_back(layout);
        if (gui)
        {
            view->setGrid(layout, false);
        }
        
        //Time to process: find forbidden patterns
        for (list<ForbiddenPattern>::iterator i = patterns->begin(); i != patterns->end(); i++)
        {
            list<coordinate> tempCoordinates;
            wxStopWatch swPattern;
            tempCoordinates = findPattern(layout, (*i));
            //if (gui)
            //{
            //   view->setInfo(wxString::Format(_("The pattern finding took %ldms to execute"), swPattern.Time()));
            //}
            if (tempCoordinates.size() > 0) //Found a forbidden pattern
            {
                if (gui)
                {
                    view->setInfo(wxString::Format(_("Forbidden pattern found at %d, %d"), tempCoordinates.front().x, tempCoordinates.front().y));
                }
                finished = true;
                result = false;
                if (gui)
                {
                    updateView();
                }
            }
        }
        
        if (!result)
        {
            forbiddenPatternsFound->push_back(s);
            //controller->finishedRow();
        }
        
        //Time to process: find out of bounds
        if (result)
        {
            bool oobFound = false;
            for (list<Rule>::iterator i = rules->begin(); i != rules->end(); i++)
            {
                list<coordinate> tempCoordinates;
                wxStopWatch swPattern;
                tempCoordinates = findOutOfBounds(layout, (*i));
                if (tempCoordinates.size() > 0) //Found an out of bounds
                {
                    if (gui)
                    {
                        for (list<coordinate>::iterator j = tempCoordinates.begin(); j != tempCoordinates.end(); j++)
                        {
                            view->setInfo(wxString::Format(_("Out of bounds found at %d, %d"), (*j).x, (*j).y));
                        }
                    }
                    finished = true;
                    oobFound = true;
                    result = false;
                }
            }
            //if (gui)
            //{
            //    updateView();
            //}
            //We push back the space which contains the oob
            //and proceed to simulate the next row
            if (oobFound)
            {
                outOfBoundsFound->push_back(s);
                //controller->finishedRow();
            }
        }
        
        if (result)
        {
            //Time to find rules to apply
            list<ruleApplying> rulesToApply;
            for (list<Rule>::iterator i = rules->begin(); i != rules->end(); i++)
            {
                list<coordinate> tempCoordinates;
                wxStopWatch swRule;
                tempCoordinates = findRule(layout, (*i));
                //if (gui)
                //{
                //    view->setInfo(wxString::Format(_("The rule finding took %ldms to execute"), swRule.Time()));
                //}
                if (tempCoordinates.size() > 0)
                {
                    for (list<coordinate>::iterator j = tempCoordinates.begin(); j != tempCoordinates.end(); j++)
                    {
                        ruleApplying ruleToApply;
                        ruleToApply.rule = &(*i);
                        ruleToApply.c = (*j);
                        rulesToApply.push_back(ruleToApply);
                    }
                }
            }
            
            if (gui)
            {
                view->setInfo(wxString::Format(_("Number of applicable rules found: %d"), rulesToApply.size()));
            }

            if (rulesToApply.size() == 0)
            {
                //Stable configuration, we return yes and the stable layout
                finalLayouts->push_back(s);
                if (gui)
                {
                    view->setInfo(_("Reached stable layout"));
                }
                result = true;
                stable = true;
            }
            else
            {
                result = true;
                //We apply the rules we can and add the resulting
                //layout to the queue to be processed
                for(list<ruleApplying>::iterator i = rulesToApply.begin(); i != rulesToApply.end(); i++)
                {
                    //Apply the rule
                    if (gui)
                    {
                        view->setInfo(wxString::Format(_("Applying rule at %d, %d"), (*i).c.x, (*i).c.y));
                    }
                    Grid changedLayout = applyRule(layout, *(*i).rule, (*i).c, true);
                    
                    //If the resulting layout is in the processed queue
                    //we don't have to simulate it again
                    //Cycles
                    //We must try to find the simulated pattern in the 
                    //current path
                    if (find(changedLayout, getGridList(s.path)))
                    {
                        list<spaceHighlighted> newList = s.path;
                        spaceHighlighted newSpaceHighlightedList;
                        newSpaceHighlightedList.g = layout;
                        newSpaceHighlightedList.h.top = (*i).c.y - (*i).rule->getHeight() + 1;
                        newSpaceHighlightedList.h.left = (*i).c.x - (*i).rule->getWidth() + 1;
                        newSpaceHighlightedList.h.width = (*i).rule->getWidth();
                        newSpaceHighlightedList.h.height = (*i).rule->getHeight();
                        newList.push_back(newSpaceHighlightedList);
                        simulationStep newStep;
                        newStep.path = newList;
                        spaceHighlighted newSpaceHighlightedStep;
                        newSpaceHighlightedStep.g = changedLayout;
                        newStep.space = newSpaceHighlightedStep;
                        cycles->push_back(newStep);
                        if (gui)
                        {
                            view->setInfo(_("Cycle found"));
                        }
                    }

                    //if (!find(changedLayout, *patternsSimulated))
                    else
                    {
                        if (gui)
                        {
                            view->setInfo(_("Adding the resulting space to the queue to be simulated"));
                        }
                        //Here we add too the highlighted zone. This is, the
                        //coordinate where we apply the rule and
                        //the width and height of the zone.
                        list<spaceHighlighted> newList = s.path;
                        spaceHighlighted newSpaceHighlightedList;
                        newSpaceHighlightedList.g = layout;
                        newSpaceHighlightedList.h.top = (*i).c.y - (*i).rule->getHeight() + 1;
                        newSpaceHighlightedList.h.left = (*i).c.x - (*i).rule->getWidth() + 1;
                        newSpaceHighlightedList.h.width = (*i).rule->getWidth();
                        newSpaceHighlightedList.h.height = (*i).rule->getHeight();
                        newList.push_back(newSpaceHighlightedList);
                        simulationStep newStep;
                        newStep.path = newList;
                        spaceHighlighted newSpaceHighlightedStep;
                        newSpaceHighlightedStep.g = changedLayout;
                        newStep.space = newSpaceHighlightedStep;
                        patternsToSimulate->push_back(newStep);
                    }
                    //else
                    //{
                    //    if (gui)
                    //    {
                    //        view->setInfo(_("The resulting space has been already simulated"));
                    //    }
                    //}
                }
                result = true;
            }
        }

        //Aqui entrem tan si hem trobat patrons prohibits com si no?
        //Si trobem patrons prohibits hauriem de plegar
        //però de moment per fer debugging ja va bé
        //We are finished if there are no more
        //layouts to process
        if (patternsToSimulate->empty())
        {
            if (gui)
            {
                view->setInfo(_("No more spaces to simulate"));
            }
            finished = true;
            result = true;
            controller->finishedRow();
            if (gui)
            {
                updateView();
            }
        }
        else
        {
            if (stable)
            {
                if (gui)
                {
                    view->setInfo(_("Backtracking to a previous applied rule"));
                }
            }
        }
    }
    else
    {
        result = true;
    }
    //if (gui)
    //{
    //    view->setInfo(wxString::Format(_("The simulation step took %ldms to execute"), sw.Time()));
    //}
    simulating = false;
    return result;
}


//! Updates the simulation view with flow control information.
/*!
   This method updates the view to allow the user simulate other
   rows and view the simulation results when the simulation is over.
*/
void Simulation::updateView()
{
    if (finished)
    {
        if (controller->nextRow())
        {
            view->setResults(false);
            view->setNextRow(true);
            view->enableSimulation(false);
        }
        else
        {
            view->setResults(true);
            view->setNextRow(false);
            view->enableSimulation(false);
        }
    }
}


//! Asks the controller for another row to simulate.
void Simulation::nextRow()
{
    if (controller->nextRow())
    {
        controller->simulateRow();
        view->setResults(false);
        view->setNextRow(false);
        view->enableSimulation(true);
    }
    else
    {
        view->setResults(true);
        view->setNextRow(false);
        view->enableSimulation(true);
    }
}


//! Asks the controller to show the simulation results.
void Simulation::results()
{
    controller->results();
}


//! Returns the stable spaces.
/*!
   \return The list of stable spaces the simulation has obtained.
   Every item on the list is a space and a trace of the spaces that
   lead to it in the simulation branch.
*/
list<simulationStep> Simulation::getStableLayouts()
{
    list<simulationStep> result;
    for (list<simulationStep>::iterator i = finalLayouts->begin(); i != finalLayouts->end(); i++)
    {
        result.push_back((*i));
    }
    return result;
}


//! Returns the spaces simulated.
/*!
   \return A list of all the spaces that have been simulated.
*/
list<Grid> Simulation::getProcessedLayouts()
{
    list<Grid> result;
    for (list<Grid>::iterator i = patternsSimulated->begin(); i != patternsSimulated->end(); i++)
    {
        result.push_back((*i));
    }
    return result;
}


//! Returns the forbidden spaces.
/*!
   \return The list of spaces the simulation has obtained that contain
   a forbidden pattern.
   Every item on the list is a space and a trace of the spaces that
   lead to it in the simulation branch.
*/
list<simulationStep> Simulation::getForbiddenLayouts()
{
    list<simulationStep> result;
    for (list<simulationStep>::iterator i = forbiddenPatternsFound->begin(); i != forbiddenPatternsFound->end(); i++)
    {
        result.push_back((*i));
    }
    return result;
}


//! Returns the cycle spaces.
/*!
   \return The list of spaces the simulation has obtained that are a cycle.
   Every item on the list is a space and a trace of the spaces that
   lead to it in the simulation branch.
*/
list<simulationStep> Simulation::getCycles()
{
    list<simulationStep> result;
    for (list<simulationStep>::iterator i = cycles->begin(); i != cycles->end(); i++)
    {
        result.push_back((*i));
    }
    return result;
}

//! Returns the out of bounds spaces.
/*!
   \return The list of spaces the simulation has obtained that push molecules
   out of the bounds of the space interface.
   Every item on the list is a space and a trace of the spaces that
   lead to it in the simulation branch.
*/
list<simulationStep> Simulation::getOutOfBounds()
{
    list<simulationStep> result;
    for (list<simulationStep>::iterator i = outOfBoundsFound->begin(); i != outOfBoundsFound->end(); i++)
    {
        result.push_back((*i));
    }
    return result;
}


//! Sets the parameters for a new simulation.
/*!
   \param view the simulation view.
   \param controller the simulation controller.
   \param initialLayout the initial space to simulate.
   \param rules the rule list to be applied.
   \param patterns the forbidden pattern list to be found.
*/
void Simulation::resetSimulation(SimulationView *view, SimulationManager *controller, Grid initialLayout, list<Rule> *rules, list<ForbiddenPattern> *patterns)
{
    //cout << "Resetting simulation" << endl;
    finalLayouts->clear();
    patternsSimulated->clear();
    patternsToSimulate->clear();
    forbiddenPatternsFound->clear();
    cycles->clear();
    simulationStep s;
    s.space.g = initialLayout;
    patternsToSimulate->push_back(s);
    this->rules = rules;
    this->patterns = patterns;
    this->view = view;
    this->controller = controller;
    view->setGrid(initialLayout, true);
    view->Show(true);
    finished = false;
    stillRows = true;
    view->Enable(true);
    view->stopSimulation();
}



//! Simulates all the rows of the simulation.
/*!
   This method simulates all the rows in a shot, without the
   timer and user interactivity. Useful for quick simulations.
*/
void Simulation::simulateAll()
{
    view->enableSimulation(false);
    view->setNextRow(false);
    view->setResults(false);
    while (stillRows)
    {
        while (!finished)
        {
            nextStep(false);
        }
        if (controller->nextRow())
        {
            controller->simulateRow();
        }
        else
        {
            stillRows = false;
        }
    }
    view->setResults(true);
}


//! Gets the grid list of a spaceHighlighted list.
/*!
   \param spaces the spaceHighlighted list.
   \return A list of the grids of the spaceHighlighted list.
*/
list<Grid> Simulation::getGridList(list<spaceHighlighted> spaces)
{
    list<Grid> result;
    for (list<spaceHighlighted>::iterator i = spaces.begin(); i != spaces.end(); i++)
    {
        result.push_back((*i).g);
    }
    return result;
}


//! Finds if a rule pushes molecules out of the space bounds.
/*!
   \param layout the space.
   \param rule the rule to analyze.
   \return A list of all the coordinates where the rule
   pushes molecules out of the space bounds.
*/
list<coordinate> Simulation::findOutOfBounds(Grid layout, Rule rule)
{
    list<coordinate> result;
    //We create a bigger space to find rules in it. The new space
    //must be rule.width - 1 and rule.height - 1 more wider/heighter
    //on every side (that's 2 for the width and 2 for the height)
    //to be able to find the rule applicability out of the bounds
    Grid newSpace(layout.getWidth() + (rule.getWidth() - 1) * 2, layout.getHeight() + (rule.getHeight() - 1) * 2);
    //We set all cells to disabled
    for (int i = 0; i < newSpace.getWidth(); i++)
    {
        for (int j = 0; j < newSpace.getHeight(); j++)
        {
            newSpace(i, j) = nDISABLED;
        }
    }
    
    //Now we copy the layout into the new grid
    for (int i = 0; i < layout.getWidth(); i++)
    {
        for (int j = 0; j < layout.getHeight(); j++)
        {
            newSpace(i + rule.getWidth() - 1, j + rule.getWidth() - 1) = layout(i, j);
        }
    }

    //Now we simply have to find the applicability of the rule
    //into the new space. If we found it can be applied, we will
    //apply it and see if the resulting space violates the space
    //interface. This is, it has a nENABLED cell on a coordinate
    //inside the "frame" we have applied to the original space.
    
    //Find the applicable coordinates
    list<coordinate> ruleApplies;
    for (int i = 0; i < newSpace.getWidth(); i++)
    {
        for (int j = 0; j < newSpace.getHeight(); j++)
        {
            coordinate c;
            c.x = i;
            c.y = j;
            if (ruleApplicable(newSpace, c, rule))
            {
                ruleApplies.push_back(c);
            }
        }
    }
    
    //Check the boundaries of the rule applying
    for(list<coordinate>::iterator i = ruleApplies.begin(); i != ruleApplies.end(); i++)
    {
        //Apply the rule
        Grid appliedSpace = applyRule(newSpace, rule, (*i), false);
        //Check the boundary
        if (checkBoundary(newSpace, appliedSpace, rule.getWidth() - 1, rule.getHeight() - 1))
        {
            coordinate normalized;
            if ((*i).x < rule.getWidth() - 1)
            {
                normalized.x = 0;
            }
            else if ((*i).x >= layout.getWidth())
            {
                normalized.x = layout.getWidth() - 1;
            }
            else
            {
                normalized.x = (*i).x;
            }
            
            if ((*i).y < rule.getHeight() - 1)
            {
                normalized.y = 0;
            }
            else if ((*i).y >= layout.getHeight())
            {
                normalized.y = layout.getHeight() - 1;
            }
            else
            {
                normalized.y = (*i).y;
            }
            result.push_back((*i));
        }
    }
    
    return result;
}


//! Checks if an enabled cell is out of bounds.
/*!
   \param originalSpace original space, used to see if any
   non space coordinate was invaded by a molecule.
   \param layout the space.
   \param widthMargin the with margin considered out of bounds.
   \param heightMargin the height margin considered out of bounds.
   \return True iif there is an enabled cell in the frame of the space
   limited by the widthMargin and heightMargin margins.
*/
bool Simulation::checkBoundary(Grid originalSpace, Grid layout, int widthMargin, int heightMargin)
{
    bool result = false;
    
    //cout << "Finding OOB in this space:" << endl;
    
    //printLayout(layout);
    
    //We'll check all the 4 subspaces that conform the frame
    //From 0,0 to width, heightMargin
    for(int i = 0; i < layout.getWidth(); i++)
    {
        for (int j = 0; j < heightMargin; j++)
        {
            if (layout(i, j) == nENABLED)
            {
                result = true;
                //cout << "1 ENABLED AT " << i << ", " << j << endl;
            }
        }
    }
    
    //From 0, height - heightMargin to width, height
    for(int i = 0; i < layout.getWidth(); i++)
    {
        for (int j = layout.getHeight() - heightMargin; j < layout.getHeight(); j++)
        {
            if (layout(i, j) == nENABLED)
            {
                result = true;
                //cout << "2 ENABLED AT " << i << ", " << j << endl;
            }
        }
    }
    
    //From 0,0 to widthMargin, height
    for(int i = 0; i < widthMargin; i++)
    {
        for (int j = 0; j < layout.getHeight(); j++)
        {
            if (layout(i, j) == nENABLED)
            {
                result = true;
                //cout << "3 ENABLED AT " << i << ", " << j << endl;
            }
        }
    }
    
    //From width-widthMargin,0 to width, height
    for(int i = layout.getWidth() - widthMargin; i < layout.getWidth(); i++)
    {
        for (int j = 0; j < layout.getHeight(); j++)
        {
            if (layout(i, j) == nENABLED)
            {
                result = true;
                //cout << "4 ENABLED AT " << i << ", " << j << endl;
            }
        }
    }
    
    //Now we check the noSPACE invasion
    for (int i = 0; i < layout.getWidth(); i++)
    {
        for (int j = 0; j < layout.getHeight(); j++)
        {
            if ((layout(i, j) == nENABLED) && (originalSpace(i, j) == nNOSPACE))
            {
                result = true;
            }
        }
    }
    return result;
}
