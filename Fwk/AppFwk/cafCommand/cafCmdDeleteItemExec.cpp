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


#include "cafCmdDeleteItemExec.h"
#include "cafCmdDeleteItemExecData.h"
#include "cafReferenceHelper.h"
#include "cafPdmDocument.h"



namespace caf
{

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QString CmdDeleteItemExec::name()
{
    return m_commandData->classKeyword();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void CmdDeleteItemExec::redo()
{
    PdmFieldHandle* field = ReferenceHelper::instance()->fieldFromReference(m_commandData->m_pathToField);

    PdmPointersFieldHandle* listField = dynamic_cast<PdmPointersFieldHandle*>(field);
    if (listField)
    {
        if (m_commandData->m_deletedObjectAsXml().isEmpty())
        {
            std::vector<PdmObject*> children;
            listField->childObjects(&children);

            PdmObject* obj = children[m_commandData->m_indexToObject];

            QString encodedXml;
            {
                // Write original objects to XML file
                PdmObjectGroup typedObjectGroup;
                typedObjectGroup.addObject(obj);

                QXmlStreamWriter xmlStream(&(m_commandData->m_deletedObjectAsXml.v()));
                xmlStream.setAutoFormatting(true);

                typedObjectGroup.writeFields(xmlStream);
            }
        }

        listField->erase(m_commandData->m_indexToObject);

        listField->ownerObject()->fieldChangedByUi(field, QVariant(), QVariant());
        listField->updateConnectedEditors();
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void CmdDeleteItemExec::undo()
{
    PdmFieldHandle* field = ReferenceHelper::instance()->fieldFromReference(m_commandData->m_pathToField);

    PdmPointersFieldHandle* listField = dynamic_cast<PdmPointersFieldHandle*>(field);
    if (listField)
    {
        PdmObjectGroup destinationObjectGroup;
        QXmlStreamReader xmlStream(m_commandData->m_deletedObjectAsXml());
        destinationObjectGroup.readFields(xmlStream);

        PdmObject* obj = destinationObjectGroup.objects[0];

        listField->insertAt(m_commandData->m_indexToObject, obj);

        listField->ownerObject()->fieldChangedByUi(field, QVariant(), QVariant());
        listField->updateConnectedEditors();
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
CmdDeleteItemExec::CmdDeleteItemExec()
{
    m_commandData = new CmdDeleteItemExecData;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
CmdDeleteItemExecData* CmdDeleteItemExec::commandData()
{
    return m_commandData;
}

} // end namespace caf
