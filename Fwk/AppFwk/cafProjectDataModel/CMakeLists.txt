cmake_minimum_required (VERSION 2.8)

project (cafProjectDataModel)

# Qt
find_package ( Qt4 COMPONENTS QtCore QtGui )
include (${QT_USE_FILE})


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
