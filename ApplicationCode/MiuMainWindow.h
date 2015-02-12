
#pragma once

#include <QMainWindow>
#include <QFrame>
#include <QItemSelection>

class QActionGroup;
class QUndoView;
class QUndoCommand;

namespace caf
{
    class PdmUiTreeView;
    class PdmUiPropertyView;
    class PdmUiTableView;
    class PdmPointersFieldHandle;
}

//==================================================================================================
//
// 
//
//==================================================================================================
class MiuMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MiuMainWindow();
    static MiuMainWindow* instance();
    
    void cleanupGuiBeforeProjectClose();
    void configureGuiProjectChanged();

    void loadLastUsedProject();

    void loadWinGeoAndDockToolBarLayout();

    void updateApplicationStateUI();
    void markUndoStackAsClean();

    void setSelectionInfoText(const QString& text);

    caf::PdmPointersFieldHandle* selectedItemInProjectTree();


protected:
    virtual void	closeEvent(QCloseEvent* event);

private:
    void        createActions();
    void        createMenus();
    void        createToolBars();
    void        createDockPanels();

    void        updateRecentFileActions();
    void        addRecentFiles(const QString& file);

    void        setWindowCaptionFromAppState();
    void        saveWinGeoAndDockToolBarLayout();

    bool        checkSaveProjectIfProjectHasChanged();
    
private slots:
    void selectedObjectsChanged(const QItemSelection& selected, const QItemSelection & deselected);

    void slotNewProject();
    void slotLoadProject();
    void slotSaveProject();
    void slotSaveProjectAs();
    void slotCloseProject();

    void openRecentFile();

    // Edit
    void slotPopulateEditMenu();

    void customMenuRequested(QPoint pos);

    void slotRedo();
    void slotUndo();
    void slotIndexChanged();
private:
    friend class EvuGlobalEventFilter;

    static MiuMainWindow* sm_mainWindowInstance;

private:
    QFrame*         m_centralFrame;

    QAction*        m_newProjectAction;
    QAction*        m_openProjectAction;
    QAction*        m_closeProjectAction;
    QAction*        m_saveProjectAction;
    QAction*        m_saveProjectAsAction;
    QAction*        m_exitAction;

    // View
    QAction*        m_zoomToFit;
    QAction*        m_defaultView;

    // Edit
    QMenu*          m_editMenu;
    QAction*        m_undoAction;
    QAction*        m_redoAction;

    QUndoView*      m_undoView;

    // Recent files
    enum { MaxRecentFiles = 5 };
    QAction*        m_recentFilesSeparatorAction;
    QMenu*          m_recentFilesMenu;
    QAction*        m_recentFileActions[MaxRecentFiles];

    caf::PdmUiTreeView*     m_projectTreeView;
    caf::PdmUiPropertyView* m_pdmUiPropertyView;

};
