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
 * \class TruthTableCanvas
 * \brief Class which draws a truth table.
 * 
 * This class is a wxWindows widget which draws a truth table on the screen.
 * It has an event which is triggered when clicking on a cell of the table
 * to allow the modification of it.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.6 $
 */

#ifndef TRUTHTABLECANVAS_HPP_
#define TRUTHTABLECANVAS_HPP_

#include <wx/wx.h>
#include <wx/grid.h>
#include <vector>
#include "truthTable.hpp"

using namespace std;

class TruthTableCanvas : public wxGrid
{
public:
	TruthTableCanvas(wxWindow *parent, wxWindowID id, TruthTable initialTable);
    void setNewTable(TruthTable newTable);
    TruthTable getTable();
	virtual ~TruthTableCanvas();
    void setEditable(bool editable);
    
private:
    DECLARE_EVENT_TABLE()
    void setTable();
    vector<wxString> getInputsString(int i);
    vector<bool> getInputsVector(int i);
    void OnCellChanged(wxGridEvent& ev);
    vector<bool> getOutputs(int i);
    void initColours();
    wxColour *colourInputTrue;
    wxColour *colourInputFalse;
    wxColour *colourOutputTrue;
    wxColour *colourOutputFalse;
    //! The truth table.
    TruthTable table;
    //! If the table is read only.
    bool editable;
};


//Custom event to send when the user changes an output
/**
 * \class TruthTableCanvasEvent
 * \brief Event class to send events from a TruthTableCanvas.
 * 
 * This class is a wxWindows event class done to send events from a TruthTableCanvas object when a cell is clicked.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.6 $
 */
class TruthTableCanvasEvent: public wxNotifyEvent
{
public:
    TruthTableCanvasEvent(wxEventType commandType = wxEVT_NULL, int id = 0);

    vector<bool> getInput();
    int getOutput();
    void setInput(vector<bool> newInput);
    void setOutput(int newOutput);

private:
    vector<bool> input;
    int output;
};

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(TRUTH_TABLE_CANVAS_ACTION, -1)
END_DECLARE_EVENT_TYPES()

typedef void (wxEvtHandler::*TruthTableCanvasEventFunction)(TruthTableCanvasEvent&);

#define EVT_TRUTH_TABLE_CANVAS(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( TRUTH_TABLE_CANVAS_ACTION, id, -1, \
    (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wxNotifyEventFunction) \
    wxStaticCastEvent( TruthTableCanvasEventFunction, & fn ), (wxObject *) NULL ),

#endif /*TRUTHTABLECANVAS_HPP_*/
