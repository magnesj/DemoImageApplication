cmake_minimum_required (VERSION 2.8)

# Qt
if (CEE_BUILD_QT5)
  find_package(Qt5Widgets REQUIRED)
  include_directories(${Qt5Widgets_INCLUDE_DIRS})

  # Add compiler flags for building executables (-fPIE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
else() 
  find_package(Qt4 COMPONENTS QtCore QtGui QtMain REQUIRED)
  include(${QT_USE_FILE})
  set (CEE_QT_LIBRARIES ${QT_LIBRARIES})
endif()

project (cafUserInterface)

include_directories(
    ${cafProjectDataModel_SOURCE_DIR}
    ${cafCommand_SOURCE_DIR}
)

# These headers need to go through Qt's MOC compiler
set( QOBJECT_HEADERS    
    cafPdmUiCheckBoxDelegate.h
    cafPdmUiCheckBoxEditor.h
    cafPdmUiColorEditor.h
    cafPdmUiComboBoxEditor.h
    cafPdmUiDoubleSliderEditor.h
    cafPdmUiFilePathEditor.h
    cafPdmUiLineEditor.h
    cafPdmUiListEditor.h
    cafPdmUiListView.h
    cafPdmUiListViewEditor.h
    cafPdmUiPropertyView.h
    cafPdmUiPushButtonEditor.h
    cafPdmUiSliderEditor.h
    cafPdmUiTableView.h
    cafPdmUiTableViewDelegate.h
    cafPdmUiTableViewEditor.h
    cafPdmUiTableViewModel.h
    cafPdmUiTextEditor.h
    cafPdmUiTreeView.h
    cafPdmUiTreeViewModel.h
    cafPdmUiTreeViewEditor.h
    cafUiProcess.h
    cafUiTreeModelPdm.h
)

if (CEE_BUILD_QT5)
  qt5_wrap_cpp(MOC_FILES_CPP ${QOBJECT_HEADERS} )
else()
  qt4_wrap_cpp(MOC_FILES_CPP ${QOBJECT_HEADERS} )
endif()


add_library( ${PROJECT_NAME}
    
    # core base classes
    cafPdmUiFieldEditorHandle.cpp
    cafPdmUiFieldEditorHandle.h
    cafPdmUiObjectEditorHandle.cpp
    cafPdmUiObjectEditorHandle.h

    # field editors
    cafPdmUiCheckBoxDelegate.cpp
    cafPdmUiCheckBoxDelegate.h
    cafPdmUiCheckBoxEditor.cpp
    cafPdmUiCheckBoxEditor.h
    cafPdmUiColorEditor.cpp
    cafPdmUiColorEditor.h
    cafPdmUiComboBoxEditor.cpp
    cafPdmUiComboBoxEditor.h
    cafPdmUiDoubleSliderEditor.cpp
    cafPdmUiDoubleSliderEditor.h
    cafPdmUiFilePathEditor.cpp
    cafPdmUiFilePathEditor.h
    cafPdmUiLineEditor.cpp
    cafPdmUiLineEditor.h
    cafPdmUiListEditor.cpp
    cafPdmUiListEditor.h
    cafPdmUiPushButtonEditor.cpp
    cafPdmUiPushButtonEditor.h
    cafPdmUiSliderEditor.cpp
    cafPdmUiSliderEditor.h
    cafPdmUiTextEditor.cpp
    cafPdmUiTextEditor.h

    # object editors
    cafPdmUiDefaultObjectEditor.cpp
    cafPdmUiDefaultObjectEditor.h
    cafPdmUiListView.cpp
    cafPdmUiListViewEditor.cpp
    cafPdmUiListViewEditor.h
    cafPdmUiTableItemEditor.cpp
    cafPdmUiTableItemEditor.h
    cafPdmUiTableView.cpp
    cafPdmUiTableView.h
    cafPdmUiTableViewDelegate.cpp
    cafPdmUiTableViewDelegate.h
    cafPdmUiTableViewEditor.cpp
    cafPdmUiTableViewEditor.h
    cafPdmUiTableViewModel.cpp
    cafPdmUiTableViewModel.h
    cafPdmUiTreeEditorHandle.cpp
    cafPdmUiTreeEditorHandle.h
    cafPdmUiTreeItemEditor.cpp
    cafPdmUiTreeItemEditor.h
    cafPdmUiTreeView.cpp
    cafPdmUiTreeViewEditor.cpp
    cafPdmUiTreeViewEditor.h
    cafPdmUiTreeViewModel.cpp
    cafPdmUiTreeViewModel.h
    cafPdmUiPropertyView.cpp
    cafPdmUiPropertyView.h

    # div
    cafAboutDialog.cpp
    cafAboutDialog.h
    cafProgressInfo.cpp
    cafProgressInfo.h
    cafUiProcess.cpp
    cafUiProcess.h
    cafUiTreeModelPdm.cpp
    cafUiTreeModelPdm.h

    ${MOC_FILES_CPP}
)
