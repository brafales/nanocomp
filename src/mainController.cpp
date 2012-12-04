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

// $Revision: 1.19 $
 
#include "mainController.hpp"
#include <wx/filename.h>

//! Constructor.
 MainController::MainController()
 {
    initControllers();
    fileName = wxEmptyString;
    simulating = false;
 }


//! Destructor.
MainController::~MainController()
{
    delete rules;
    delete layouts;
    delete forbiddenPatterns;
    delete truthTables;  
}


//! Sets the application main window.
/*!
   \param frame the application main window.
*/
 void MainController::setNanoFrame(NanoFrame *frame)
 {
    this->frame = frame;
 }


//! Member accessor.
/*!
   \return The application main window.
*/
NanoFrame* MainController::getNanoFrame()
 {
    return frame;
 }


//! Sets the rule controller.
/*!
   \param rules the rule controller.
*/
void MainController::setRules(RuleManager *rules)
{
    this->rules = rules;
}


//! Sets the space controller.
/*!
   \param layouts the space controller.
*/
void MainController::setLayouts(LayoutManager *layouts)
{
    this->layouts = layouts;
}


//! Sets the forbidden pattern controller.
/*!
   \param forbiddenPatterns the forbidden pattern controller.
*/
void MainController::setForbiddenPatterns(ForbiddenPatternManager *forbiddenPatterns)
{
    this->forbiddenPatterns = forbiddenPatterns;
}


//! Sets the truth table controller.
/*!
   \param truthTables the truth table controller.
*/
void MainController::setTruthTables(TruthTableManager *truthTables)
{
    this->truthTables = truthTables;
}


//! Creates the controllers.
void MainController::initControllers()
{
    rules = new RuleManager(this);
    forbiddenPatterns = new ForbiddenPatternManager(this);
    truthTables = new TruthTableManager(this);
    layouts = new LayoutManager(this, truthTables, forbiddenPatterns, rules);
    simulation = new SimulationManager(this, truthTables, forbiddenPatterns, rules, layouts);
}


//! Returns the rule controller.
/*!
   \return The rule controller.
*/
RuleManager* MainController::getRules()
{
    return rules;    
}


//! Returns the space controller.
/*!
   \return The space controller.
*/
LayoutManager* MainController::getLayouts()
{
    return layouts;
}


//! Returns the forbidden pattern controller.
/*!
   \return The forbidden pattern controller.
*/
ForbiddenPatternManager* MainController::getForbiddenPatterns()
{
    return forbiddenPatterns;
}


//! Returns the truth table controller.
/*!
   \return The truth table controller.
*/
TruthTableManager* MainController::getTruthTables()
{
    return truthTables;
}


//! Method called when the table collection changes.
void MainController::tablesChanged()
{
    layouts->tablesChanged();
}


//! Method called when the pattern collection changes.
void MainController::FPsChanged()
{
    layouts->FPsChanged();
}


//! Method called when the rule collection changes.
void MainController::rulesChanged()
{
    layouts->rulesChanged();
}


//! Sets a message on the status bar.
/*!
   \param message the status bar message.
*/
void MainController::setStatusMessage(wxString message)
{
    frame->setStatusMessage(message);
}


