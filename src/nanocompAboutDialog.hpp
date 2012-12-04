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
 * \class NanocompAboutDialog
 * \brief About dialog.
 * 
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.1 $
 */


#ifndef NANOCOMPABOUTDIALOG_HPP_
#define NANOCOMPABOUTDIALOG_HPP_

#include <wx/wx.h>

class NanocompAboutDialog : public wxDialog
{
public:
	NanocompAboutDialog(wxWindow *parent);
	virtual ~NanocompAboutDialog();

private:
    void initControls();
    void initSizers();
    //! Logo bitmap
    wxStaticBitmap *logo;
};

#endif /*NANOCOMPABOUTDIALOG_HPP_*/
