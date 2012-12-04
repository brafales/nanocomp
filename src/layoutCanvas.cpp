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

// $Revision: 1.27 $

#include "layoutCanvas.hpp"
#include <wx/dcbuffer.h>

#define NUMTRANSITIONS 25
#define TIMERINTERVAL 15
#define CELL_SIZE 18
#define CELL_PROPORTION 1.10

enum
{
    TIMER_ID
};

//Custom event
DEFINE_EVENT_TYPE( LAYOUT_CANVAS_ACTION )


//! Constructor.
/*!
  \param commandType event type.
  \param id identifier of the widget sending the event.
*/
LayoutCanvasEvent::LayoutCanvasEvent(wxEventType commandType, int id)
    : wxNotifyEvent( commandType, id )
{
    x = 0;
    y = 0;
}


//! Member accessor.
/*!
  \return The x coordinate.
*/
int LayoutCanvasEvent::getX()
{
    return x;
}


//! Member accessor.
/*!
  \return The y coordinate.
*/
int LayoutCanvasEvent::getY()
{
    return y;
}


//! Member set.
/*!
  \param x the x coordinate.
*/
void LayoutCanvasEvent::setX(int x)
{
    this->x = x;
}


//! Member set.
/*!
  \param y the y coordinate.
*/
void LayoutCanvasEvent::setY(int y)
{
    this->y = y;
}


BEGIN_EVENT_TABLE(LayoutCanvas, wxWindow)
    EVT_PAINT(LayoutCanvas::OnPaint)
    EVT_LEFT_DOWN(LayoutCanvas::OnLeftDown)
    EVT_SCROLLWIN(LayoutCanvas::OnScroll)
    EVT_SIZE(LayoutCanvas::OnResize)
    EVT_TIMER(TIMER_ID, LayoutCanvas::OnTimer)
END_EVENT_TABLE()


//! Constructor.
/*!
  \param parent parent window.
  \param id window identifier.
  \param initialLayout grid to be painted.
*/
LayoutCanvas::LayoutCanvas(wxWindow *parent, wxWindowID id, Grid initialLayout)
    : wxWindow( parent, id, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE | wxSUNKEN_BORDER), layout(initialLayout), previous(initialLayout), chrono(this, TIMER_ID)
{
    width = initialLayout.getWidth();
    height = initialLayout.getHeight();
    cellSize = CELL_SIZE;
    scrollX = 0;
    scrollY = 0;
    centerX  = 0;
    centerY = 0;
    adjustScrollbars();
    adjustCenter();
    SetMinSize(wxSize(100, 100));
    enableds = true;
    disableds = true;
    inputs = true;
    outputs = true;
    spaces = true;
    initTransitions();
    resetTransitions(NUMTRANSITIONS -1);
    drawScene();
    transition = false;
    chrono.Start(TIMERINTERVAL, false);
    rectColour.Set(236, 213, 57);
}


//! Destructor.
LayoutCanvas::~LayoutCanvas()
{  
}


//! Paint event.
/*!
  This method is called every time the window receives a paint event.
  The grid must be painted here.
*/
void LayoutCanvas::OnPaint(wxPaintEvent& WXUNUSED (event))
{
        wxBufferedPaintDC dc(this);
        clear(dc);
        drawRectangles(dc);
        drawGrid(dc);
        drawCells(dc);
        drawIO(dc);
        drawMargins(dc);
}


//! Left button mouse click event.
/*!
   Method called when the left button of the mouse
   is clicked on the window. The event is sent here if necessary.
  \param event the event
*/
void LayoutCanvas::OnLeftDown(wxMouseEvent& event)
{
    if (!transition) //We do not consider event if we are in a painting transition
    {
        //Get the logic coordinate of the click
        int widgetWidth;
        int widgetHeight;
        GetClientSize(&widgetWidth, &widgetHeight);
        coordinate view;
        view.x = event.GetX();
        view.y = event.GetY();
        coordinate real = viewToReal(view);
     
        //Send the event if the click was on a cell, nothing if the click was outbounds
        if ((real.x >= 0) && (real.x < width) && (real.y >= 0) && (real.y < height))
        {
            LayoutCanvasEvent event( LAYOUT_CANVAS_ACTION, GetId() );
            event.SetEventObject( this );
            event.setX(real.x);
            event.setY(real.y);
            GetEventHandler()->ProcessEvent( event );
        }
    }
}


