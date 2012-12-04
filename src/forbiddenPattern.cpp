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

// $Revision: 1.13 $

#include "forbiddenPattern.hpp"
#include <iostream>
using namespace std;

//! Constructor.
/*!
  \param width the forbidden pattern width.
  \param height the forbidden pattern height.
*/
ForbiddenPattern::ForbiddenPattern(int width, int height)
    : grid(width, height)
{
    this->width = width;
    this->height = height;
}


//! Destructor.
ForbiddenPattern::~ForbiddenPattern()
{
}


//! Member accessor.
/*!
  \return The forbidden pattern width.
*/
int ForbiddenPattern::getWidth()
{
    return width;
}


//! Member accessor.
/*!
  \return The forbidden pattern height.
*/
int ForbiddenPattern::getHeight()
{
    return height;
}


//! Changes the status of a cell of the pattern.
/*!
  This method changes the status of the cell of
  the pattern at x, y. The change graph is as follows:
  don't care -> enabled -> disabled -> don't care.
  \param x x coordinate.
  \param y y coordinate 
  \return The change was successful.
*/
bool ForbiddenPattern::cellChanged(int x, int y)
{
    switch (grid(x, y))
    {
        case nDONTCARE:
        {
            grid(x, y) = nENABLED;
            break;
        }
        case nENABLED:
        {
            grid(x, y) = nDISABLED;
            break;
        }
        case nDISABLED:
        {
            grid(x, y) = nDONTCARE;
            break;
        }
        default:
        {
        }
    }

    return true;
}


//! Member accessor.
/*!
  \return The forbidden pattern grid.
*/
Grid ForbiddenPattern::getGrid()
{
    return grid;    
}


//! Equality comparison.
/*!
  \param fp the pattern to be compared to.
  \return True iif fp has the same width, height and grid.
*/
bool ForbiddenPattern::equals(ForbiddenPattern *fp)
{
    return ((fp->getGrid().getCells() == grid.getCells()));
}


//! Matrix accessor.
/*!
  \return The forbidden pattern grid as a matrix (STL vector of STL vectors).
*/
matrix ForbiddenPattern::getMatrix()
{
    return grid.getCells();
}


//! Sets the status of a cell.
/*!
  This method changes the status of the position x, y of the
  forbidden pattern.
  \param x x coordinate.
  \param y y coordinate.
  \param status  the status to set
*/
void ForbiddenPattern::cellChanged(int x, int y, int status)
{
    grid(x, y) = status;
}


//! Equality operator.
/*!
  \param pattern the forbidden pattern to be compared to.
  \return True iif fp has the same width, height and grid.
*/
bool ForbiddenPattern::operator ==(ForbiddenPattern pattern)
{
    return ((getWidth() == pattern.getWidth()) &&
            (getHeight() == pattern.getHeight()) &&
            (grid == pattern.getGrid()));
}


//! Member changer.
/*!
  \param newGrid the grid to set.
*/
void ForbiddenPattern::setGrid(Grid newGrid)
{
    grid.init(newGrid);
    width = grid.getWidth();
    height = grid.getHeight();
}
