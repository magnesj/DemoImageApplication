cmake_minimum_required (VERSION 2.8)

include (CheckCSourceCompiles)
project (MinimalApplication_Master)



################################################################################
# Setup the main platform defines
################################################################################
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_definitions(-DCVF_LINUX)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-DCVF_OSX)
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS 		"-DCVF_LINUX -pipe -Wextra -Woverloaded-virtual -Wformat")
    set(CMAKE_CXX_FLAGS_DEBUG 	"-ggdb -g3 -O0 -DDEBUG -D_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNO_DEBUG")
endif() 


option(CEE_BUILD_QT5 "Build application using Qt5 (if disabled, Qt4 is used)" ON)



################################################################################
# Application Framework
################################################################################

add_subdirectory(Fwk/AppFwk/cafProjectDataModel)
#add_subdirectory(Fwk/AppFwk/CommonCode)
#add_subdirectory(Fwk/AppFwk/cafAnimControl)
#add_subdirectory(Fwk/AppFwk/cafViewer)
add_subdirectory(Fwk/AppFwk/cafCommand)
add_subdirectory(Fwk/AppFwk/cafUserInterface)
#add_subdirectory(Fwk/AppFwk/cafPdmCvf)

include_directories(
    ${cafUserInterface_SOURCE_DIR}
    ${cafProjectDataModel_SOURCE_DIR}
    ${cafPdmCvf_SOURCE_DIR}
    ${CommonCode_SOURCE_DIR}
	${cafAnimControl_SOURCE_DIR}
	${cafViewer_SOURCE_DIR}
)


################################################################################
# Application 
################################################################################
add_subdirectory(ApplicationCode)

