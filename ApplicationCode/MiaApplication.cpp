/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2014 Ceetron Solutions AS
// 
//  <APPLICATION_NAME> is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
// 
//  <APPLICATION_NAME> is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
// 
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////


#include "MiaApplication.h"
          
#include "MimProject.h"
#include "MiuMainWindow.h"

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>


#define PROJECT_ORGANIZATION_NAME "Ceetron Solutions AS"
#define PROJECT_APPLICATION_NAME "MinimalApplication"
 
//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
MiaApplication::MiaApplication(int& argc, char** argv)
:   QApplication(argc, argv)
{
    // USed to get registry settings in the right place
    QCoreApplication::setOrganizationName(PROJECT_ORGANIZATION_NAME);
    QCoreApplication::setApplicationName(PROJECT_APPLICATION_NAME);

    //setWindowIcon(QIcon(":/Element16x16.png"));

}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
MiaApplication::~MiaApplication()
{
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
MiaApplication* MiaApplication::instance()
{
    return static_cast<MiaApplication*>qApp;
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
MimProject* MiaApplication::project()
{
    return m_project.p();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool MiaApplication::loadProject(const QString& projectFileName)
{
    if (!closeProject()) return false;

    if (!QFile::exists(projectFileName)) return false;

    m_project = new MimProject;

    m_project->fileName = projectFileName;
    m_project->readFile();

    MiuMainWindow* mainWnd = MiuMainWindow::instance();
    mainWnd->configureGuiProjectChanged();

    return true;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool MiaApplication::loadProject()
{
    QString startPath = QDir::home().absolutePath();
    QString fileName = QFileDialog::getOpenFileName(NULL, "Open Project", startPath, "Project (*.mip);;All files(*.*)");

    if (fileName.isEmpty()) return true;

    return loadProject(fileName);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool MiaApplication::saveProject()
{
    if (!QFile::exists(m_project->fileName()))
    {
        return saveProjectPromptForFileName();
    }
    else
    {
        return saveProjectAs(m_project->fileName());
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool MiaApplication::saveProjectAs(const QString& fileName)
{
    m_project->fileName = fileName;
    m_project->writeFile();

    MiuMainWindow* mainWnd = MiuMainWindow::instance();
    mainWnd->markUndoStackAsClean();

    return true;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool MiaApplication::saveProjectPromptForFileName()
{
    QString startPath;
    if (!m_project->fileName().isEmpty())
    {
        QFileInfo fi(m_project->fileName());
        startPath = fi.absolutePath();
    }
    else
    {
        startPath = QDir::home().absolutePath();
    }

    startPath += "/MinimalApplication.mip";

    QString fileName = QFileDialog::getSaveFileName(NULL, tr("Save File"), startPath, tr("Project Files (*.mip);;All files(*.*)"));
    if (fileName.isEmpty())
    {
        return false;
    }

    bool bSaveOk = saveProjectAs(fileName);
    return bSaveOk;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool MiaApplication::closeProject()
{
    // TODO: Check if document is dirty before close

    MiuMainWindow* mainWnd = MiuMainWindow::instance();
    mainWnd->cleanupGuiBeforeProjectClose();

    if (m_project)
    {
        delete m_project;
    }

    m_project = NULL;

    mainWnd->configureGuiProjectChanged();

    return true;
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiaApplication::newProject()
{
    if (closeProject())
    {
        m_project = new MimProject;
    }

    MiuMainWindow* mainWnd = MiuMainWindow::instance();
    mainWnd->configureGuiProjectChanged();
}
