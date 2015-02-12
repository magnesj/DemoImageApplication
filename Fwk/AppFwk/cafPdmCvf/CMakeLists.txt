cmake_minimum_required (VERSION 2.8)

project (cafPdmCvf)

# Qt
find_package ( Qt4 COMPONENTS QtCore QtGui QtMain )
include (${QT_USE_FILE})

include_directories(
    ${LibCore_SOURCE_DIR}
    ${cafProjectDataModel_SOURCE_DIR}
    ${cafUserInterface_SOURCE_DIR}
)


add_library( ${PROJECT_NAME}
	cafPdmFieldCvfColor.cpp
	cafPdmFieldCvfColor.h
	cafPdmFieldCvfMat4d.cpp
	cafPdmFieldCvfMat4d.h
	cafPdmFieldCvfVec3d.cpp
	cafPdmFieldCvfVec3d.h
)