//TODO: check the path of all filenames since they all must be saved
//in the same directory to work
//So we'll have to change the order of file asking or something
//! Save project.
/*!
   Saves a project. The different components are saved and then
   the project is copied into a file. All the components are saved on
   the same directory.
*/
void MainController::save()
{
   if (rules->saveFile() && forbiddenPatterns->saveFile() && truthTables->saveFile() && layouts->saveFile())
    {
        if (fileName == wxEmptyString)
        {
            wxString name = fileView.saveFileChoose(frame, _("Select a project file to save"), _("*.ncp"));
    
            if (name != wxEmptyString)
            {
                fileName = name;
            }
            else
            {
                return;
            }
        }
        //Here we save all files again in the same directory as the project
        //TODO: check errors
        wxFileName path(fileName);
        wxString pPath = path.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
        wxFileName ruleFile(rules->getFileName());
        wxFileName FPFile(forbiddenPatterns->getFileName());
        wxFileName tableFile(truthTables->getFileName());
        wxFileName layoutFile(layouts->getFileName());
        
        if (rules->getFileName() != wxEmptyString)
        {
            rules->saveFilename(pPath + ruleFile.GetFullName());
        }
        if (forbiddenPatterns->getFileName() != wxEmptyString)
        {
            forbiddenPatterns->saveFilename(pPath + FPFile.GetFullName());
        }
        if (truthTables->getFileName() != wxEmptyString)
        {
            truthTables->saveFilename(pPath + tableFile.GetFullName());
        }
        if (layouts->getFileName() != wxEmptyString)
        {
            layouts->saveFilename(pPath + layoutFile.GetFullName());
        }
        
        wxFileName extension(fileName);
        extension.SetExt(_("ncp"));
        fileName = extension.GetFullPath();
        wxFileOutputStream fileStream(fileName);
        wxTextOutputStream *outStream = new wxTextOutputStream(fileStream);
        saveFileNames(outStream);
        saveTables(outStream);
        saveRules(outStream);
        savePatterns(outStream);
        delete outStream;
        frame->setTitleBar(fileName);
    }
}


//! Saves the project in a new file.
void MainController::saveAs()
{
if (rules->saveFile() && forbiddenPatterns->saveFile() && truthTables->saveFile() && layouts->saveFile())
    {
        wxString name = fileView.saveFileChoose(frame, _("Select a project file to save"), _("*.ncp"));

        if (name != wxEmptyString)
        {
            fileName = name;
        }
        else
        {
            return;
        }
    }
    //Here we save all files again in the same directory as the project
    //TODO: check errors
    wxFileName path(fileName);
    wxString pPath = path.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
    wxFileName ruleFile(rules->getFileName());
    wxFileName FPFile(forbiddenPatterns->getFileName());
    wxFileName tableFile(truthTables->getFileName());
    wxFileName layoutFile(layouts->getFileName());
    
    if (rules->getFileName() != wxEmptyString)
    {
        rules->saveFilename(pPath + ruleFile.GetFullName());
    }
    if (forbiddenPatterns->getFileName() != wxEmptyString)
    {
        forbiddenPatterns->saveFilename(pPath + FPFile.GetFullName());
    }
    if (truthTables->getFileName() != wxEmptyString)
    {
        truthTables->saveFilename(pPath + tableFile.GetFullName());
    }
    if (layouts->getFileName() != wxEmptyString)
    {
        layouts->saveFilename(pPath + layoutFile.GetFullName());
    }
            
    wxFileName extension(fileName);
    extension.SetExt(_("ncp"));
    fileName = extension.GetFullPath();
    wxFileOutputStream fileStream(fileName);
    wxTextOutputStream *outStream = new wxTextOutputStream(fileStream);
    saveFileNames(outStream);
    saveTables(outStream);
    saveRules(outStream);
    savePatterns(outStream);
    delete outStream;
    frame->setTitleBar(fileName);
}


//! Opens a project file.
void MainController::open()
{
    //First we ask for saving the project if some part
    //has been modified
    if (rules->isModified() || 
        forbiddenPatterns->isModified() ||
        truthTables->isModified() ||
        layouts->isLayoutModified())
    {
        save();
    }
    
    //Reset collections
    rules->clean();
    forbiddenPatterns->clean();
    truthTables->clean();
    layouts->clean();
    
    wxString name = fileView.openFileChoose(frame, _("Select a project file to open"), _("*.ncp"));
    
    if (name != wxEmptyString)
    {
        fileName = name;
    }
    else
    {
        return;
    }
    
    if (!openFile(fileName))
    {
        //Clean
        rules->clean();
        forbiddenPatterns->clean();
        truthTables->clean();
        layouts->clean();
        frame->setTitleBar(fileName);
        fileName = wxEmptyString;
        frame->setTitleBar(_("Untitled"));
    }
    else
    {
        frame->setTitleBar(fileName);
    }
}


//! Creates a new project.
/*!
   The user is asked to save the current project if desired.
*/
void MainController::newFile()
{
    //Save 
    save();
    
    //New
    fileName = wxEmptyString;
    rules->clean();
    forbiddenPatterns->clean();
    truthTables->clean();
    layouts->clean();
    frame->setTitleBar(_("Untitled"));
}


