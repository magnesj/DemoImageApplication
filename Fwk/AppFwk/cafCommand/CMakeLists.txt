cmake_minimum_required (VERSION 2.8)


project (cafCommand)

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

include_directories(
    ${cafProjectDataModel_SOURCE_DIR}
)

# These headers need to go through Qt's MOC compiler
set( QOBJECT_HEADERS    
    cafCommandFeature.h
    cafCommandFeaturesCore.h
	cafCommandFeatureManager.h
)

if (CEE_BUILD_QT5)
  qt5_wrap_cpp(MOC_FILES_CPP ${QOBJECT_HEADERS} )
else()
  qt4_wrap_cpp(MOC_FILES_CPP ${QOBJECT_HEADERS} )
endif()


add_library( ${PROJECT_NAME}
    cafAppExecCommandManager.cpp
    cafAppExecCommandManager.h

    cafCmdAddItemExec.cpp
    cafCmdAddItemExec.h
    cafCmdAddItemExecData.cpp
    cafCmdAddItemExecData.h
    cafCmdAddItemFeature.cpp
    cafCmdAddItemFeature.h
    cafCmdDeleteItemExec.cpp
    cafCmdDeleteItemExec.h
    cafCmdDeleteItemExecData.cpp
    cafCmdDeleteItemExecData.h
    cafCmdDeleteItemFeature.cpp
    cafCmdDeleteItemFeature.h
    cafCmdFieldChangeExec.cpp
    cafCmdFieldChangeExec.h
    cafCmdSelectionChangeExec.cpp
    cafCmdSelectionChangeExec.h

    cafCommandFeature.cpp
    cafCommandFeature.h
    cafCommandFeaturesCore.cpp
    cafCommandFeaturesCore.h
    cafCommandFeatureManager.cpp
    cafCommandFeatureManager.h
    cafExecuteCommand.h
    cafSelectionManager.cpp
    cafSelectionManager.h

    ${MOC_FILES_CPP}
)
