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
 * \class NanoStatusBar
 * \brief Custom status bar.
 * 
 * This is a custom status bar for using with the windows of the application.
 * It behaves exactly as a normal wxWindows status bar but has an important
 * addition for usability: it has a legend of the meaning of the colours used
 * on the LayoutCanvas class for painting the different status of the cells.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.2 $
 */

#ifndef NANOSTATUSBAR_HPP_
#define NANOSTATUSBAR_HPP_

#define INITSPACE 10
#define INTERSPACE 5

#include <wx/wx.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>

class NanoStatusBar : public wxStatusBar
{
public:
    NanoStatusBar(wxWindow *parent);
	virtual ~NanoStatusBar();

private:
    DECLARE_EVENT_TABLE()
    void initControls();
    void OnSize(wxSizeEvent& event);
    void setWidthHeight();
    wxStaticText *labelEnabled;
    wxStaticText *labelDisabled;
    wxStaticText *labelInput;
    wxStaticText *labelOutput;
    wxStaticText *labelNoSpace;
    
    wxStaticBitmap *bitmapEnabled;
    wxStaticBitmap *bitmapDisabled;
    wxStaticBitmap *bitmapInput;
    wxStaticBitmap *bitmapOutput;
    wxStaticBitmap *bitmapNoSpace;
};

#endif /*NANOSTATUSBAR_HPP_*/
