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
 * \class TruthTable
 * \brief Model class for a truth table.
 * 
 * This is a class representing a truth table.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.7 $
 */

#ifndef TRUTHTABLE_HPP_
#define TRUTHTABLE_HPP_

#include <wx/wx.h>
#include <vector>

using namespace std;

class TruthTable
{
public:
	TruthTable(wxString name, int inputs, int outputs);
    TruthTable();
	virtual ~TruthTable();
    vector<bool> getOutput(vector<bool> input);
    wxString getOutput(wxString input);
    bool getOutput(vector<bool> input, int output);
    bool getOutput(wxString input, int output);
    wxString getName();
    void setOutput(vector<bool> input, vector<bool> output);
    void setOutput(wxString input, wxString output);
    void setName(wxString newName);
    void setTable(vector< vector<bool> > table);
    int getInputs();
    int getOutputs();
    void copyTable(TruthTable newTable);
    vector< vector<bool> > getTable();
    
private:
    //! Truth table name.
    wxString name;
    //! Truth table #inputs.
    int inputs;
    //! Truth table #outputs.
    int outputs;
    vector< vector<bool> > table;
    int getIndex(vector<bool>input);
    int getIndex(wxString input);
    vector <bool> stringToVector(wxString input);
    wxString vectorToString(vector<bool> input);
};

#endif /*TRUTHTABLE_HPP_*/
