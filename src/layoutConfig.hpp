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
 * \class LayoutConfig
 * \brief Model class for space.
 * 
 * This class represents the model for an initial configuration. A grid and
 * it's inputs and outputs.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.15 $
 */

#ifndef LAYOUTCONFIG_HPP_
#define LAYOUTCONFIG_HPP_

#include "grid.hpp"
#include <wx/wx.h>
#include "truthTableManager.hpp"
#include <vector>

using namespace std;


class TruthTableManager;

class LayoutConfig
{
public:
	LayoutConfig(int width, int height, int inputs, int outputs);
	virtual ~LayoutConfig();
    bool cellChanged(int x, int y, bool assigningInput, bool assigningOuput, vector<coordinate> listInputs, vector<coordinate> listOutputs);
    bool cellChanged(int x, int y, int status);
    Grid getGrid();
    void setGrid(Grid newGrid);
    int getWidth();
    int getHeight();
    int getInputs();
    int getOutputs();
    matrix getMatrix();
    
private:
    //! The grid of the space.
    Grid grid;
    //! Space width.
    int width;
    //! Space height.
    int height;
    //! Space #inputs.
    int inputs;
    //! Space #outputs.
    int outputs;
    //! Space assigned #inputs.
    int assignedInputs;
    //! Space assigned #outputs.
    int assignedOutputs;
    bool findCoordinate(vector<coordinate> list, int x, int y);
};

#endif /*LAYOUTCONFIG_HPP_*/
