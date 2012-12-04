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

// $Revision: 1.9 $

#include "truthTableCanvas.hpp"
#include <math.h>


//Custom event
DEFINE_EVENT_TYPE( TRUTH_TABLE_CANVAS_ACTION )

//! Constructor.
/*!
   \param commandType event type.
   \param id event identifier.
*/
TruthTableCanvasEvent::TruthTableCanvasEvent(wxEventType commandType, int id)
    : wxNotifyEvent( commandType, id )
{
}


//! Returns the input combination of the event.
/*!
   \return The input combination of the event.
*/
vector<bool> TruthTableCanvasEvent::getInput()
{
    return input;
}


//! Returns the output combination of the event.
/*!
   \return The output combination of the event.
*/
int TruthTableCanvasEvent::getOutput()
{
    return output;
}


//! Sets the input combination of the event.
/*!
   \param newInput the input combination of the event.
*/
void TruthTableCanvasEvent::setInput(vector<bool> newInput)
{
    input = newInput;
}


//! Sets the output combination of the event.
/*!
   \param newOutput the input combination of the event.
*/
void TruthTableCanvasEvent::setOutput(int newOutput)
{
    output = newOutput;
}


BEGIN_EVENT_TABLE(TruthTableCanvas, wxGrid)
    EVT_GRID_CELL_LEFT_CLICK    (TruthTableCanvas::OnCellChanged)
END_EVENT_TABLE()


//! Constructor.
/*!
   \param parent the parent window.
   \param id the window identifier.
   \param initialTable the truth table to paint.
*/
TruthTableCanvas::TruthTableCanvas(wxWindow *parent, wxWindowID id, TruthTable initialTable)
    :wxGrid(parent, id), table(initialTable)
{
    initColours();
    if ((initialTable.getInputs() > 0) && (initialTable.getOutputs() > 0))
    {
        CreateGrid((int)pow(2, (double)table.getInputs()), table.getInputs() + table.getOutputs());
        setTable();
    }
    else
    {
        Show(false);
    }
    this->EnableDragGridSize(false);
    wxColour defaultColor = wxSystemSettings::GetColour (wxSYS_COLOUR_3DFACE);
    editable = true;
    this->SetDefaultCellBackgroundColour (defaultColor);
}


//! Sets a truth table.
/*!
   \param newTable the new table to set.
*/
void TruthTableCanvas::setNewTable(TruthTable newTable)
{
    table.copyTable(newTable);
    DeleteCols(0, GetNumberCols());
    DeleteRows(0, GetNumberRows());
    if ((newTable.getInputs() > 0) && (newTable.getOutputs() > 0))
    {
        InsertRows(0, (int)pow(2, (double)table.getInputs()));
        InsertCols(0, table.getInputs() + table.getOutputs());
        CreateGrid((int)pow(2, (double)table.getInputs()), table.getInputs() + table.getOutputs());
        setTable();
        Show(true);
    }
    else
    {
        Show(false);
    }
}


//! Destructor.
TruthTableCanvas::~TruthTableCanvas()
{
}


//! Initializes the grid.
/*!
   This method is called to initialize all the grid values and properties
   such as column names, strings and background colours of the cells.
*/
void TruthTableCanvas::setTable()
{
    for (int i = 0; i < pow(2, (double)table.getInputs()); i++)
    {
        vector<wxString> labels = getInputsString(i);
        vector<bool> outputs = table.getOutput(getInputsVector(i));
        for (int j = 0; j < table.getInputs(); j++)
        {
            SetColLabelValue(j, _("Input ") + wxString::Format(_("%d"), j+1));
            SetCellValue(i, j, labels[j]);
            SetCellAlignment(i, j, wxALIGN_CENTRE, wxALIGN_CENTRE);
            SetReadOnly(i, j);
            if (labels[j] == _("True"))
            {
                SetCellBackgroundColour(i, j, *colourInputTrue);
            }
            else
            {
                SetCellBackgroundColour(i, j, *colourInputFalse);
            }
        }
        for(int j = table.getInputs(); j < table.getInputs() + table.getOutputs(); j++)
        {
            SetColLabelValue(j, _("Output ") + wxString::Format(_("%d"), j - table.getInputs()+1));
            if (outputs[j - table.getInputs()])
            {
                SetCellValue(i, j, _("True"));
                SetCellBackgroundColour(i, j, *colourOutputTrue);
            }
            else
            {
                SetCellValue(i, j, _("False"));
                SetCellBackgroundColour(i, j, *colourOutputFalse);
            }
            SetCellAlignment(i, j, wxALIGN_CENTRE, wxALIGN_CENTRE);
            SetReadOnly(i, j);
        }
    }
}


