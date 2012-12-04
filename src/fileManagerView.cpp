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

// $Revision: 1.2 $


#include "fileManagerView.hpp"

//! Constructor.
/*!
  Default constructor, it does not take any parameter.
*/
FileManagerView::FileManagerView()
{
}

FileManagerView::~FileManagerView()
{
}

//! Shows an open file dialog.
/*!
  \param parent the parent window.
  \param message the window title.
  \param wildcard the extension(s) to filter files.
  \return The full path of the file chosen or wxEmptyString if the user cancels.
*/
wxString FileManagerView::openFileChoose(wxWindow* parent, wxString message, wxString wildcard)
{
    return fileChoose(parent, 
        message, 
        wildcard, 
        wxOPEN | wxFILE_MUST_EXIST | wxCHANGE_DIR);
}


//! Shows a save file dialog.
/*!
  \param parent the parent window.
  \param message the window title.
  \param wildcard the extension(s) to filter files.
  \return The full path of the file chosen or wxEmptyString if the user cancels.
*/
wxString FileManagerView::saveFileChoose(wxWindow* parent, wxString message, wxString wildcard)
{
    return fileChoose(parent, 
        message, 
        wildcard, 
        wxSAVE | wxOVERWRITE_PROMPT | wxCHANGE_DIR);
}


//! Shows a file dialog.
/*!
  \param parent the parent window.
  \param message the window title.
  \param wildcard the extension(s) to filter files.
  \param style style to show on the dialog, to determine if it's an open dialog or a save one.
  \return The full path of the file chosen or wxEmptyString if the user cancels.
*/
wxString FileManagerView::fileChoose(wxWindow* parent, wxString message, wxString wildcard, long style)
{
    wxFileDialog *fileDialog = new wxFileDialog(
        parent, 
        message, 
        _(""), 
        _(""), 
        wildcard, 
        style);
    
    if (fileDialog->ShowModal() == wxID_OK)
    {
        return fileDialog->GetPath();
    }
    else
    {
        return wxEmptyString;
    }
}
