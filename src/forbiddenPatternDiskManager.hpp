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
 * \class ForbiddenPatternDiskManager
 * \brief Forbidden pattern disk controller
 * 
 * Class providing all the data persistance layer of the
 * forbidden pattern model layer.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.6 $
 */

#ifndef FORBIDDENPATTERNDISKMANAGER_HPP_
#define FORBIDDENPATTERNDISKMANAGER_HPP_

#define LINE_SPACE 2
#define CELL_SPACE 3

#include "FlexLexer.h"
#include <list>
#include "forbiddenPatternManager.hpp"
#include "wx/textfile.h"

using namespace std;

class ForbiddenPatternManager;

class ForbiddenPatternDiskManager
{
public:
	ForbiddenPatternDiskManager(ForbiddenPatternManager *controller);
	virtual ~ForbiddenPatternDiskManager();
    bool saveFPs(list<ForbiddenPattern*> FPs, wxString fileName);
    bool openFPs(wxString fileName);
    
private:
    //! Forbidden pattern controller.
    ForbiddenPatternManager *controller;
    //! Text file used for writing.
    wxTextFile file;
    bool readFP(int width, int height, list<ForbiddenPattern *> *FPList, FlexLexer *lexer);
    void returnFPs(list<ForbiddenPattern *> *FPList);
    bool saveFPList(list<ForbiddenPattern*> rules);
    bool saveFP(ForbiddenPattern *fp);
    bool saveGrid(Grid grid);
};

#endif /*FORBIDDENPATTERNDISKMANAGER_HPP_*/
