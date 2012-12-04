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
 * \class SimulationManager
 * \brief Main controller for the simulation.
 * 
 * This class manages the simulation. Basically it prepares and checks 
 * the simulations and creates the necessary Simulation objects to
 * proceed. It also controlls them and shows the results for the simulations.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.11 $
 */

#ifndef SIMULATIONMANAGER_HPP_
#define SIMULATIONMANAGER_HPP_

#include "mainController.hpp"
#include "layoutManager.hpp"
#include "truthTableManager.hpp"
#include "ruleManager.hpp"
#include "forbiddenPatternManager.hpp"
#include "layoutManager.hpp"
#include "simulation.hpp"
#include "simulationView.hpp"
#include "resultsView.hpp"
#include <vector>
#include <list>

using namespace std;

class LayoutManager;
class SimulationView;
class ResultsView;
class Simulation;

struct simulationStep;

class SimulationManager
{
public:
	SimulationManager(MainController *controller, TruthTableManager *tableManager, ForbiddenPatternManager *FPManager, RuleManager *ruleManager, LayoutManager *layoutManager);
	virtual ~SimulationManager();
    void prepareSimulation();
    void startSimulation();
    bool nextRow();
    void simulateRow();
    void finishedRow();
    void results();
    void rowSelected(int row);
    void informationSelected(int information);
    void pathSelected(int path);
    void endSimulation();
    
private:
    void addRule(Rule newRule);
    void addPattern(ForbiddenPattern newPattern);
    bool findRule(Rule rule);
    bool findPattern(ForbiddenPattern pattern);
    void printRule(Rule rule);
    void printPattern(Grid pattern);
    bool rotateGrid(Grid originalGrid, Grid *destGrid);
    void rotateHexCoordinate(int center, int i, int j, int *ii, int *jj);
    void updateRowList();
    void updateInformationList();
    void updatePathList();
    void updateGrid();
    bool verifyRow(int row);
    bool verifyGrid(int row, Grid g);
    bool verifyCycle(int row, simulationStep st);
    
    //! Truth table controller.
    TruthTableManager *tableManager;
    //! Forbidden pattern controller.
    ForbiddenPatternManager *FPManager;
    //! Rule controller.
    RuleManager *ruleManager;
    //! Space controller.
    LayoutManager *layoutManager;
    //! Main application controller.
    MainController *controller;
    
    //! Rules to use for simulating.
    list<Rule> rules;
    //! Forbidden patterns to use for simulating.
    list<ForbiddenPattern> patterns;
    //! Truth table to simulate.
    TruthTable table;
    //! Space to be simulated.
    matrix layout;
    //! Input coordinates for the truth table.
    vector<coordinate> tableInputs;
    //! Output coordinates for the truth table.
    vector<coordinate> tableOutputs;
    //! List of simulated spaces for every table row.
    vector< list<Grid> > processedLayouts;
    //! List of stable spaces reached for every table row.
    vector< list<simulationStep> > finalLayouts;
    //! List of forbidden pattern spaces reached for every table row.
    vector< list<simulationStep> > forbiddenLayouts;
    //! List of out of bounds spaces reached for every table row.
    vector< list<simulationStep> > outOfBoundsLayouts;
    //! List of cycles found for every table row.
    vector< list<simulationStep> > cycles;
    //! Row being simulated.
    int row;
    //! Simulation presentation layer.
    SimulationView *view;
    //! Results presentation layer.
    ResultsView *resultsView;
    //! Simulation.
    Simulation *simulation;
    //! Results row selected.
    int rRow;
    //! Results event selected.
    int rInformation;
    //! Results simulation step selected.
    int rPath;
    //! Initial space.
    Grid initialGrid;
};

#endif /*SIMULATIONMANAGER_HPP_*/
