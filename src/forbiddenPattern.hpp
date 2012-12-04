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
 * \class ForbiddenPattern
 * \brief Forbidden Pattern class.
 * 
 * This class represents the model of a forbidden pattern.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.10 $
 */

#ifndef FORBIDDENPATTERN_HPP_
#define FORBIDDENPATTERN_HPP_

#include "grid.hpp"
#include <vector>

using namespace std;

typedef vector< vector<int> > matrix;

class ForbiddenPattern
{
public:
	ForbiddenPattern(int width, int height);
	virtual ~ForbiddenPattern();
    bool operator ==(ForbiddenPattern pattern);
    int getWidth();
    int getHeight();
    bool cellChanged(int x, int y);
    void cellChanged(int x, int y, int status);
    Grid getGrid();
    void setGrid(Grid newGrid);
    bool equals(ForbiddenPattern *fp);
    matrix getMatrix();

private:
    //! Grid of the forbidden pattern.
    Grid grid;
    //! Width of the forbidden pattern.
    int width;
    //! Height of the forbidden pattern.
    int height;
};

#endif /*FORBIDDENPATTERN_HPP_*/
