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
 * \class FileManagerView
 * \brief Class providing some file system GUI views.
 * 
 * This class provides some views for interacting with the
 * file system, such as open or save file dialogs.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.4 $
 */

#ifndef FILEMANAGER_HPP_
#define FILEMANAGER_HPP_

#include "wx/wx.h"
#include "wx/textfile.h"

class FileManagerView
{
public:
    FileManagerView();
    virtual ~FileManagerView();
    wxString openFileChoose(wxWindow* parent, wxString message, wxString wildcard);
    wxString saveFileChoose(wxWindow* parent, wxString message, wxString wildcard);
    
private:
    wxString fileChoose(wxWindow* parent, wxString message, wxString wildcard, long style);
};

#endif /*FILEMANAGER_HPP_*/