//! Sets the scrollbars of the window.
/*!
  Because the painting can be bigger than the actual size
  of the window, two scrollbars are set to allow the user to
  navigate through all the area of the painting.
*/
void LayoutCanvas::adjustScrollbars()
{
    int widgetWidth;
    int widgetHeight;
    GetClientSize(&widgetWidth, &widgetHeight);

    //See wxWindows documentation for details
    //the last parameter of the call is the total width/height of the window
    //the width last sum is because we paint the cells in an isometric view,
    //so as the grid grows in height, it also grows in width because the new
    //lines are painted shifted to the right
    SetScrollbar(wxHORIZONTAL, 0, widgetWidth, width*cellSize + (cellSize/2) * height);
    SetScrollbar(wxVERTICAL, 0, widgetHeight, height*cellSize);
    adjustCenter();
    drawScene();  
}


//! Scrollbar change method.
/*!
  Method called when the user uses the scrollbars.
  Some painting variables are updated and the window
  is repainted to show the scrolled zone.
  \param event the event.
*/
void LayoutCanvas::OnScroll(wxScrollWinEvent& event)
{
    WXTYPE type = (WXTYPE)event.GetEventType();
    int pos     = event.GetPosition();
    int orient  = event.GetOrientation();

    if (type == wxEVT_SCROLLWIN_THUMBTRACK)
    {
        if (orient == wxHORIZONTAL)
        {
            //Set the horizontal shift
            scrollX = -pos;
        }
        else
        {
            //Set the vertical shift
            scrollY = -pos;
        }
    }
    drawScene();
}


//! The grid is drawn in this method.
/*!
  The draw scene has been split into several
  draw methods.
  This method has to be called when anything of
  the grid changes to refresh the window.
*/
void LayoutCanvas::drawScene()
{
        wxClientDC dc(this);
        wxBufferedDC bdc(&dc);
        clear(bdc);
        drawRectangles(bdc);
        drawGrid(bdc);
        drawMargins(bdc);
        drawCells(bdc);
        drawIO(bdc);
}


//! Resize method.
/*!
  This method is called on the window resize events.
  The grid is redrawn.
  \param event the event.
*/
void LayoutCanvas::OnResize(wxSizeEvent& event)
{
    adjustCenter();
    adjustScrollbars();
    drawScene();
}


//! Calculates the top/left of the painting.
/*!
  The painting have to be centered if the window
  size is bigger than the painting size. The centered
  top and left coordinates are calculated here.
*/
void LayoutCanvas::adjustCenter()
{
    int widgetWidth;
    int widgetHeight;
    GetClientSize(&widgetWidth, &widgetHeight);

    if (widgetWidth > (width * cellSize + (cellSize/2) * height))
    {
        centerX = (widgetWidth / 2) - ((width * cellSize + (cellSize / 2) * height)) / 2;
    }
    else
    {
        centerX = 0;
    }
    if (widgetHeight > (height * cellSize))
    {
        centerY = (widgetHeight / 2) - ((height * cellSize)) / 2;
    }
    else
    {
        centerY = 0;
    }
}


//! Converts a window coordinate to a grid logical coordinate.
/*!
  \param view the window coordinate.
  \returns The logical coordinate of the grid corresponding to the
  window coordinate. If the window coordinate is outbounds, the
  returned coordinate is -1, -1.
*/
coordinate LayoutCanvas::viewToReal(coordinate view)
{
	int row = (view.y - centerY - scrollY) / cellSize;
    coordinate result;
    if ((view.x < centerX + scrollX + row * cellSize/2) || (view.y < centerY + scrollY) || (view.x > width * cellSize + centerX + scrollX + row * cellSize/2) || (view.y > height * cellSize + centerY + scrollY))
    {
        result.x = -1;
        result.y = -1;
    }
    else
    {
    	result.y = (view.y - centerY - scrollY) / cellSize;
        result.x = (view.x - centerX - scrollX - result.y * cellSize / 2) / cellSize;
    }
    
    return result;
}


