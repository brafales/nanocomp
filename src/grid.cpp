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

// $Revision: 1.17 $

#include "grid.hpp"
#include <iostream>

using namespace std;


//! Constructor.
/*!
   Creates a default grid with width and height of zero.
*/
Grid::Grid()
{
    width = 0;
    height = 0;
}


//! Constructor.
/*!
  \param w width of the grid.
  \param h height of the grid.
*/
Grid::Grid(int w, int h)
{
    width = w;
    height = h;
    //Main vector
    cells.resize(w);
    //Vector's vector resizes
    for (int i = 0; i < w; i++)
    {
        cells[i].resize(h);
    }
}


//! Constructor.
/*!
  \param newList matrix of cells.
*/
Grid::Grid(matrix newList)
{
    width = newList.size();
    height = newList[0].size();
    cells = newList;    
}


//! Destructor.
Grid::~Grid()
{
}


//Grid class
//! Initializes the grid.
/*!
  \param grid the grid to be set.
*/
void Grid::init(const Grid& grid)
{
    width = grid.getWidth();
    height = grid.getHeight();
    cells = grid.getCells();
}


//! Member accessor.
/*!
  \return The grid height.
*/
int Grid::getHeight() const
{
    return height;
}


//! Member accessor.
/*!
  \return The grid width
*/
int Grid::getWidth() const
{
    return width;
}


//! Cell status.
/*!
   Allows to know the status of the cell at x, y.
  \param x x coordinate.
  \param y y coordinate.
  \return True iif the cell at x, y has the status nENABLED.
*/
bool Grid::isEnabled(int x, int y)
{
    return cells[x][y] == nENABLED;
}


//! Cell status.
/*!
   Allows to know the status of the cell at x, y.
  \param x x coordinate.
  \param y y coordinate.
  \return True iif the cell at x, y has the status nDISABLED.
*/
bool Grid::isDisabled(int x, int y)
{
    return cells[x][y] == nDISABLED;
}


//! Cell status.
/*!
   Allows to know the status of the cell at x, y.
  \param x x coordinate.
  \param y y coordinate.
  \return True iif the cell at x, y has the status nINPUT.
*/
bool Grid::isInput(int x, int y)
{
        return cells[x][y] == nINPUT;
}


//! Cell status.
/*!
   Allows to know the status of the cell at x, y.
  \param x x coordinate.
  \param y y coordinate.
  \return True iif the cell at x, y has the status nOUTPUT.
*/
bool Grid::isOutput(int x, int y)
{
        return cells[x][y] == nOUTPUT;
}


//! Cell status.
/*!
   Allows to know the status of the cell at x, y.
  \param x x coordinate.
  \param y y coordinate.
  \return True iif the cell at x, y has the status nNOSPACE.
*/
bool Grid::isSpace(int x, int y)
{
    return cells[x][y] != nNOSPACE;
}


//! Member accessor.
/*!
  \return The grid in a matrix form.
*/
matrix Grid::getCells() const
{
    return cells;
}


//! Cell accessor.
/*!
   Special operator to get a reference to a cell. Using this
   the status of a cell can be inspected this way: int status = grid(x, y)
   or modified the same way: grid(x, y) = nENABLED.
  \param x x coordinate.
  \param y y coordinate.
  \return Reference to the cell at x, y.
*/
int& Grid::operator()(int x, int y)
{
    return cells[x][y];
}


//! Equality operator.
/*!
  \param grid the grid to be compared to.
  \return True iif the grid equals to the parameter. This is if the grids
  have the same width and height and if all cells of the grids have the same
  status.
*/
bool Grid::operator ==(Grid grid)
{
    if (grid.getWidth() != getWidth())
    {
        return false;
    }
    else if (grid.getHeight() != getHeight())
    {
        return false;
    }
    else
    {
        for (int i = 0; i < getWidth(); i++)
        {
            for (int j = 0; j < getHeight(); j++)
            {
                if (cells[i][j] != grid(i, j))
                {
                    return false;
                }
            }
        }
    }
    return true;
}
