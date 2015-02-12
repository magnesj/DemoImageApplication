cmake_minimum_required (VERSION 2.8)

project (ApplicationCode)

################################################################################
# Qt
################################################################################
set (QT_COMPONENTS_REQUIRED QtCore QtGui QtMain QtOpenGl QtNetwork QtScript QtScriptTools)
find_package (Qt4 COMPONENTS ${QT_COMPONENTS_REQUIRED})

include (${QT_USE_FILE})


include_directories(

    ${cafProjectDataModel_SOURCE_DIR}
	${cafCommand_SOURCE_DIR}
	${cafUserInterface_SOURCE_DIR}
)


set( APPLICATION_FILES 
    MiaMain.cpp
    MiaApplication.cpp
    
    MimProject.cpp
    MimDesignCase.cpp
)

set( USER_INTERFACE_FILES
    MiuMainWindow.cpp
)


list( APPEND CPP_SOURCES
     ${APPLICATION_FILES}
     ${USER_INTERFACE_FILES}
)


# Define files for MOC-ing
set ( QT_MOC_HEADERS
    MiaApplication.h
    MiuMainWindow.h
)

qt4_wrap_cpp( MOC_FILES_CPP ${QT_MOC_HEADERS} )

qt4_wrap_ui( FORM_FILES_CPP ${QT_UI_FILES} )



################################################################################
# Create source groups - see also included CMakeLists_files.cmake
################################################################################
source_group( "Application"         FILES ${APPLICATION_FILES} )
source_group( "UserInterface"       FILES ${USER_INTERFACE_FILES} )


# Default behaviour for a Qt application is a console application, resulting in a console window always being launced at startup
# The following statement is used to control this behaviour
#   set_target_properties( MY_TARGET PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
# This setting i not honored by Cmake - http://public.kitware.com/Bug/view.php?id=14326 
# We use the following workaround described in the bug report
#   ADD_EXECUTABLE(${TARGET_NAME} WIN32 ${SRC})
# See CMake symbol WIN32_EXECUTABLE for details

if (MSVC)
    set( EXE_FILES WIN32)
elseif (APPLE)
    set( EXE_FILES MACOSX_BUNDLE)
endif()

set( EXE_FILES
    ${EXE_FILES}
    ${CPP_SOURCES}
    ${MOC_FILES_CPP}
    ${FORM_FILES_CPP}
    ${QRC_FILES_CPP}
    ${WIN_RESOURCE}
    ${HEADER_FILES}
    ${REFERENCED_CMAKE_FILES}
)

add_executable( ApplicationCode ${EXE_FILES} )


set( LINK_LIBRARIES
    cafCommand
    cafUserInterface
    cafProjectDataModel

    ${QT_LIBRARIES}
)


target_link_libraries( ApplicationCode ${LINK_LIBRARIES} ${EXTERNAL_LINK_LIBRARIES})

# Copy Dlls
if (MSVC)

    # Qt DLLs
    set (QTLIBLIST QtCore QtCored QtGui QtGuid QtOpenGl QtOpenGld QtNetwork QtNetworkd QtScript QtScriptd QtScriptTools QtScriptToolsd)
    foreach (qtlib ${QTLIBLIST})
        add_custom_command(TARGET ResInsight POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT_BINARY_DIR}/${qtlib}4.dll"
                ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>)
    endforeach( qtlib )

endif(MSVC)