//! Converts a grid logical coordinate to a window coordinate.
/*!
  \param real the grid logical coordinate.
  \returns The window coordinate of the center of the logical
  grid coordinate. Used to know where to paint the cells in the
  window.
*/
coordinate LayoutCanvas::realToView(coordinate real)
{
    coordinate result;
    result.x = (real.x * cellSize) + centerX + scrollX + cellSize / 2 + real.y * cellSize / 2;
    result.y = (real.y * cellSize) + centerY + scrollY + cellSize / 2;

    return result;
}


//! Draws a cell.
/*!
  \param i the x logical coordinate of the grid.
  \param j the y logical coordinate of the grid.
  \param color the colour of the cell to be painted.
  \param dc the device context where to paint.
*/
void LayoutCanvas::drawCell(int i, int j, wxColour color, wxDC &dc)
{  
    //Calculate the window coordinates
    coordinate real;
    real.x = i;
    real.y = j;
    coordinate view;
    view = realToView(real);
    //Paint
    dc.SetPen(wxPen(color));
    dc.SetBrush(wxBrush(color));
    dc.DrawCircle(view.x, view.y, (int)(cellSize / CELL_PROPORTION ) / 2 - 1);
}


//! Member setting.
/*!
  Sets the size (in pixels) of the cells.
  \param newCellSize the cell size.
*/
void LayoutCanvas::setCellSize(int newCellSize)
{
    if (newCellSize >= 4)
    {
        cellSize = newCellSize;
        adjustScrollbars();
        adjustCenter();
    }
}


//! Draws the grid points.
/*!
  Draws the grid of the grid. This is, the points of the grid
  but not the cells.
  \param dc the device context.
*/
void LayoutCanvas::drawGrid(wxDC &dc)
{
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.SetPen(*wxWHITE_PEN);
    dc.SetBrush(*wxBLACK_BRUSH);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j ++)
        {
            dc.DrawCircle(scrollX + centerX + i * cellSize + cellSize / 2 + j * cellSize / 2, scrollY + centerY + j * cellSize + cellSize / 2, 3);
        }
    }
}


//! Draws the cells of the grid.
/*!
  \param dc the device context.
*/
void LayoutCanvas::drawCells(wxDC &dc)
{
    //Draw Cells
    //We use a transition system. When a cell has it's status
    //changed it doesn't get repainted in the new colour
    //immediately, it gets a transition between the previous
    //colour and the new one. This is achieved by mantaining
    //two grids: the previous and the current, and a third grid
    //which saves the transition percent reached in this gradual
    //colour change. So, when a cell is painted, the initial and final
    //colours are accessed, and then the resulting colour is
    //calculated based on the percent. The percent grid is
    //refreshed by a timer, controlling the transition duration.
    wxColour initial;
    wxColour final;
    for (int i = 0; i < layout.getWidth(); i++)
    {
        for (int j = 0; j < layout.getHeight(); j++)
        {
            //Set initial colour
            switch (previous(i, j))
            {
                case nENABLED:
                {
                    initial = wxColour(*wxRED);
                    break;
                }
                case nDISABLED:
                {
                    initial = wxColour(*wxBLACK);
                    break;
                }
                case nINPUT:
                {
                    initial = wxColour(*wxGREEN);
                    break;
                }
                case nOUTPUT:
                {
                    initial = wxColour(*wxCYAN);
                    break;
                }
                case nNOSPACE:
                {
                    initial = wxColour(*wxLIGHT_GREY);
                    break;
                }
                case nDONTCARE:
                {
                    initial = wxColour(*wxWHITE);
                    break;
                }
            }
            //Set final colour and paint if necessary
            switch (layout(i, j))
            {
                case nENABLED:
                {
                    final = wxColour(*wxRED);
                    if (enableds)
                    {
                        drawCell(i, j, calculateColour(initial, final, transitions[i][j]), dc);
                    }
                    break;
                }
                case nDISABLED:
                {
                    final = wxColour(*wxBLACK);
                    if (disableds)
                    {
                        drawCell(i, j, calculateColour(initial, final, transitions[i][j]), dc);
                    }
                    break;
                }
                case nINPUT:
                {
                    final = wxColour(*wxGREEN);
                    if (inputs)
                    {
                        drawCell(i, j, calculateColour(initial, final, transitions[i][j]), dc);
                    }
                    break;
                }
                case nOUTPUT:
                {
                    final = wxColour(*wxCYAN);
                    if (outputs)
                    {
                        drawCell(i, j, calculateColour(initial, final, transitions[i][j]), dc);
                    }
                    break;
                }
                case nNOSPACE:
                {
                    final = wxColour(*wxLIGHT_GREY);
                    if (spaces)
                    {
                        drawCell(i, j, calculateColour(initial, final, transitions[i][j]), dc);
                    }
                    break;
                }
                case nDONTCARE:
                {
                    final = wxColour(*wxWHITE);
                    break;
                }
            }
        }
    }
}