//! Saves the collection file names to a stream.
/*!
   \param stream the stream to write.
*/
void MainController::saveFileNames(wxTextOutputStream *stream)
{
        *stream << _("rules=") << wxFileName(rules->getFileName()).GetFullName() << endl;
        *stream << _("fps=") << wxFileName(forbiddenPatterns->getFileName()).GetFullName() << endl;
        *stream << _("tables=") << wxFileName(truthTables->getFileName()).GetFullName() << endl;
        *stream << _("layout=") << wxFileName(layouts->getFileName()).GetFullName() << endl;
}


//! Saves the tables information to a stream.
/*!
   \param stream the stream to write.
*/
void MainController::saveTables(wxTextOutputStream *stream)
{
    map< wxString, vector<coordinate> >  tableInputs = layouts->getTableInputs();
    map< wxString, vector<coordinate> > tableOutputs = layouts->getTableOutputs();
    map< wxString, bool> tableEnabled = layouts->getTableEnabled();
    
    //Save all table information
    for (map< wxString, bool>::iterator i = tableEnabled.begin(); i != tableEnabled.end(); i++)
    {
        wxString table = (*i).first;
        //Name
        *stream << table << endl;
        //Enabled/disabled
        if ((*i).second)
        {
            *stream << 1 << endl;
        }
        else
        {
            *stream << 0 << endl;
        }
        //Inputs
        vector<coordinate> inputs = tableInputs[table];
        for (unsigned int j = 0; j < inputs.size(); j++)
        {
            *stream << inputs[j].x << endl << inputs[j].y << endl;
        }
        
        //Outputs
        vector<coordinate> outputs = tableOutputs[table];
        for (unsigned int j = 0; j < outputs.size(); j++)
        {
            *stream << outputs[j].x << endl << outputs[j].y << endl;
        }
    }
}


//! Saves the rules information to a stream.
/*!
   \param stream the stream to write.
*/
void MainController::saveRules(wxTextOutputStream *stream)
{
    map< unsigned int, bool> ruleEnabled = layouts->getRuleEnabled();
    for (map< unsigned int, bool>::iterator i = ruleEnabled.begin(); i != ruleEnabled.end(); i++)
    {
        *stream << (*i).first << endl << (*i).second << endl;
    }
}


//! Saves the patterns information to a stream.
/*!
   \param stream the stream to write.
*/
void MainController::savePatterns(wxTextOutputStream *stream)
{
    map< unsigned int, bool> FPEnabled = layouts->getFPEnabled();
    for (map< unsigned int, bool>::iterator i = FPEnabled.begin(); i != FPEnabled.end(); i++)
    {
        *stream << (*i).first << endl << (*i).second << endl;
    }
}


