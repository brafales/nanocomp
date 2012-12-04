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

// $Revision: 1.16 $

//TODO: check invalid combinations of Grids => initial cannot be the same as final

#include "rule.hpp"
#include <iostream>

using namespace std;

//! Constructor.
/*!
   \param width the rule width.
   \param height the rule height.
*/
Rule::Rule(int width, int height)
    : initialGrid(width, height), finalGrid(width, height)
{
    this->height = height;
    this->width = width;
}


//! Destructor.
Rule::~Rule()
{
}


//! Member accessor.
/*!
   \return The rule width.
*/
int Rule::getWidth()
{
    return width;
}


//! Member accessor.
/*!
   \return The rule height.
*/
int Rule::getHeight()
{
    return height;
}


//! Member accessor.
/*!
   \return The initial rule grid.
*/
Grid Rule::getInitialGrid()
{
    return initialGrid;
}


//! Member accessor.
/*!
   \return The final grid of the rule.
*/
Grid Rule::getFinalGrid()
{
    return finalGrid;
}


//! Cell status change.
/*!
   This is called from the manager when a cell recieves a change status signal
   We must know the current status and try to set the new one.
   Cell status changes are "Don't care" -> "Enabled" -> "Disabled"
   \param x x coordinate.
   \param y y coordinate.
   \param layout the grid, can be the initial configuration or the final one.
   \return True if the status change was made correctly.
*/
bool Rule::cellChanged(int x, int y, GridType layout)
{
    int status;
    Grid *grid;
    
    switch (layout)
    {
        case nINITIAL:
        {
            status = initialGrid(x, y);
            grid = &initialGrid;
            break;
        }
        case nFINAL:
        {
            status = finalGrid(x, y);
            grid = &finalGrid;
            break;
        }
    }
    
    switch (status)
    {
        case nENABLED:
        {
            (*grid)(x, y) = nDISABLED;
            break;
        }
        case nDISABLED:
        {
            (*grid)(x, y) = nDONTCARE;
            break;
        }
        case nDONTCARE:
        {
            (*grid)(x, y) = nENABLED;
            break;
        }
    }
    
    return true;
}


//! Sets the grid for the rule initial configuration.
/*!
   \param newGrid the grid.
*/
void Rule::setInitialGrid(Grid newGrid)
{
    initialGrid.init(newGrid);    
}


//! Sets the grid for the rule final configuration.
/*!
   \param newGrid the grid.
*/
void Rule::setFinalGrid(Grid newGrid)
{
    finalGrid.init(newGrid);
}


//! Changes the status of a cell to status.
/*!
   \param x the x coordinate.
   \param y the y coordinate.
   \param status the status to set.
   \param layout the grid to change, can be the initial layout or the final layout.
   \return True if the change was made successfully.
*/
bool Rule::cellChanged(int x, int y, int status, GridType layout)
{
    Grid *grid;
    switch (layout)
    {
        case nINITIAL:
        {
            grid = &initialGrid;
            break;
        }
        case nFINAL:
        {
            grid = &finalGrid;
            break;
        }
    }
    (*grid)(x, y) = status;
    
    return true;
}


//! Tests two rules for equality.
/*!
   \param rule the rule to be compared.
   \return True if the rules were equal. This is, have the same width and height
   and all the cells have the same status. False otherwise.
*/
bool Rule::equals(Rule *rule)
{
    if (getHeight() != rule->getHeight() || getWidth() != rule->getWidth())
    {
        return false;
    }
    Grid ruleInitialGrid = rule->getInitialGrid();
    Grid ruleFinalGrid = rule->getFinalGrid();
    for (int i = 0; i < getWidth(); i++)
    {
        for (int j = 0; j < getHeight(); j++)
        {
            if ((initialGrid(i, j) != ruleInitialGrid(i, j)) || (finalGrid(i, j) != ruleFinalGrid(i, j)))
            {
                return false;
            }
        }
    }
    return true;
}


//! Compares the initial and final configurations of a rule.
/*!
   \return True if the initial and final configurations equal.
*/
bool Rule::sameInitialFinal()
{
    for (int i = 0; i < getWidth(); i++)
    {
        for (int j = 0; j < getHeight(); j++)
        {
            if ((initialGrid(i, j) != finalGrid(i, j)))
            {
                return false;
            }
        }
    }
    return true;
}


//! Checks if a rule is valid.
/*!
   \return True if the rule is valid. This is, if there's no
   cells on the final configurations in the nENABLED mode and
   the corresponding cell n the initial configuration on the 
   nDONTCARE status. False otherwise.
*/
bool Rule::valid()
{
    for (int i = 0; i < getWidth(); i++)
    {
        for (int j = 0; j < getHeight(); j++)
        {
            if ((initialGrid(i, j) == nDONTCARE && (finalGrid(i, j) == nENABLED || finalGrid(i, j) == nDISABLED)))
            {
                return false;
            }
        }
    }
    return true;
}


//! Member accessor.
/*!
   \return The initial grid as a matrix.
*/
matrix Rule::getInitialMatrix()
{
return initialGrid.getCells();
}


//! Member accessor.
/*!
   \return The initial grid as a matrix.
*/
matrix Rule::getFinalMatrix()
{
    return finalGrid.getCells();
}


//! Compares two rules.
/*!
   \param rule the rule to be compared.
   \return True if the two rules equal, false otherwise.
   \sa equals(Rule *rule).
*/
bool Rule::operator ==(Rule rule)
{
    return ((getWidth() == rule.getWidth()) && 
            (getHeight() == rule.getHeight()) && 
            (initialGrid == rule.getInitialGrid()) && 
            (finalGrid == rule.getFinalGrid()));
}