//! Calculates the optimal cell size for the window size.
/*!
  This method calculates the maximum cell size that can be
  used to allow all the grid to be painted in the window, without
  having to use scrollbars.
*/
void LayoutCanvas::setFittingCellSize()
{
    int widgetWidth;
    int widgetHeight;
    GetClientSize(&widgetWidth, &widgetHeight);
    
    if (widgetWidth > widgetHeight)
    {
        setCellSize(widgetHeight / height);
    }
    else
    {
        setCellSize(widgetWidth / width + 1);
    }
}


//! Sets the grid to be painted.
/*!
  \param newGrid the grid to be painted.
  \param changed false if the new grid is only a cell status change
  or it's really an entire new grid.
*/
void LayoutCanvas::setGrid(Grid newGrid, bool changed)
{
    //Previous grid = actual grid
    previous.init(layout);
    //Actual grid = new grid
    layout.init(newGrid);
    //Reset the transitions grid
    initTransitions();
    resetTransitions(0);
    if (changed || (previous.getHeight() != newGrid.getHeight()) || (previous.getWidth() != newGrid.getWidth()))
    {
        //We must reset both layouts because they have different width and heights
        previous.init(newGrid);
        width = newGrid.getWidth();
        height = newGrid.getHeight();
        adjustCenter();
        adjustScrollbars();
    }
    //Start the transition control timer
    if (!chrono.IsRunning())
    {
        chrono.Start(TIMERINTERVAL, false);
    }
    rectangles.clear();
    //Redraw scene
    drawScene();
}


//! Draws the margins of the grid.
/*!
  Those margins are drawn around the grid when the
  window size is bigger than the grid painting size.
  \param dc the device context.
*/
void LayoutCanvas::drawMargins(wxDC &dc)
{
    int widgetWidth;
    int widgetHeight;
    GetClientSize(&widgetWidth, &widgetHeight);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(*wxLIGHT_GREY_BRUSH);

    if ((width > 0) && (height > 0))
    {
        dc.DrawRectangle(0, 0, widgetWidth, scrollY + centerY - cellSize);
        dc.DrawRectangle(0, 0, scrollX + centerX - cellSize, widgetHeight);
        dc.DrawRectangle(0, scrollY + centerY + height * cellSize + cellSize, widgetWidth, widgetHeight - (scrollY + centerY + height * cellSize + cellSize));
        dc.DrawRectangle(scrollX + centerX + width * cellSize + height * (cellSize/2) + cellSize, 0, widgetWidth - scrollX + centerX + width * cellSize + height * (cellSize/2), widgetHeight);
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawRectangle(scrollX + centerX - cellSize, scrollY + centerY - cellSize, width * cellSize + height * (cellSize/2) + cellSize*2, height * cellSize + cellSize*2);
    }
    else
    {
        dc.DrawRectangle(0, 0, widgetWidth, widgetHeight);
    }
}


