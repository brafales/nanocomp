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
 * \class TruthTableView
 * \brief Presentation class for the truth tables.
 * 
 * This is the presentation layer class for the truth table collection.
 * \author Bernat Ràfales Mulet <the_bell@users.sourceforge.net>
 * \version $Revision: 1.7 $
 */

#ifndef TRUTHTABLEVIEW_HPP_
#define TRUTHTABLEVIEW_HPP_

#include <wx/wx.h>
#include "truthTableCanvas.hpp"
#include "truthTableManager.hpp"

class TruthTableManager;

class TruthTableView : public wxPanel
{
public:
    TruthTableView(wxWindow* parent, wxWindowID id, TruthTableManager *controller);
    virtual ~TruthTableView();
    void updateTable(TruthTable table);
    void OnClick(TruthTableCanvasEvent &event);
    void OnNew(wxCommandEvent &event);
    void OnSelection(wxCommandEvent &event);
    void OnRemove(wxCommandEvent &event);
    void updateList(wxArrayString strings);
    void selectTable(wxString table);
    void errorMsg(wxString message);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    bool msgYesNo(wxString message);
    
private:
    DECLARE_EVENT_TABLE()
    void initControls();
    void initSizers();
    
    //Labels
    wxStaticText *labelTitle;
    wxStaticText *labelTable;
    wxStaticText *labelInfo;
    
    //Buttons
    wxBitmapButton *buttonNew;
    wxBitmapButton *buttonOpen;
    wxBitmapButton *buttonSave;
    wxBitmapButton *buttonSaveAs;
    wxBitmapButton *buttonRemove;
    
    //Other controls
    wxListBox *listTables;
    TruthTableCanvas *canvas;
    TruthTableManager *controller;
};

#endif /*TRUTHTABLEVIEW_HPP_*/
