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
 * \class LayoutCanvas
 * \brief Class which draws a grid.
 * 
 * This class is a wxWindows widget which draws a grid on the screen.
 * It has an event which is triggered when clicking on a cell of the grid
 * to allow the modification of it.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.19 $
 */

#ifndef _LAYOUTCANVAS_H_
#define _LAYOUTCANVAS_H_

#include <wx/wx.h>
#include "grid.hpp"
#include <vector>
#include <list>

//! Coordinate struct.
/*! This struct represents a 2D coordinate. */
struct coordinate
{
    int x; /*!< X coordinate. */
    int y; /*!< Y coordinate. */
};

struct rectangle
{
    coordinate topleft;
    int width;
    int height;
};

using namespace std;

class LayoutCanvas : public wxWindow
{
public:
    LayoutCanvas(wxWindow *parent, wxWindowID id, Grid initialLayout);
    ~LayoutCanvas();
    void setCellSize(int newCellSize);
    void setFittingCellSize();
    void drawScene();
    void setGrid(Grid newGrid, bool changed);
    void setVisible(bool enableds, bool disableds, bool inputs, bool outputs, bool spaces);
    void setAssignedInputs(vector<coordinate> inputs);
    void setAssignedOutputs(vector<coordinate> outputs);
    void addRectangle(int top, int left, int width, int height);

private:
    DECLARE_EVENT_TABLE()
    void OnPaint(wxPaintEvent& WXUNUSED(event));
    void OnLeftDown(wxMouseEvent& event);
    void OnScroll(wxScrollWinEvent& event);
    void OnTimer(wxTimerEvent& event);
    void adjustScrollbars();
    void adjustCenter();
    void drawMargins(wxDC &dc);
    void drawGrid(wxDC &dc);
    void drawCells(wxDC &dc);
    void drawIO(wxDC &dc);
    void drawNumber(int i, int j, int number, wxDC &dc);
    void OnResize(wxSizeEvent& event);
    coordinate viewToReal(coordinate view);
    coordinate realToView(coordinate real);
    void drawCell(int i, int j, wxColour color, wxDC &dc);
    void drawRectangles(wxDC &dc);
    void drawRectangle(rectangle r, wxDC &dc);
    void clear(wxDC &dc);
    void initTransitions();
    void resetTransitions(int value);
    wxColour calculateColour(wxColour initial, wxColour final, int percent);
    //! Width of the grid.
    int width;
    //! Height of the grid.
    int height;
    //! X coordinate of the centered top/left of the grid.
    int centerX;
    //! Y coordinate of the centered top/left of the grid.
    int centerY;
    //! Size in pixels of the molecules to be drawn.
    int cellSize;
    //! X coordinate of the widget scoll bars.
    int scrollX;
    //! Y coordinate of the widget scoll bars.
    int scrollY;
    //! Show inputs?
    bool inputs;
    //! Show outputs?
    bool outputs;
    //! Show enabled molecules?
    bool enableds;
    //! Show disabled molecules?
    bool disableds;
    //! Show no space molecules?
    bool spaces;
    //! The grid being drawn.
    Grid layout;
    //! The previous grid (used for some eye candy feature).
    Grid previous;
    //! Matrix of transition percentage.
    /*!
      This matrix is the grandient percentage to be drawn between
      the actual grid and the previous one. Is used to draw the molecules
      in a gradient way to make it more eye candy to the user.
    */
    matrix transitions;
    //! Timer
    wxTimer chrono;
    //! List of assigned inputs to be drawn as numbers.
    vector<coordinate> assignedInputs;
    //! List of assigned outputs to be drawn as numbers.
    vector<coordinate> assignedOutputs;
    //! Should we use gradient transitions?
    bool transition;
    //! List of highlight rectangles to show special zones
    list<rectangle> rectangles;
    //! Rectangle colour
    wxColour rectColour;
};

/**
 * \class LayoutCanvasEvent
 * \brief Event class to send events from a LayoutCanvas.
 * 
 * This class is a wxWindows event class done to send events from
 * a LayoutCanvas object when a cell is clicked.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.19 $
 */
class LayoutCanvasEvent: public wxNotifyEvent
{
public:
    LayoutCanvasEvent(wxEventType commandType = wxEVT_NULL, int id = 0);

    int getX();
    int getY();
    void setX(int y);
    void setY(int y);

private:
    //! x coordinate
    int x;
    //! y coordinate
    int y;
};

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(LAYOUT_CANVAS_ACTION, -1)
END_DECLARE_EVENT_TYPES()

typedef void (wxEvtHandler::*LayoutCanvasEventFunction)(LayoutCanvasEvent&);

#define EVT_LAYOUT_CANVAS(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( LAYOUT_CANVAS_ACTION, id, -1, \
    (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wxNotifyEventFunction) \
    wxStaticCastEvent( LayoutCanvasEventFunction, & fn ), (wxObject *) NULL ),
    
#endif