//! Sets the cell status that have to be drawn.
/*!
  The cell status which it's wanted to be painted can be customized.
  This way we can paint only inputs and outputs, for example.
  \param enableds true iif we want the nENABLED cells to be painted.
  \param disableds true iif we want the nDISABLED cells to be painted.
  \param inputs true iif we want the nINPUT cells to be painted.
  \param outputs true iif we want the nOUTPUT cells to be painted.
  \param spaces true iif we want the nNOSPACE cells to be painted.
*/
void LayoutCanvas::setVisible(bool enableds, bool disableds, bool inputs, bool outputs, bool spaces)
{
    this->enableds = enableds;
    this->disableds = disableds;
    this->inputs = inputs;
    this->outputs = outputs;
    this->spaces = spaces;
    drawScene();
}


//! Sets the coordinates of the assigned inputs.
/*!
  The input cells that have been assigned must be painted
  in a different way, this method is used to set the coordinates
  of the assigned inputs.
  \param newInputs vector of assigned inputs. The index of the vector is
  the input (position 0 is the first input, position 1 is the second input...).
  If a position has the coordinates -1, -1 means that that input is not assigned.
*/
void LayoutCanvas::setAssignedInputs(vector<coordinate> newInputs)
{
    assignedInputs = newInputs;
}


//! Sets the coordinates of the assigned outputs.
/*!
  The output cells that have been assigned must be painted
  in a different way, this method is used to set the coordinates
  of the assigned outputs.
  \param newOutputs vector of assigned outputs. The index of the vector is
  the output (position 0 is the first output, position 1 is the second output...).
  If a position has the coordinates -1, -1 means that that output is not assigned.
*/
void LayoutCanvas::setAssignedOutputs(vector<coordinate> newOutputs)
{
    assignedOutputs = newOutputs;
}


//! Draws the assigned inputs and outputs.
/*!
  \param dc the context device.
*/
void LayoutCanvas::drawIO(wxDC &dc)
{
    //We draw a number in the assigned inputs/outputs.
    //The number is the index position of the input/output
    //vector.
    int number;
    
     if (inputs)
    {
        number = 1;
   
        for (unsigned int i = 0; i < assignedInputs.size(); i++)
        {
            if (assignedInputs[i].x >= 0)
            {
                drawNumber(assignedInputs[i].x, assignedInputs[i].y, number, dc);
            }
            number++;
        }
    }
    
    if (outputs)
    {
        number = 1;
        for (unsigned int i = 0; i < assignedOutputs.size(); i++)
        {
            if (assignedOutputs[i].x >= 0)
            {
                drawNumber(assignedOutputs[i].x, assignedOutputs[i].y, number, dc);
            }
            number++;
        }
    }
}


//! Draws a number.
/*!
  \param i x logical grid coordinate.
  \param j y logical grid coordinate.
  \param number number to be painted.
  \param dc the device context.
*/
void LayoutCanvas::drawNumber(int i, int j, int number, wxDC &dc)
{
    coordinate real;
    real.x = i;
    real.y = j;
    coordinate view;
    view = realToView(real);
    int w, h;
    dc.GetTextExtent(wxString::Format(_("%d"), number), &w, &h);
    dc.SetBackgroundMode(wxTRANSPARENT);
    dc.SetTextForeground(*wxBLACK);
    dc.DrawText(wxString::Format(_("%d"), number), view.x - w / 2, view.y - h / 2);
}


//! Erases the window.
/*!
  \param dc the device context.
*/
void LayoutCanvas::clear(wxDC &dc)
{
    dc.Clear();
}


//! Initializes the transition grid.
void LayoutCanvas::initTransitions()
{
    transitions.resize(layout.getWidth());
    for (unsigned int i = 0; i < transitions.size(); i++)
    {
        transitions[i].resize(layout.getHeight());
        for (unsigned int j = 0; j < transitions[i].size(); j++)
        {
            transitions[i][j] = NUMTRANSITIONS -1;
        }
    }
}


