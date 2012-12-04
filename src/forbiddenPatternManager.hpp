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
 * \class ForbiddenPatternManager
 * \brief Forbidden pattern controller.
 * 
 * This class is the main controller for forbidden patterns.
 * It manages the forbidden pattern view, manages the forbidden
 * pattern collection of the system, provides services to the other
 * controllers regarding forbidden patterns, and manages the 
 * disk persistance class.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.13 $
 */

#ifndef FORBIDDENPATTERNMANAGER_HPP_
#define FORBIDDENPATTERNMANAGER_HPP_

#include "forbiddenPatternView.hpp"
#include "forbiddenPattern.hpp"
#include "fileManagerView.hpp"
#include "forbiddenPatternDiskManager.hpp"
#include "mainController.hpp"
#include <map>

using namespace std;

class ForbiddenPatternView;
class ForbiddenPatternDiskManager;
class MainController;

class ForbiddenPatternManager
{
public:
	ForbiddenPatternManager(MainController *controller);
	virtual ~ForbiddenPatternManager();
    void setView(ForbiddenPatternView *view);
    void newFP(int width, int height);
    void newFP(ForbiddenPattern *fp);
    void FPSelected(unsigned int FPId);
    void cellChanged(int x, int y);
    void removeFP();
    bool saveFile();
    bool saveFileAs();
    void openFile();
    bool openPFile(wxString file);
    bool saveFilename(wxString file);
    wxArrayString getFPList();
    ForbiddenPattern *getFP(unsigned int id);
    bool isModified();
    wxString getFileName();
    void clean();
    bool checkPatterns();

private:
    //! File manager view for filesystem interaction.
    FileManagerView fileView;
    //! Data persistance class.
    ForbiddenPatternDiskManager *diskManager;
    //! Presentation class.
    ForbiddenPatternView *view;
    //! Main application controller.
    MainController *controller;
    //! Current filename for the forbidden patterns.
    wxString fileName;
    //! Has the forbidden pattern collection been modified without saving?
    bool modified;
    //! Do I need a file name to save the forbidden pattern collection?
    bool needNewFile;
    //! Collection of forbidden patterns.
    map<unsigned int, ForbiddenPattern*> FPList;
    //! Next identifier to be used for the coming forbidden pattern.
    unsigned int nextId;
    //! Forbidden pattern currently in use.
    unsigned int currentFP;
    void updateGrids(bool changed);
    void updateList();
    bool appendFPs(bool mustDeleteFPs, bool newFile);
    void deleteFPs();
};

#endif /*FORBIDDENPATTERNMANAGER_HPP_*/
