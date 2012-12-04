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
 * \class Grid
 * \brief Class representing the spaces.
 * 
 * The Grid class is the main class for spaces, forbidden patterns
 * and rules, as all of them are a composition of one of two grids.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.13 $
 */

#ifndef GRID_HPP_
#define GRID_HPP_

#include <vector>

using namespace std;

//! Status enum.
/*! This enum represents the different status of a cell on a grid can have. */
enum Status
{
    nDONTCARE = 0, /*!< There can be a molecule or not. */
    nNOSPACE, /*!< The space is not available for use. */
    nENABLED, /*!< There is a molecule on the space. */
    nDISABLED, /*!< There is not a molecule on the space. */
    nINPUT, /*!< The space is an input. */
    nOUTPUT, /*!< The space is an output. */
    nERROR /*!< Special status. */
};

using namespace std;

typedef vector< vector<int> > matrix;

class Grid
{
public:
	Grid();
    Grid(matrix newList);
    Grid(int w, int h);
	virtual ~Grid();
    int getWidth() const;
    int getHeight() const;
    matrix getCells() const;
    void init(const Grid& grid);
    bool operator ==(Grid grid);
    int& operator ()(int x, int y);
    bool isEnabled(int x, int y);
    bool isDisabled(int x, int y);
    bool isInput(int x, int y);
    bool isOutput(int x, int y);
    bool isSpace(int x, int y);
	
private:
    //! Matrix of cells.
    matrix cells;
    //! Grid width.
    int width;
    //! Grid height.
    int height;
};

#endif /*GRID_HPP_*/