//! Opens a project file.
/*!
   \param file the file to open.
   \return True iif the project was correctly opened.
*/
bool MainController::openFile(wxString file)
{
    bool result;
    wxFileName path(file);
    wxString pPath = path.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
    
    wxFileInputStream *fileStream = new wxFileInputStream(file);
    wxTextInputStream *inStream = new wxTextInputStream(*fileStream);
    
    wxString tempFile;
    wxString line;
    
    //Rules
    line = inStream->ReadLine();
    //TODO: look another way to do this
    if (fileStream->LastRead() == 0 || fileStream->Eof())
    {
        //ERROR
        //TODO: when error "clean" all project
        frame->errorMsg(_("Error opening rule file."));
        return false;
    }
    if (!line.StartsWith(_("rules="), &tempFile))
    {
        //ERROR
        //TODO: when error "clean" all project
        frame->errorMsg(_("Error opening rule file."));
        return false;
    }
    else
    {
        if (!tempFile.IsEmpty())
        {
            result = rules->openPFile(pPath + tempFile);
            if (!result)
            {
                frame->errorMsg(_("Error opening rule file."));
                return false;
            }
        }
    }
    
    //Patterns
    line = inStream->ReadLine();
    if (fileStream->LastRead() == 0 || fileStream->Eof())
    {
        //ERROR
        //TODO: when error "clean" all project
        frame->errorMsg(_("Error opening pattern file."));
        return false;
    }
    if (!line.StartsWith(_("fps="), &tempFile))
    {
        //ERROR
        //TODO: when error "clean" all project
        frame->errorMsg(_("Error opening pattern file."));
        return false;
    }
    else
    {
        if (!tempFile.IsEmpty())
        {
            result = result && forbiddenPatterns->openPFile(pPath + tempFile);
            if (!result)
            {
                frame->errorMsg(_("Error opening forbidden pattern file."));
                return false;
            }
        }
    }
    
    //Truth Tables
    line = inStream->ReadLine();
    if (fileStream->LastRead() == 0 || fileStream->Eof())
    {
        //ERROR
        //TODO: when error "clean" all project
        frame->errorMsg(_("Error opening table file."));
        return false;
    }
    if (!line.StartsWith(_("tables="), &tempFile))
    {
        //ERROR
        //TODO: when error "clean" all project
        frame->errorMsg(_("Error opening table file."));
        return false;
    }
    else
    {
        if (!tempFile.IsEmpty())
        {
            result = result && truthTables->openPFile(pPath + tempFile);
            if (!result)
            {
                frame->errorMsg(_("Error opening truth table file."));
                return false;
            }
        }
    }
    
    //Layout
    line = inStream->ReadLine();
    if (fileStream->LastRead() == 0 || fileStream->Eof())
    {
        //ERROR
        //TODO: when error "clean" all project
        frame->errorMsg(_("Error opening layout file."));
        return false;
    }
    if (!line.StartsWith(_("layout="), &tempFile))
    {
        //ERROR
        //TODO: when error "clean" all project
        frame->errorMsg(_("Error opening layout file."));
        return false;
    }
    else
    {
        if (!tempFile.IsEmpty())
        {
            result = result && layouts->openPFile(pPath + tempFile);
            if (!result)
            {
                frame->errorMsg(_("Error opening layout file."));
                return false;
            }
        }
    }
    
    //Tables
    if (!openTables(fileStream, inStream))
    {
        frame->errorMsg(_("Error mapping tables."));
        result = false;
    }
    else
    {
        result = result && true;
    }
    
    //Rules
    if (!openRules(fileStream, inStream))
    {
        frame->errorMsg(_("Error mapping rules."));
        result = false;
    }
    else
    {
        result = result && true;
    }
    //Patterns
    if (!openFPs(fileStream, inStream))
    {
        frame->errorMsg(_("Error mapping patterns."));
        result = false;
    }
    else
    {
        result = result && true;
    }
    
    return result;
}


//! Opens the tables information from a stream.
/*!
   \param fileStream file stream.
   \param inStream the text input stream.
   \return True if the tables information was correctly opened.
*/
bool MainController::openTables(wxFileInputStream *fileStream, wxTextInputStream *inStream)
{
    wxString table;
    wxString temp;
    long int iTemp;
    bool enabled;
    long int x;
    long int y;
    
    for (int i = 0; i < layouts->getTables(); i++)
    {
        table = inStream->ReadLine();
        if (fileStream->LastRead() == 0 || fileStream->Eof())
        {
            return false;
        }
        //We've read a table name, proceed to see if enabled or not
        temp = inStream->ReadLine();
        if (fileStream->LastRead() == 0 || fileStream->Eof())
        {
            return false;
        }
        if (!temp.ToLong(&iTemp))
        {
            return false;
        }
        
        if (iTemp == 0)
        {
            enabled = false;
        }
        else if (iTemp == 1)
        {
            enabled = true;
        }
        else
        {
            return false;
        }
        
        layouts->tableChecked(table, enabled);
        //Inputs
        for (int j = 0; j < truthTables->getInputs(table); j++)
        {
            temp = inStream->ReadLine();
            if (fileStream->LastRead() == 0 || fileStream->Eof())
            {
                return false;
            }
            if (!temp.ToLong(&x))
            {
                return false;
            }
            temp = inStream->ReadLine();
            if (fileStream->LastRead() == 0 || fileStream->Eof())
            {
                return false;
            }
            if (!temp.ToLong(&y))
            {
                return false;
            }
            if (!layouts->setInput(table, j, x, y))
            {
                return false;
            }
        }
        
        //Outputs
        for (int j = 0; j < truthTables->getOutputs(table); j++)
        {
            temp = inStream->ReadLine();
            if (fileStream->LastRead() == 0 || fileStream->Eof())
            {
                return false;
            }
            if (!temp.ToLong(&x))
            {
                return false;
            }
            temp = inStream->ReadLine();
            if (fileStream->LastRead() == 0 || fileStream->Eof())
            {
                return false;
            }
            if (!temp.ToLong(&y))
            {
                return false;
            }
            if (!layouts->setOutput(table, j, x, y))
            {
                return false;
            }
        }
    }
    return true;
}


