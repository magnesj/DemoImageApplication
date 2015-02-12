cmake_minimum_required (VERSION 2.8)

# Qt
find_package ( Qt4 COMPONENTS QtCore QtGui QtMain )
include (${QT_USE_FILE})

project (cafCommand)

include_directories(
    ${cafProjectDataModel_SOURCE_DIR}
)

# These headers need to go through Qt's MOC compiler
set( QOBJECT_HEADERS    
    cafCommandFeature.h
    cafCommandFeaturesCore.h
	cafCommandFeatureManager.h
)

if ( (${CMAKE_VERSION} VERSION_LESS 2.8.6) OR (NOT CMAKE_AUTOMOC) )
    qt4_wrap_cpp( MOC_FILES_CPP ${QOBJECT_HEADERS} )
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