//! Returns the string outputs for a given table index.
/*!
   \param i the table index. The table input combination can be
   recreated with this value, converting it to binary.
   \return The output combination of the table index, in string form.
   \sa getInputsVector(int i)
*/
vector<wxString> TruthTableCanvas::getInputsString(int i)
{
    vector<wxString> result(table.getInputs(), _("False"));
    
    //Convert the row into binary to know the combination
    int j = result.size() -1;
    while (i > 0)
    {
        if ((i % 2) == 1) //1 = True
        {
            result[j] = _("True");
        }  
        i /= 2;  
        j--;
    }
    
    return result;
}


//! Returns the vector outputs for a given table index.
/*!
   \param i the table index. The table input combination can be
   recreated with this value, converting it to binary.
   \return The output combination of the table index, in vector form.
   \sa getInputsString(int i)
*/
vector<bool> TruthTableCanvas::getInputsVector(int i)
{
    vector<bool> result(table.getInputs(), false);
    
    //Convert the row into binary to know the combination
    int j = result.size() -1;
    while (i > 0)
    {
        if ((i % 2) == 1) //1 = True
        {
            result[j] = true;
        }  
        i /= 2;  
        j--;
    }
    
    return result;
}


//! Returns the outputs for a given index.
/*!
   \param i the table index.
   \return The outputs of the table index.
*/
vector<bool> TruthTableCanvas::getOutputs(int i)
{
    vector<bool> result(table.getOutputs());
    for(int j = table.getInputs(); j < table.getInputs() + table.getOutputs(); j++)
    {
        
        if (GetCellValue(i, j) == _("True"))
        {
            result[j - table.getInputs()] = true;
        }
        else
        {
            result[j - table.getInputs()] = false;
        }
    }
    
    return result;
}


//! Cell click event.
/*!
   \param ev the event.
*/
void TruthTableCanvas::OnCellChanged(wxGridEvent& ev)
{
    if (ev.GetCol() >= table.getInputs()) //Clicked on an output
    {
        vector<bool> newInput(table.getInputs());
        newInput = getInputsVector(ev.GetRow());
        TruthTableCanvasEvent event( TRUTH_TABLE_CANVAS_ACTION, GetId() );
        event.SetEventObject( this );
        event.setInput(newInput);
        event.setOutput(ev.GetCol() - table.getInputs());
        //Optimization: we change here the colour and text
        //because repainting entire table would be very
        //time consuming
        if (editable)
        {
            if (GetCellValue(ev.GetRow(), ev.GetCol()) == _("True"))
            {
                SetCellValue(ev.GetRow(), ev.GetCol(), _("False"));
                SetCellBackgroundColour(ev.GetRow(), ev.GetCol(), *colourOutputFalse);
            }
            else
            {
                SetCellValue(ev.GetRow(), ev.GetCol(), _("True"));
                SetCellBackgroundColour(ev.GetRow(), ev.GetCol(), *colourOutputTrue);
            }
        }
        GetEventHandler()->ProcessEvent( event );
    }
    else
    {
        ev.Skip();
    }
}


//! Color value initialization.
void TruthTableCanvas::initColours()
{
    colourInputTrue = new wxColour(167, 232, 146);
    colourInputFalse = new wxColour(237, 167, 167);
    colourOutputTrue = new wxColour(66, 174, 13);
    colourOutputFalse = new wxColour(215, 72, 72);
}


//! Returns the table being drawn.
/*!
   \return The table being drawn.
*/
TruthTable TruthTableCanvas::getTable()
{
    return table;
}


//! Sets the editable property.
/*!
   \param editable if true, the table will be editable by the user,
   if false, it won't.
*/
void TruthTableCanvas::setEditable(bool editable)
{
    this->editable = editable;
}
