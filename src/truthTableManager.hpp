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
 * \class TruthTableManager
 * \brief Main controller for the truth tables.
 * 
 * This class is the main controller for the truth table collection. It manages it's view
 * and data persistance layers and also provides services to the
 * other controllers.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.12 $
 */

#ifndef TRUTHTABLEMANAGER_HPP_
#define TRUTHTABLEMANAGER_HPP_

#include "truthTableView.hpp"
#include "truthTable.hpp"
#include "fileManagerView.hpp"
#include "truthTableDiskManager.hpp"
#include "mainController.hpp"
#include <wx/wx.h>
#include <map>

using namespace std;

class TruthTableView;
class TruthTableDiskManager;
class MainController;

class TruthTableManager
{
public:
	TruthTableManager(MainController *controller);
	virtual ~TruthTableManager();
    wxArrayString getTableList(); 
    void setView(TruthTableView *view);
    void newTable(wxString name, int inputs, int outputs);
    void newTable(TruthTable *table);
    void tableSelected(wxString TableId);
    void tableChanged(vector<bool> input, int output);
    void removeTable();
    bool saveFile();
    bool saveFileAs();
    void openFile();
    bool openPFile(wxString file);
    bool saveFilename(wxString file);
    int getInputs(wxString tableId);
    int getOutputs(wxString tableId);
    bool isModified();
    wxString getFileName();
    void clean();
    TruthTable * getTable(wxString id);

private:
    //! Presentation layer.
    TruthTableView *view;
    //! FIle system presentation layer.
    FileManagerView fileView;
    //! Main application controller.
    MainController *controller;
    //! Data persistance controller.
    TruthTableDiskManager *diskManager;
    //! The truth table collection.
    map<wxString, TruthTable*> tableList;
    //! Current truth table selected.
    wxString currentTable;
    //! File name for the collection.
    wxString fileName;
    //! Has the collection been modified after saving?
    bool modified;
    //! Do we need a filename to save the collection?
    bool needNewFile;
    void updateTable();
    void updateList();
    bool appendTables(bool mustDeleteTables, bool newFile);
    void deleteTables();
};

#endif /*TRUTHTABLEMANAGER_HPP_*/