//! Opens the rules information from a stream.
/*!
   \param fileStream file stream.
   \param inStream the text input stream.
   \return True if the rules information was correctly opened.
*/
bool MainController::openRules(wxFileInputStream *fileStream, wxTextInputStream *inStream)
{
    wxString temp;
    long int iTemp;
    bool enabled;
    unsigned long int rule;
    
    for (int i = 0; i < layouts->getRules(); i++)
    {
        temp = inStream->ReadLine();
        if (fileStream->LastRead() == 0 || fileStream->Eof())
        {
            return false;
        }
        if (!temp.ToULong(&rule))
        {
            return false;
        }
        temp = inStream->ReadLine();
        if (fileStream->LastRead() == 0 || fileStream->Eof())
        {
            return false;
        }
       if (!temp.ToLong(&iTemp))
        {
            return false;
        }
        
        if (iTemp == 0)
        {
            enabled = false;
        }
        else if (iTemp == 1)
        {
            enabled = true;
        }
        else
        {
            return false;
        }
        layouts->ruleChecked(rule, enabled);
    }
        return true;
}


//! Opens the forbidden patterns information from a stream.
/*!
   \param fileStream file stream.
   \param inStream the text input stream.
   \return True if the forbidden patterns information was correctly opened.
*/
bool MainController::openFPs(wxFileInputStream *fileStream, wxTextInputStream *inStream)
{
    wxString temp;
    long int iTemp;
    bool enabled;
    unsigned long int fp;
    
    for (int i = 0; i < layouts->getFPs(); i++)
    {
        temp = inStream->ReadLine();
        if (fileStream->LastRead() == 0 || fileStream->Eof())
        {
            return false;
        }
        if (!temp.ToULong(&fp))
        {
            return false;
        }
        temp = inStream->ReadLine();
        if (fileStream->LastRead() == 0 || fileStream->Eof())
        {
            return false;
        }
       if (!temp.ToLong(&iTemp))
        {
            return false;
        }
        
        if (iTemp == 0)
        {
            enabled = false;
        }
        else if (iTemp == 1)
        {
            enabled = true;
        }
        else
        {
            return false;
        }
        layouts->FPChecked(fp, enabled);
    }
    return true;
}


//! Starts the simulation.
void MainController::startSimulation()
{
    if (!simulating)
    {
        simulation->prepareSimulation();
        setStatusMessage(_("Simulation in progress"));
        simulating = true;
    }
    else
    {
        frame->showMessage(_("Another simulation is in progress,\nplease close all simulation windows before starting a new one"));
    }
}


//! Checks all the parameters to start a simulation.
void MainController::prepareSimulation()
{
    if (!rules->checkRules())
    {
        frame->errorMsg(_("Check Rules for errors"));
    }
    else if (!forbiddenPatterns->checkPatterns())
    {
        frame->errorMsg(_("Check Forbidden Patterns for errors"));
        setStatusMessage(_("Check Forbidden Patterns for errors"));
    }
    else if (!layouts->checkLayout())
    {
        frame->errorMsg(_("Check there is a truth table checked and it has its inputs and outputs assigned"));
        setStatusMessage(_("Check there is a truth table checked and it has its inputs and outputs assigned"));
    }
    else
    {
        layouts->enableSimulation();
        setStatusMessage(_("Ready to simulate"));
    }
}


//! Method called when any collection changes its status.
void MainController::elementChanged()
{
    layouts->elementChanged();
}


//! Method called when a simulation has ended.
void MainController::endSimulation()
{
    simulating = false;
    frame->setStatusMessage(_("Simulation finished"));
}
