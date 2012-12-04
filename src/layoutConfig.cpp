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

#include "layoutConfig.hpp"


//! Constructor.
/*!
  \param width the space width.
  \param height the space height.
  \param inputs the space input number.
  \param outputs the space output number.
*/
LayoutConfig::LayoutConfig(int width, int height, int inputs, int outputs)
    :grid(width, height)
{
    this->width = width;
    this->height = height;
    this->inputs = inputs;
    this->outputs = outputs;
    assignedInputs = 0;
    assignedOutputs = 0;
}


//! Destructor.
LayoutConfig::~LayoutConfig()
{
}


//! Cell status change.
/*!
  If all the inputs and outputs are already set we skip
  those states.
  It (de)assigns inputs and outputs if the assigning mode is on.
  If the cell is already assigned for another input/output does nothing.
  If the cell is already assigned for the current input it gets deassigned.
  \param x x coordinate.
  \param y y coordinate.
  \param assigningInput true iif the assigning input mode is enabled.
  \param assigningOutput true iif the assigning output mode is enabled.
  \param listInputs coordinates of the assigned inputs.
  \param listOutputs coordinates of the assigned outputs.
  \returns True iif the change was successful.
*/
bool LayoutConfig::cellChanged(int x, int y, bool assigningInput, bool assigningOutput, vector<coordinate> listInputs, vector<coordinate> listOutputs)
{
    if (assigningInput)
    {
        //Only do something if the selected cell is an input
        if (grid(x, y) == nINPUT)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (assigningOutput)
    {
        //Only do something if the selected cell is an output
        if (grid(x, y) == nOUTPUT)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else //Status change mode
    {
        switch (grid(x, y))
        {
            case nNOSPACE:
            {
                grid(x, y) = nENABLED;
                return true;
            }
            case nDONTCARE:
            {
                grid(x, y) = nNOSPACE;
                return true;
            }
            case nENABLED:
            {
                grid(x, y) = nDISABLED;
                return true;
            }
            case nDISABLED:
            {
                //Input only if there are inputs left to be set
                //if not we skip the nINPUT status
                 if (assignedInputs < inputs)
                {
                    grid(x, y) = nINPUT;
                    assignedInputs++;
                    return true;
                }
                else
                {
                    //Output only if there are outputs left to be set
                    //if not we skip the nOUTPUT status
                    if (assignedOutputs < outputs)
                    {
                        grid(x, y) = nOUTPUT;
                        assignedOutputs++;
                        return true;
                    }
                    else
                    {
                        //Output to no cell
                        grid(x, y) = nDONTCARE;
                        return true;
                    }
                }
            }
            case nINPUT:
            {
                if (!findCoordinate(listInputs, x, y))
                {
                    if (assignedOutputs < outputs)
                    {
                        grid(x, y) = nOUTPUT;
                        assignedInputs--;
                        assignedOutputs++;
                        return true;
                    }
                    else
                    {
                        //Output to don't care
                        grid(x, y) = nDONTCARE;
                        assignedInputs--;
                        return true;
                    }
                }
                else
                {
                    return false;
                }
            }
            case nOUTPUT:
            {
                if (!findCoordinate(listOutputs, x, y))
                {
                    grid(x, y) = nDONTCARE;
                    assignedOutputs--;
                    return true;
                }
                else
                {
                    return false;
                }
            }
            default:
            {
                return false;
            }
        }
    }
}


//! Member accessor.
/*!
  \return The grid of the space.
*/
Grid LayoutConfig::getGrid()
{
    return grid;    
}


//! Member accessor.
/*!
  \return The width of the space.
*/
int LayoutConfig::getWidth()
{
    return width;
}


//! Member accessor.
/*!
  \return The height of the space.
*/
int LayoutConfig::getHeight()
{
    return height;
}


//! Cell status change.
/*!
   The cell is changed to the given status directly.
  \param x the x coordinate.
  \param y te y coordinate.
  \param status the new cell status.
  \returns True iif the change was successful.
*/
bool LayoutConfig::cellChanged(int x, int y, int status)
{
    if (status == nINPUT)
    {
        assignedInputs++;
    }
    if (status == nOUTPUT)
    {
        assignedOutputs++;
    }
    
    grid(x, y) = status;
    return true;
}


//! Member accessor.
/*!
  \return The input number of the space.
*/
int LayoutConfig::getInputs()
{
    return inputs;
}


//! Member accessor.
/*!
  \return The output number of the space.
*/
int LayoutConfig::getOutputs()
{
    return outputs;
}


//! Coordinate finder.
/*!
   This method finds the given coordinate in a list. It's used
   to find assigned inputs and outputs.
  \param list coordinate list to find in.
  \param x x coordinate to be found.
  \param y y coordinate to be found.
*/
bool LayoutConfig::findCoordinate(vector<coordinate> list, int x, int y)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        if (list[i].x == x && list[i].y == y)
        {
            return true;
        }
    }
    return false;
}


//! Member accessor.
/*!
  \return the space grid in a matrix form.
*/
matrix LayoutConfig::getMatrix()
{
    return grid.getCells();
}
