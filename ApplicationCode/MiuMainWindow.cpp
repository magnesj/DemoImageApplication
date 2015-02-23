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


#include "MiuMainWindow.h"
#include "MiaApplication.h"
#include "MimFilterSettings.h"
#include "MimImage.h"
#include "MimProject.h"

#include "cafAppExecCommandManager.h"
#include "cafCommandFeatureManager.h"
#include "cafCommandFeaturesCore.h"
#include "cafPdmField.h"
#include "cafPdmUiPropertyView.h"
#include "cafPdmUiTableView.h"
#include "cafPdmUiTreeView.h"
#include "cafReferenceHelper.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDialog>
#include <QDockWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSettings>
#include <QTableView>
#include <QTextEdit>
#include <QToolBar>
#include <QTreeView>
#include <QUndoStack>
#include <QUndoView>
#include <QLabel>
#include <QScrollArea>


MiuMainWindow* MiuMainWindow::sm_mainWindowInstance = NULL;

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
MiuMainWindow::MiuMainWindow()
{
    caf::AppExecCommandManager::instance()->enableUndoStack(true);

    m_centralFrame = new QFrame;
    QHBoxLayout* frameLayout = new QHBoxLayout(m_centralFrame);
    frameLayout->setMargin(0);
    setCentralWidget(m_centralFrame);

    createActions();
    createMenus();
    createToolBars();
    createDockPanels();

    m_imageLabel = new QLabel;
    m_imageLabel->setBackgroundRole(QPalette::Base);
    m_imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_imageLabel->setScaledContents(true);

    m_scrollArea = new QScrollArea;
    m_scrollArea->setBackgroundRole(QPalette::Dark);
    m_scrollArea->setWidget(m_imageLabel);
    m_centralFrame->layout()->addWidget(m_scrollArea);

    sm_mainWindowInstance = this;
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
MiuMainWindow* MiuMainWindow::instance()
{
    return sm_mainWindowInstance;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::createActions()
{
    m_newProjectAction = new QAction(QIcon(":/new.png"), "&New Project", this);
    connect(m_newProjectAction, SIGNAL(triggered()), SLOT(slotNewProject()));

    m_openProjectAction = new QAction(style()->standardIcon(QStyle::SP_DirOpenIcon), "&Open Project", this);
    connect(m_openProjectAction, SIGNAL(triggered()), SLOT(slotLoadProject()));

    m_saveProjectAction = new QAction(QIcon(":/Save.png"), "&Save Project", this);
    connect(m_saveProjectAction, SIGNAL(triggered()), SLOT(slotSaveProject()));

    m_saveProjectAsAction = new QAction(QIcon(":/Save.png"), "Save Project &As", this);
    connect(m_saveProjectAsAction, SIGNAL(triggered()), SLOT(slotSaveProjectAs()));

    m_closeProjectAction = new QAction("&Close Project", this);
    connect(m_closeProjectAction, SIGNAL(triggered()), SLOT(slotCloseProject()));

    for (int i = 0; i < MaxRecentFiles; ++i) {
        m_recentFileActions[i] = new QAction(this);
        m_recentFileActions[i]->setVisible(false);
        connect(m_recentFileActions[i], SIGNAL(triggered()),
            this, SLOT(openRecentFile()));
    }

    m_exitAction = new QAction(QIcon(":/exit.png"), tr("E&xit"), this);
    m_exitAction->setShortcuts(QKeySequence::Quit);
    m_exitAction->setStatusTip(tr("Exit the application"));
    connect(m_exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    // Edit actions
    m_undoAction = new QAction(QIcon(":/undo.png"), tr("Undo"), this);
    m_undoAction->setShortcut(QKeySequence::Undo);
    connect(m_undoAction, SIGNAL(triggered()), SLOT(slotUndo()));

    m_redoAction = new QAction(QIcon(":/redo.png"), tr("Redo"), this);
    m_redoAction->setShortcut(QKeySequence::Redo);
    connect(m_redoAction, SIGNAL(triggered()), SLOT(slotRedo()));

}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::createMenus()
{
    {
        QMenu* menu = menuBar()->addMenu("&File");
        menu->addAction(m_newProjectAction);
        menu->addAction(m_openProjectAction);
        menu->addAction(m_saveProjectAction);
        menu->addAction(m_saveProjectAsAction);
        menu->addSeparator();
        //menu->addAction(caf::PdmCommandFeatureManager::instance()->action("MdcExportSimulationInputFile"));
        
        menu->addSeparator();
        menu->addAction(m_closeProjectAction);

        m_recentFilesSeparatorAction = menu->addSeparator();
        for (int i = 0; i < MaxRecentFiles; ++i)
            menu->addAction(m_recentFileActions[i]);
        
        menu->addSeparator();
        menu->addAction(m_exitAction);
        
        updateRecentFileActions();
    }

    {
        m_editMenu = menuBar()->addMenu("&Edit");
        connect(m_editMenu, SIGNAL(aboutToShow()), SLOT(slotPopulateEditMenu()));
    }
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::createToolBars()
{
    {
        QToolBar* toolbar = addToolBar("Project");
        toolbar->setObjectName("Project");

        toolbar->addAction(m_newProjectAction);
        toolbar->addAction(m_openProjectAction);
        toolbar->addAction(m_saveProjectAction);
        //toolbar->addAction(caf::PdmCommandFeatureManager::instance()->action("MdcStartSimulation"));

    }
   

    {
        QToolBar* toolbar = addToolBar("Edit");
        toolbar->setObjectName("Edit");

        toolbar->addAction(m_undoAction);
        toolbar->addAction(m_redoAction);
    }
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------

void MiuMainWindow::createDockPanels()
{

    {
        QDockWidget* dockWidget = new QDockWidget("Project Tree", this);
        dockWidget->setObjectName("dockWidget");
        dockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);

        m_projectTreeView = new caf::PdmUiTreeView(this);
        m_projectTreeView->enableDefaultContextMenu(true);
        dockWidget->setWidget(m_projectTreeView);

        addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

        connect(m_projectTreeView->treeView()->selectionModel(), SIGNAL( selectionChanged(const QItemSelection & , const QItemSelection & ) ), 
                this, SLOT(selectedObjectsChanged(const QItemSelection& , const QItemSelection & )));

    }

    {
        QDockWidget* dockWidget = new QDockWidget("Property Editor", this);
        dockWidget->setObjectName("dockWidget");
        dockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);

        m_pdmUiPropertyView = new caf::PdmUiPropertyView(this);
        dockWidget->setWidget(m_pdmUiPropertyView);

        m_pdmUiPropertyView->layout()->setContentsMargins(5,0,0,0);

        addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
    }

 
   
    {
        QDockWidget* dockWidget = new QDockWidget("Undo stack", this);
        dockWidget->setObjectName("dockWidget");
        dockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);

        m_undoView = new QUndoView(this);
        m_undoView->setStack(caf::AppExecCommandManager::instance()->undoStack());
        connect(caf::AppExecCommandManager::instance()->undoStack(), SIGNAL(indexChanged(int)), SLOT(slotIndexChanged()));

        dockWidget->setWidget(m_undoView);

        addDockWidget(Qt::RightDockWidgetArea, dockWidget);
    }

   
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::selectedObjectsChanged(const QItemSelection& selected, const QItemSelection & deselected)
{
    std::vector<caf::PdmUiItem*> uiItems;

    m_projectTreeView->selectedObjects(uiItems);
    if (uiItems.size() > 0)
    {
        caf::PdmObject* pdmObject = dynamic_cast<caf::PdmObject*>(uiItems[0]);
        m_pdmUiPropertyView->showProperties(pdmObject);

        MimImage* mimImage = dynamic_cast<MimImage*>(pdmObject);
        if (mimImage)
        {
            mimImage->redrawImageInDisplayWidget();
        }

        MimFilterSettings* mimFilter = dynamic_cast<MimFilterSettings*>(pdmObject);
        if (mimFilter)
        {
            mimFilter->updateDisplayOfConnectedImage();
        }

    }
    else
    {
        m_pdmUiPropertyView->showProperties(NULL);
    }

}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
caf::PdmPointersFieldHandle* MiuMainWindow::selectedItemInProjectTree()
{
    std::vector<caf::PdmUiItem*> uiItems;
    m_projectTreeView->selectedObjects(uiItems);

    caf::PdmPointersFieldHandle* listField = NULL;
    if (uiItems.size() > 0)
    {
        listField = dynamic_cast<caf::PdmPointersFieldHandle*>(uiItems[0]);

        if (!listField)
        {
            caf::PdmObject* pdmObject = dynamic_cast<caf::PdmObject*>(uiItems[0]);

            if (pdmObject)
            {
                std::vector<caf::PdmFieldHandle*> parentFields;
                pdmObject->parentFields(parentFields);

                if (parentFields.size() > 0)
                {
                    listField = dynamic_cast<caf::PdmPointersFieldHandle*>(parentFields[0]);
                }
            }
        }
    }

    return listField;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::cleanupGuiBeforeProjectClose()
{
    m_pdmUiPropertyView->showProperties(NULL);
    m_projectTreeView->setPdmItem(NULL);

    m_imageLabel->clear();
 
    caf::AppExecCommandManager::instance()->undoStack()->clear();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::slotLoadProject()
{
    if (!MiuMainWindow::checkSaveProjectIfProjectHasChanged())
    {
        return;
    }

    MiaApplication::instance()->loadProject();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::slotSaveProject()
{
    MiaApplication::instance()->saveProject();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::slotSaveProjectAs()
{
    MiaApplication::instance()->saveProjectPromptForFileName();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::slotCloseProject()
{
    if (!MiuMainWindow::checkSaveProjectIfProjectHasChanged())
    {
        return;
    }

   MiaApplication::instance()->closeProject();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::configureGuiProjectChanged()
{
    markUndoStackAsClean();

    caf::SelectionManager::instance()->clear();

    MimProject* proj = MiaApplication::instance()->project();

    m_projectTreeView->setPdmItem(proj);
    caf::ReferenceHelper::instance()->setRoot(proj);
    m_projectTreeView->setPdmItem(proj);

    if (proj)
    {
        proj->updateConnectedEditors();
    }
    
    m_projectTreeView->treeView()->expandAll();

    setWindowCaptionFromAppState();
   
 
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::openRecentFile()
{
    if (!MiuMainWindow::checkSaveProjectIfProjectHasChanged())
    {
        return;
    }

    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        MiaApplication::instance()->loadProject(action->data().toString());
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(QFileInfo(files[i]).fileName());
        m_recentFileActions[i]->setText(text);
        m_recentFileActions[i]->setData(files[i]);
        m_recentFileActions[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        m_recentFileActions[j]->setVisible(false);

    m_recentFilesSeparatorAction->setVisible(numRecentFiles > 0);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::addRecentFiles(const QString& file)
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(file);
    files.removeAll("");
    files.prepend(file);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);

    updateRecentFileActions();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::setWindowCaptionFromAppState()
{
    QString projFileName;

    MimProject* proj = MiaApplication::instance()->project();
    if (proj)
    {
       projFileName = proj->fileName();
    }

    if (projFileName.isEmpty())
    {
        projFileName = "Untitled project";
    }
    else
    {
        addRecentFiles(projFileName);
    }

    if (projFileName.isEmpty()) projFileName = "Untitled project";

    setWindowFilePath(projFileName);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::slotPopulateEditMenu()
{
    m_editMenu->clear();

    if (m_undoView->stack())
    {
        m_editMenu->addAction(m_redoAction);
        m_editMenu->addAction(m_undoAction);

        m_redoAction->setDisabled(!m_undoView->stack()->canRedo());
        m_undoAction->setDisabled(!m_undoView->stack()->canUndo());

    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::slotRedo()
{
    if (m_undoView->stack() && m_undoView->stack()->canRedo())
    {
        m_undoView->stack()->redo();
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::slotUndo()
{
    if (m_undoView->stack() && m_undoView->stack()->canUndo())
    {
        m_undoView->stack()->undo();
    }
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::loadLastUsedProject()
{
    if (!MiuMainWindow::checkSaveProjectIfProjectHasChanged())
    {
        return;
    }

    QString projFilname = m_recentFileActions[0]->data().toString();
    if (!projFilname.isEmpty())
    {
        MiaApplication::instance()->loadProject(projFilname);
    }
    else
    {
        this->slotNewProject();
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::saveWinGeoAndDockToolBarLayout()
{
    // Company and appname set through QCoreApplication
    QSettings settings;

    QByteArray winGeo = saveGeometry();
    settings.setValue("winGeometry", winGeo);

    QByteArray layout = saveState();
    settings.setValue("dockAndToolBarLayout", layout);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::loadWinGeoAndDockToolBarLayout()
{
    // Company and appname set through QCoreApplication
    QSettings settings;

    QVariant winGeo = settings.value("winGeometry");
    QVariant layout = settings.value("dockAndToolBarLayout");

    if (winGeo.isValid())
    {
        if (restoreGeometry(winGeo.toByteArray()))
        {
            if (layout.isValid())
            {
                restoreState(layout.toByteArray());
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::closeEvent(QCloseEvent* event)
{
    if (!MiuMainWindow::checkSaveProjectIfProjectHasChanged())
    {
        event->ignore();
        return;
    }
    
    if (!MiaApplication::instance()->closeProject())
    {
        event->ignore();
        return;
    }
   
    saveWinGeoAndDockToolBarLayout();

    event->accept();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::slotNewProject()
{
    if (!MiuMainWindow::checkSaveProjectIfProjectHasChanged())
    {
        return;
    }

    MiaApplication::instance()->newProject();

}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool MiuMainWindow::checkSaveProjectIfProjectHasChanged()
{
    if (!m_undoView->stack()->isClean())
    {
        QMessageBox msgBox;
        msgBox.setText("The project has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Save)
        {
            slotSaveProject();
        }
        else if (ret == QMessageBox::Cancel)
        {
            return false;
        }
    }

    return true;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::markUndoStackAsClean()
{
    m_undoView->stack()->setClean();

    slotIndexChanged();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::customMenuRequested(QPoint pos)
{

}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::slotIndexChanged()
{
    m_redoAction->setDisabled(!m_undoView->stack()->canRedo());
    m_undoAction->setDisabled(!m_undoView->stack()->canUndo());
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MiuMainWindow::setImage(const QImage& image)
{
    m_imageLabel->setPixmap(QPixmap::fromImage(image));
    m_imageLabel->adjustSize();
}

