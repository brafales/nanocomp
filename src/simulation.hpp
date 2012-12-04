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
 * \class Simulation
 * \brief Class for a one row simulation.
 * 
 * This class is used to simulate rows of a truth table.
 * It gets the initial configuration, the rules and forbidden patterns to
 * use, and it simulates step by step until all the spaces have been
 * simulated or an error occurs.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.14 $
 */

#ifndef SIMULATION_HPP_
#define SIMULATION_HPP_

#include "simulationManager.hpp"
#include "simulationView.hpp"
#include <vector>
#include <list>

//! Rule applying struct.
/*! Struct used to store information about a rule application. */
struct ruleApplying
{
    Rule *rule; /*!< The rule which applies. */
    coordinate c; /*!< The coordinate in which the rule applies.. */
};


//! Highlight information struct.
/*! Struct used to store information about a space highlighted zone. */
struct highlight
{
    int top;
    int left;
    int width;
    int height;
};


//! Step space.
/*! Struct used to store information about a space and it's highligted zone. */
struct spaceHighlighted
{
    Grid g;
    highlight h;
};


//! Siulation step struct.
/*! This is used to represent a single simulation step. */
struct simulationStep
{
    list <spaceHighlighted> path; /*!< List of spaces which lead to the current space. */
    spaceHighlighted space; /*!< Current space of the simulation step. */
};

using namespace std;

class SimulationManager;

class SimulationView;

class Simulation
{
public:
	Simulation(SimulationView *view, SimulationManager *controller, Grid initialLayout, list<Rule> *rules, list<ForbiddenPattern> *patterns);
	virtual ~Simulation();
    bool isFinished();
    bool nextStep(bool gui);
    void nextRow();
    void results();
    void simulateAll();
    void resetSimulation(SimulationView *view, SimulationManager *controller, Grid initialLayout, list<Rule> *rules, list<ForbiddenPattern> *patterns);
    list<simulationStep> getStableLayouts();
    list<Grid> getProcessedLayouts();
    list<simulationStep> getForbiddenLayouts();
    list<simulationStep> getCycles();
    list<simulationStep> getOutOfBounds();

private:
    list<coordinate> findRule(Grid layout, Rule rule);
    list<coordinate> findPattern(Grid layout, ForbiddenPattern pattern);
    Grid applyRule(Grid layout, Rule rule, coordinate position, bool absolute);
    bool ruleApplicable(Grid layout, coordinate position, Rule rule);
    void printRule(Rule rule);
    void printLayout(Grid layout);
    bool find(Grid initialLayout, list<Grid> processedLayouts);
    bool patternApplicable(Grid layout, coordinate position, ForbiddenPattern pattern);
    void updateView();
    list<Grid> getGridList(list<spaceHighlighted> spaces);
    list<coordinate> findOutOfBounds(Grid layout, Rule rule);
    bool checkBoundary(Grid originalSpace, Grid layout, int widthMargin, int heightMargin);
    //! Simulation presentation layer.
    SimulationView *view;
    //! Simulation controller.
    SimulationManager *controller;
    //! List of stable layouts reached through the simulation.
    list<simulationStep> *finalLayouts;
    //! List of patterns to still be simulated.
    list<simulationStep> *patternsToSimulate;
    //! List of spaces which already have been simulated.
    list<Grid> *patternsSimulated;
    //! List of forbidden patterns found during the simulation.
    list<simulationStep> *forbiddenPatternsFound;
    //! List of cycles found during the simulation.
    list<simulationStep> *cycles;
    //! List of out of bounds layouts found during the simulation.
    list<simulationStep> *outOfBoundsFound;
    //! List of rules to test.
    list<Rule> *rules;
    //! List of forbidden patterns to test.
    list<ForbiddenPattern> *patterns;
    //! Is the simulation finished?
    bool finished;
    //! Are we in a step?
    bool simulating;
    //! There are more rows to simulate?
    bool stillRows;
};

#endif /*SIMULATION_HPP_*/
