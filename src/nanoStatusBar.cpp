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
// $Revision: 1.3 $

enum
{
    ID_LEGEND_ENABLED,
    ID_LEGEND_DISABLED,
    ID_LEGEND_INPUT,
    ID_LEGEND_OUTPUT,
    ID_LEGEND_NOSPACE
};

#include "nanoStatusBar.hpp"
#include "resources/red.xpm"
#include "resources/blue.xpm"
#include "resources/black.xpm"
#include "resources/green.xpm"
#include "resources/grey.xpm"

BEGIN_EVENT_TABLE(NanoStatusBar, wxStatusBar)
    EVT_SIZE(NanoStatusBar::OnSize)
END_EVENT_TABLE()

//! Constructor.
/*!
   \param parent the parent window.
*/
NanoStatusBar::NanoStatusBar(wxWindow *parent)
:wxStatusBar(parent, wxID_ANY)
{
    SetFieldsCount(2);
    initControls();
    setWidthHeight();
}


//! Destructor.
NanoStatusBar::~NanoStatusBar()
{
}


//! Initiates the status bar controls.
void NanoStatusBar::initControls()
{
     wxBitmap bmpEnabled(red_xpm);
     wxBitmap bmpDisabled(black_xpm);
     wxBitmap bmpInput(green_xpm);
     wxBitmap bmpOutput(blue_xpm);
     wxBitmap bmpNoSpace(grey_xpm);
     
     labelEnabled = new wxStaticText(this, wxID_ANY, _("Enabled"));
     bitmapEnabled = new wxStaticBitmap(this, wxID_ANY, bmpEnabled);
     labelDisabled = new wxStaticText(this, wxID_ANY, _("Disabled"));
     bitmapDisabled = new wxStaticBitmap(this, wxID_ANY, bmpDisabled);
     labelInput = new wxStaticText(this, wxID_ANY, _("Input"));
     bitmapInput = new wxStaticBitmap(this, wxID_ANY, bmpInput);
     labelOutput = new wxStaticText(this, wxID_ANY, _("Output"));
     bitmapOutput = new wxStaticBitmap(this, wxID_ANY, bmpOutput);
     labelNoSpace = new wxStaticText(this, wxID_ANY, _("No Space"));
     bitmapNoSpace = new wxStaticBitmap(this, wxID_ANY, bmpNoSpace);
}


//! OnSize eventh method.
/*!
   In this method the right part of the status bar is drawn.
   It's done by moving the texts and the colour rectangles according
   to the new bar size.
   \param event the event.
*/
void NanoStatusBar::OnSize(wxSizeEvent& event)
{  
    //Put the staticbitmaps and labels
    wxRect rect;
    GetFieldRect(1, rect);
    int x;
    int bitmapY;
    int labelY;

    x = rect.GetX() + INITSPACE;
    bitmapY = rect.GetY() + (rect.GetHeight() - bitmapEnabled->GetSize().GetHeight()) / 2;
    labelY = rect.GetY() + (rect.GetHeight() - labelEnabled->GetSize().GetHeight()) / 2;;
    
    bitmapEnabled->Move(x, bitmapY);
    x = x + bitmapEnabled->GetSize().GetWidth() + INTERSPACE;
    labelEnabled->Move(x, labelY);
    x = x + labelEnabled->GetSize().GetWidth() + INTERSPACE;
    
    bitmapDisabled->Move(x, bitmapY);
    x = x + bitmapDisabled->GetSize().GetWidth() + INTERSPACE;
    labelDisabled->Move(x, labelY);
    x = x + labelDisabled->GetSize().GetWidth() + INTERSPACE;
    
    bitmapInput->Move(x, bitmapY);
    x = x + bitmapInput->GetSize().GetWidth() + INTERSPACE;
    labelInput->Move(x, labelY);
    x = x + labelInput->GetSize().GetWidth() + INTERSPACE;
    
    bitmapOutput->Move(x, bitmapY);
    x = x + bitmapOutput->GetSize().GetWidth() + INTERSPACE;
    labelOutput->Move(x, labelY);
    x = x + labelOutput->GetSize().GetWidth() + INTERSPACE;
    
    bitmapNoSpace->Move(x, bitmapY);
    x = x + bitmapNoSpace->GetSize().GetWidth() + INTERSPACE;
    labelNoSpace->Move(x, labelY);
    x = x + labelNoSpace->GetSize().GetWidth() + INTERSPACE;
}


//! Sets the minimum width and height for the icons to fit.
void NanoStatusBar::setWidthHeight()
{
    int width = 0;
    width = INITSPACE + 
        labelEnabled->GetSize().GetWidth() + 
        INTERSPACE + 
        labelDisabled->GetSize().GetWidth() +
        INTERSPACE +
        labelInput->GetSize().GetWidth() +
        INTERSPACE + 
        labelOutput->GetSize().GetWidth() +
        INTERSPACE +
        labelNoSpace->GetSize().GetWidth() +
        INTERSPACE +
        bitmapEnabled->GetSize().GetWidth() + 
        INTERSPACE + 
        bitmapDisabled->GetSize().GetWidth() +
        INTERSPACE +
        bitmapInput->GetSize().GetWidth() +
        INTERSPACE + 
        bitmapOutput->GetSize().GetWidth() +
        INTERSPACE +
        bitmapNoSpace->GetSize().GetWidth() +
        INITSPACE;
        
    static const int widths[2] = { -1, width };
    SetStatusWidths(2, widths);
    if ((labelEnabled->GetSize().GetHeight() + 2) > (bitmapEnabled->GetSize().GetHeight()) + 2)
    {
        SetMinHeight(labelEnabled->GetSize().GetHeight() + 2);
    }
    else
    {
        SetMinHeight(bitmapEnabled->GetSize().GetHeight() + 2);
    }
}
