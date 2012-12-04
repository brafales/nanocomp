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
// $Revision: 1.11 $

#include "nanoComp.hpp"

using namespace std;

IMPLEMENT_APP(NanoComp)

//! Application initialization method.
/*!
   \return True.
*/
bool NanoComp::OnInit()
{
    controller = new MainController();
    frame = new NanoFrame(controller);
    controller->setNanoFrame(frame);
    frame->Show(true);
    SetTopWindow(frame);
    
    return true;
}

//! Exit application method, all cleanup should be done here.
/*!
   \return Exit status.
*/
int NanoComp::OnExit()
{
    delete controller;
    return 0; 
}
