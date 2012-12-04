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

#include "nanocompAboutDialog.hpp"
#include "resources/logo.xpm"
#include <wx/statline.h>

//! Constructor.
/*!
   \param parent the parent window.
  */
NanocompAboutDialog::NanocompAboutDialog(wxWindow *parent)
    : wxDialog(parent, wxID_ANY, _("About NanoComp"),
               wxDefaultPosition, wxDefaultSize)
{
    initControls();
    initSizers();
}

NanocompAboutDialog::~NanocompAboutDialog()
{
}


//! Control initialization.
void NanocompAboutDialog::initControls()
{
    logo = new wxStaticBitmap(this, wxID_ANY, wxBitmap(logo_xpm));    
}


//! SIzers (layout) initialization.
void NanocompAboutDialog::initSizers()
{
    wxBoxSizer *sizer = new wxBoxSizer( wxVERTICAL );
    sizer->Add( logo, 0, wxCENTRE | wxALL, 10 );

    sizer->Add( new wxStaticLine(this, wxID_ANY), 0, wxGROW | wxLEFT | wxRIGHT, 5 );

    wxString message = _("NanoComp version 1.0\n\n\
(c) 2006 Bernat Rafales Mulet\n\n\
<the_bell@users.sourceforge.net>");

    sizer->Add( CreateTextSizer(message) , 0, wxCENTRE | wxALL, 20 );


    sizer->Add( new wxStaticLine(this, wxID_ANY), 0, wxGROW | wxLEFT | wxRIGHT, 5 );


    sizer->Add( CreateButtonSizer(wxOK), 0, wxCENTRE | wxALL, 10 );

    SetSizer(sizer);

    sizer->SetSizeHints(this);
    sizer->Fit(this);
    Centre(wxBOTH | wxCENTRE_ON_SCREEN);    
}
