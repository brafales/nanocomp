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
 * \class NanoFrame
 * \brief Main application window.
 * 
 * The application main window. It contains a panel with all the other
 * main controller views, a toolbar, and a custom status bar.
 * \author Bernat R�fales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.16 $
 */

#ifndef NANOFRAME_HPP_
#define NANOFRAME_HPP_

class MainController;
#include "mainController.hpp"
#include <wx/wx.h>

class NanoFrame : public wxFrame
{
public:
    NanoFrame(MainController *controller);
    ~NanoFrame();
    void setStatusMessage(wxString messsage);
    void errorMsg(wxString message);
    void setTitleBar(wxString title);
    void showMessage(wxString message);

private:
    DECLARE_EVENT_TABLE()
    //! Main controller.
    MainController *nanoController;
	void initMenubar();
	void initToolbar();
	void initStatusbar();
    void OnSave(wxCommandEvent &event);
    void OnSaveAs(wxCommandEvent &event);
    void OnOpen(wxCommandEvent &event);
    void OnNew(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
};

#endif /*NANOFRAME_HPP_*/
