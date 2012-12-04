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
 * \class Rule
 * \brief Model class for a rule.
 * 
 * This is a class representing a rule.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.14 $
 */

#ifndef RULE_HPP_
#define RULE_HPP_

#include "grid.hpp"
#include <vector>

//! Type enum
/*! This enumeration is used to specify the grid. */
enum GridType
{
    nINITIAL, /*!< Initial configuration. */
    nFINAL /*!< Fina configuration. */
};

using namespace std;

typedef vector< vector<int> > matrix;

class Rule
{
public:
	Rule(int width, int height);
	virtual ~Rule();
    bool operator ==(Rule rule);
    bool cellChanged(int x, int y, int status, GridType layout);
    Grid getInitialGrid();
    void setInitialGrid(Grid newGrid);
    Grid getFinalGrid();
    void setFinalGrid(Grid newGrid);
    bool cellChanged(int x, int y, GridType layout);
    int getWidth();
    int getHeight();
    bool equals(Rule *rule);
    bool sameInitialFinal();
    bool valid();
    void print();
    matrix getInitialMatrix();
    matrix getFinalMatrix();
    
private:
    //! Rule width.
    int width;
    //! Rule height.
    int height;
    //! Rule initial configuration.
    Grid initialGrid;
    //! Rule final configuration.
    Grid finalGrid;
};

#endif /*RULE_HPP_*/
