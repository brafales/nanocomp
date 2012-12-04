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
 * \class LayoutDiskManager
 * \brief Data persistance class for spaces.
 * 
 * This class manages all the data persistance of space files.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.5 $
 */

#ifndef LAYOUTDISKMANAGER_HPP_
#define LAYOUTDISKMANAGER_HPP_

#define LINE_SPACE 2
#define CELL_SPACE 3

#include "FlexLexer.h"
#include "layoutManager.hpp"
#include "wx/textfile.h"

class LayoutManager;
class LayoutConfig;

class LayoutDiskManager
{
public:
    LayoutDiskManager(LayoutManager *controller);
    virtual ~LayoutDiskManager();
    bool saveLayout(LayoutConfig *layoutConfig, wxString fileName);
    bool openLayout(wxString fileName);
    
private:
    //! Space controller.
    LayoutManager *controller;
    //! File to be written/read
    wxTextFile file;
    LayoutConfig* readLayout(int width, int height, FlexLexer *lexer);
    bool saveLayout(LayoutConfig *layoutConfig);
    bool saveGrid(Grid grid);
    void returnLayout(LayoutConfig *layoutConfig);
};

#endif /*LAYOUTDISKMANAGER_HPP_*/