//! Timer event method.
/*!
  Method called on every timer event. The transition grid
  is refreshed in this method.
  \param event the event.
*/
void LayoutCanvas::OnTimer(wxTimerEvent& event)
{
    bool stop = true;
    //Update the transition values
    for (unsigned int i = 0; i < transitions.size(); i++)
    {
        for (unsigned int j = 0; j < transitions[i].size(); j++)
        {
            if (transitions[i][j] < NUMTRANSITIONS - 1)
            {
                transitions[i][j] = transitions[i][j] + 1;
                stop = false;
            }
        }
    }
    //We stop the transition refresh if there's nothing
    //to be refreshed
    if (stop)
    {
        chrono.Stop();
    }
    //Redraw scene with the new gradient colours
    drawScene();
}


//! Sets all the cells of the transition grid.
/*!
  \param value the value to set
*/
void LayoutCanvas::resetTransitions(int value)
{
    for (unsigned int i = 0; i < transitions.size(); i++)
    {
        transitions[i].resize(layout.getHeight());
        for (unsigned int j = 0; j < transitions[i].size(); j++)
        {
            transitions[i][j] = value;
        }
    }
}


//! Calculates an intermediate colour.
/*!
  \param initial the initial colour.
  \param final the final colour.
  \param percent the percent gradient.
  \return A colour between the initial colour and the final colour
  that is percent of the final colour and 100-percent of the initial colour.
*/
wxColour LayoutCanvas::calculateColour(wxColour initial, wxColour final, int percent)
{
    float rIncrease;
    float gIncrease;
    float bIncrease;
    int r = (int)final.Red() - (int)initial.Red();
    rIncrease = ((float)r/NUMTRANSITIONS);
    r = (int)initial.Red();

    int g = (int)final.Green() - (int)initial.Green();
    gIncrease = ((float)g/NUMTRANSITIONS);
    g = (int)initial.Green();
    
    int b = (int)final.Blue() - (int)initial.Blue();
    bIncrease = ((float)b/NUMTRANSITIONS);
    b = (int)initial.Blue();
    return wxColour((unsigned char)((float)r + (float)percent*rIncrease), (unsigned char)((float)g + (float)percent*gIncrease), (unsigned char)((float)b + (float)percent*bIncrease));
}


//! Adds a rectangle to be highlighted.
/*!
  \param top topleft top coordinate.
  \param left topleft coordinate.
  \param width rectangle width.
  \param height rectangle height
*/
void LayoutCanvas::addRectangle(int top, int left, int width, int height)
{
    rectangle newRectangle;
    newRectangle.topleft.x = left;
    newRectangle.topleft.y = top;
    newRectangle.width = width;
    newRectangle.height = height;
    rectangles.push_back(newRectangle);
}


//! Draws a rectangle.
/*!
  \param r the rectangle to draw.
  \param dc the device context where to paint.
*/
void LayoutCanvas::drawRectangle(rectangle r, wxDC &dc)
{
    //Calculate the window coordinates
    coordinate topleft;
    coordinate topright;
    coordinate bottomleft;
    coordinate bottomright;
    topleft = r.topleft;
    topright.x = topleft.x + r.width - 1;
    topright.y = topleft.y;
    bottomleft.x = topleft.x;
    bottomleft.y = topleft.y + r.height - 1;
    bottomright.x = topright.x;
    bottomright.y = bottomleft.y;
    coordinate realTL = realToView(topleft);
    coordinate realTR = realToView(topright);
    coordinate realBL = realToView(bottomleft);
    coordinate realBR = realToView(bottomright);
    wxPoint rectPoints[4];
    rectPoints[0] = wxPoint(realTL.x, realTL.y);
    rectPoints[1] = wxPoint(realTR.x, realTR.y);
    rectPoints[2] = wxPoint(realBR.x, realBR.y);
    rectPoints[3] = wxPoint(realBL.x, realBL.y);
    
    //Paint
    dc.SetPen(wxPen(rectColour));
    dc.SetBrush(wxBrush(rectColour));
    dc.DrawPolygon(4, rectPoints, 0, 0);
}


//! Draws the rectangles of the grid.
/*!
  \param dc the device context.
*/
void LayoutCanvas::drawRectangles(wxDC &dc)
{
    for (list<rectangle>::iterator i = rectangles.begin(); i != rectangles.end(); i++)
    {
        drawRectangle(*i, dc);
    }
}
