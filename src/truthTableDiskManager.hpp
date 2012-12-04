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
 * \class TruthTableDiskManager
 * \brief Data persistace class for the truth tables.
 * 
 * The data persistance class for the truth tables.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.5 $
 */


#ifndef TRUTHTABLEDISKMANAGER_HPP_
#define TRUTHTABLEDISKMANAGER_HPP_

#include "FlexLexer.h"
#include "truthTableManager.hpp"
#include "wx/textfile.h"
#include <list>
#include <vector>

using namespace std;

class TruthTableManager;


class TruthTableDiskManager
{
public:
	TruthTableDiskManager(TruthTableManager *controller);
	virtual ~TruthTableDiskManager();
    bool saveTables(list<TruthTable*> tables, wxString fileName);
    bool openTables(wxString fileName);
    
private:
    //! Truth table controller.
    TruthTableManager *controller;
    //! Text file to be written/read.
    wxTextFile file;
    bool saveTableList(list<TruthTable*> tables);
    bool saveTable(TruthTable *table);
    bool saveVector(vector< vector<bool> > booleans);
    bool readTable(wxString name, int inputs, int outputs, list<TruthTable *> *tableList, FlexLexer *lexer);
    void returnTables(list<TruthTable *> *tableList);
};

#endif /*TRUTHTABLEDISKMANAGER_HPP_*/
