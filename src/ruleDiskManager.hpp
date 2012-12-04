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
 * \class RuleDiskManager
 * \brief Data persistace class for the rules.
 * 
 * The data persistance class for the rules.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.9 $
 */

#ifndef RULEDISKMANAGER_HPP_
#define RULEDISKMANAGER_HPP_

#define LINE_SPACE 2
#define CELL_SPACE 3

#include "FlexLexer.h"
#include <list>
#include "ruleManager.hpp"
#include "wx/textfile.h"

using namespace std;

class RuleManager;

class RuleDiskManager
{
public:
	RuleDiskManager(RuleManager *controller);
	virtual ~RuleDiskManager();
    bool saveRules(list<Rule*> rules, wxString fileName);
    bool openRules(wxString fileName);
    
private:
    //! Rule controller.
    RuleManager *controller;
    //! File to be written/read.
    wxTextFile file;
    bool saveRuleList(list<Rule*> rules);
    bool saveRule(Rule *rule);
    bool saveGrid(Grid grid);
    bool readRule(int width, int height, list<Rule *> *ruleList, FlexLexer *lexer);
    void returnRules(list<Rule *> *ruleList);
};

#endif /*RULEDISKMANAGER_HPP_*/
