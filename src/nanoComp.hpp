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

/*! \mainpage NanoComp documentation main page
 *
 * Here you'll find all the information regarding the NanoComp internals.
 * Although NanoComp is not an API of any kind, it's possible that someone
 * in the near future is willing to modify or add new features to the program.
 * With this objective, all the code has been documented, hoping that this
 * will help understanding how every part of the project is connected to the
 * others, and seeing how the program flow works.
 *
 */


/**
 * \class NanoComp
 * \brief wxWindows application class.
 * 
 * Main class providing the application. It's a must for the
 * wxWindows API use. It starts the program main window and gives
 * application control to the wxWindows susbsystem. It provides init and 
 * exit functions as well.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.9 $
 */

#ifndef NANOCOMP_HPP_
#define NANOCOMP_HPP_

#include <wx/wx.h>
#include "nanoFrame.hpp"
#include "mainController.hpp"

class NanoComp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();

private:
    //! Main application controller.
    MainController *controller;
    //! Main window.
    NanoFrame *frame;
};

#endif /*NANOCOMP_HPP_*/
