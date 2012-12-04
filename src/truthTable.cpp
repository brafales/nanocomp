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

// $Revision: 1.8 $

#include "truthTable.hpp"
#include <math.h>
#include <iostream>

//! Constructor.
/*!
   \param name the table name.
   \param inputs the table number of inputs.
   \param outputs the table number of outputs.
*/
TruthTable::TruthTable(wxString name, int inputs, int outputs)
{
    table.resize((int)(pow(2, (double)inputs)));
    //We have 2^inputs outputs, so we must
    //initialize 2^input false values
    for (unsigned int i = 0; i < table.size(); i++)
    {
        table[i].resize(outputs);
        for (unsigned int j = 0; j < table[i].size(); j++)
        {
            table[i][j] = false;
        }
    }
    
    this->inputs = inputs;
    this->outputs = outputs;
    this->name = name;
}


//! Constructor.
/*!
   Builds an empty table.
*/   
TruthTable::TruthTable()
{
    inputs = 0;
    outputs = 0;
}


//! Destructor.
TruthTable::~TruthTable()
{
}


//! Returns the outputs for a given input.
/*!
   \param input the input combination.
   \return The values of the outputs for the given input values.
*/
vector<bool> TruthTable::getOutput(vector<bool> input)
{
    return table[getIndex(input)];
}


//! Maps an input combination to an index to access the table outputs.
/*!
   It's a converter between decimal and binary sistems because this is the
   way the class it's implemented: the inputs are reproduced and the outputs
   are accessed using indexes.
   \param input the input combination.
   \return The table index corresponding to the input combination.
   \sa getIndex(wxString input)
*/
int TruthTable::getIndex(vector<bool> input)
{
    int result = 0;
    int base = 1;
    for (unsigned int i = 0; i < input.size(); i++)
    {
        if (input[input.size() - 1 - i])
        {
            result += base;
        } 
        base *= 2;
    }
    return result;
}


//! Sets the output values for an input combination.
/*!
   \param input the input combination.
   \param output the output values for the input combination.
   \sa setOutput(wxString input, wxString output)
*/
void TruthTable::setOutput(vector<bool> input, vector<bool> output)
{
    table[getIndex(input)] = output;
}


//! Returns the number of inputs of the table.
/*!
   \return the table input number.
*/
int TruthTable::getInputs()
{
    return inputs;
}


//! Returns the number of outputs of the table.
/*!
   \return the table output number.
*/
int TruthTable::getOutputs()
{
    return outputs;
}


//! Returns the outputs of an input combination.
/*!
   \param input the input combination ("1" = true, "0" = false).
   \return The outputs of the input combination in string form ("1" = true, "0" = false).
*/
wxString TruthTable::getOutput(wxString input)
{
    vector<bool> vInput = stringToVector(input);
    return vectorToString(getOutput(vInput));
}


//! Returns a concrete output of an input combination.
/*!
   \param input the input combination.
   \param output the output number.
   \return The number output for a given input combination.
   \sa getOutput(wxString input, int output)
*/
bool TruthTable::getOutput(vector<bool> input, int output)
{
    return getOutput(input)[output];
}


//! Returns a concrete output of an input combination.
/*!
   \param input the input combination in string form.
   \param output the output number.
   \return The number output for a given input combination.
   \sa getOutput(vector<bool> input, int output)
*/
bool TruthTable::getOutput(wxString input, int output)
{
    vector<bool> vInput = stringToVector(input);
    return getOutput(vInput)[output];
}
    
    
//! Sets the output values for an input combination.
/*!
   \param input the input combination in string form.
   \param output the output values for the input combination in string form.
   \sa setOutput(vector<bool> input, vector<bool> output)
*/
void TruthTable::setOutput(wxString input, wxString output)
{
    vector<bool> vInput = stringToVector(input);
    vector<bool> vOutput = stringToVector(output);
    setOutput(vInput, vOutput);
}


//! Maps an input combination to an index to access the table outputs.
/*!
   It's a converter between decimal and binary sistems because this is the
   way the class it's implemented: the inputs are reproduced and the outputs
   are accessed using indexes.
   \param input the input combination in string form.
   \return The table index corresponding to the input combination.
   \sa getIndex(vector<bool> input)
*/
int TruthTable::getIndex(wxString input)
{
    vector<bool> vInput = stringToVector(input);
    return getIndex(vInput);
}


//! Converts a string to a vector.
/*!
   \param input the string to be converted.
   \return A vector version of the string.
   \sa vectorToString(vector<bool> input)
*/
vector <bool> TruthTable::stringToVector(wxString input)
{
    vector<bool> result(input.length());
    for (unsigned int i = 0; i < input.length(); i++)
    {
        if (input[i] == '0')
        {
            result[i] = false;
        }
        else
        {
            result[i] = true;
        }
    }
    
    return result;
}


//! Converts a vector to a string.
/*!
   \param input the vector to be converted.
   \return A string version of the vector.
   \sa stringToVector(wxString input)
*/
wxString TruthTable::vectorToString(vector<bool> input)
{
    wxString result= _("");
    for (unsigned int i = 0; i < input.size(); i++)
    {
        if (input[i])
        {
            result += '1';
        }
        else
        {
            result += '0';
        }
    }
    
    return result;
}


//! Returns the table.
/*!
   \return A vector of vector of booleans containing the output values
   of the table (the inputs can be recreated).
*/
vector< vector<bool> > TruthTable::getTable()
{
    return table;
}


//! Copies the contents of a table.
/*!
   \param newTable table to be copied.
*/
void TruthTable::copyTable(TruthTable newTable)
{
    table = newTable.getTable();
    name = newTable.getName();
    inputs = newTable.getInputs();
    outputs = newTable.getOutputs();
}


//! Returns the name of the table.
/*!
   \return The name of the table.
*/
wxString TruthTable::getName()
{
    return name;
}


//! Sets the name of the table.
/*!
   \param newName the name of the table.
*/
void TruthTable::setName(wxString newName)
{
    name = newName;
}


//! Sets the table values.
/*!
   \param table values to set.
*/
void TruthTable::setTable(vector< vector<bool> > table)
{
    this->table = table;
}
