//##################################################################################################
//
//   Custom Visualization Core library
//   Copyright (C) 2011-2013 Ceetron AS
//
//   This library may be used under the terms of either the GNU General Public License or
//   the GNU Lesser General Public License as follows:
//
//   GNU General Public License Usage
//   This library is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This library is distributed in the hope that it will be useful, but WITHOUT ANY
//   WARRANTY; without even the implied warranty of MERCHANTABILITY or
//   FITNESS FOR A PARTICULAR PURPOSE.
//
//   See the GNU General Public License at <<http://www.gnu.org/licenses/gpl.html>>
//   for more details.
//
//   GNU Lesser General Public License Usage
//   This library is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Lesser General Public License as published by
//   the Free Software Foundation; either version 2.1 of the License, or
//   (at your option) any later version.
//
//   This library is distributed in the hope that it will be useful, but WITHOUT ANY
//   WARRANTY; without even the implied warranty of MERCHANTABILITY or
//   FITNESS FOR A PARTICULAR PURPOSE.
//
//   See the GNU Lesser General Public License at <<http://www.gnu.org/licenses/lgpl-2.1.html>>
//   for more details.
//
//##################################################################################################


#include "cafCmdAddItemExec.h"
#include "cafCmdAddItemExecData.h"
#include "cafReferenceHelper.h"
#include "cafSelectionManager.h"



namespace caf
{

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QString CmdAddItemExec::name()
{
    PdmFieldHandle* field = ReferenceHelper::instance()->fieldFromReference(m_commandData->m_pathToField);

    QString containedObjectType = "object";

    PdmPointersFieldHandle* listField = dynamic_cast<PdmPointersFieldHandle*>(field);
    if (listField)
    {
        containedObjectType = listField->containedClassKeyword();
    }

    return QString("Create new '%1'").arg(containedObjectType);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void CmdAddItemExec::redo()
{
    PdmFieldHandle* field = ReferenceHelper::instance()->fieldFromReference(m_commandData->m_pathToField);

    PdmPointersFieldHandle* listField = dynamic_cast<PdmPointersFieldHandle*>(field);
    if (listField)
    {
        caf::PdmObject* obj = listField->createAppendObject(m_commandData->m_indexAfter);

        if (m_commandData->m_indexAfter == -1)
        {
            m_commandData->m_createdItemIndex = static_cast<int>(listField->size() - 1);
        }
        else
        {
            m_commandData->m_createdItemIndex = m_commandData->m_indexAfter;
        }

        SelectionManager::instance()->setSelectedItem(obj, SelectionManager::CREATED_BY_COMMAND);

        listField->updateConnectedEditors();

        if (listField->ownerObject())
        {
            listField->ownerObject()->fieldChangedByUi(listField, QVariant(), QVariant());
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void CmdAddItemExec::undo()
{
    PdmFieldHandle* field = ReferenceHelper::instance()->fieldFromReference(m_commandData->m_pathToField);

    PdmPointersFieldHandle* listField = dynamic_cast<PdmPointersFieldHandle*>(field);
    if (listField)
    {
        listField->erase(m_commandData->m_createdItemIndex);
        listField->updateConnectedEditors();

        if (listField->ownerObject())
        {
            listField->ownerObject()->fieldChangedByUi(listField, QVariant(), QVariant());
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
CmdAddItemExec::CmdAddItemExec()
{
    m_commandData = new CmdAddItemExecData;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
CmdAddItemExecData* CmdAddItemExec::commandData()
{
    return m_commandData;
}

} // end namespace caf
