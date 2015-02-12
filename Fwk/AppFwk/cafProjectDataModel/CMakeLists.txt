cmake_minimum_required (VERSION 2.8)

project (cafProjectDataModel)

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


add_library( ${PROJECT_NAME}
	cafAppEnum.h
	cafFactory.h
	cafClassTypeName.h
	cafFixedArray.h
	cafOmpMutex.h
	cafUiTreeItem.h
	cafPdmDocument.cpp
	cafPdmDocument.h
	cafPdmField.cpp
	cafPdmField.h
	cafPdmField.inl
	cafPdmFieldImpl.h
	cafPdmObject.cpp
	cafPdmObject.h
	cafPdmObjectFactory.h
	cafPdmPointer.cpp
	cafPdmPointer.h
	cafPdmUiEditorHandle.cpp
	cafPdmUiEditorHandle.h
	cafPdmUiItem.cpp
	cafPdmUiItem.h
	cafPdmUiOrdering.cpp
	cafPdmUiOrdering.h
	cafPdmUiTreeOrdering.cpp
	cafPdmUiTreeOrdering.h
	cafReferenceHelper.cpp
    cafReferenceHelper.h
)
