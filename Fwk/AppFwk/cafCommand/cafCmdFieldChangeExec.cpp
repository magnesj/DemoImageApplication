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


#include "cafCmdFieldChangeExec.h"
#include "cafReferenceHelper.h"


namespace caf
{

CAF_PDM_SOURCE_INIT(CmdFieldChangeExecData, "CmdFieldChangeExecData");


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QString CmdFieldChangeExec::name()
{
    PdmFieldHandle* field = ReferenceHelper::instance()->fieldFromReference(m_commandData->m_pathToField);
    if (field)
    {
        QString fieldText = QString("Change field '%1'").arg(field->uiName());
        if (field->ownerObject())
        {
            fieldText += QString(" in '%1'").arg(field->ownerObject()->uiName());
        }
        return fieldText;
    }
    else
    {
        return m_commandData->classKeyword();
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void CmdFieldChangeExec::redo()
{
    PdmFieldHandle* field = ReferenceHelper::instance()->fieldFromReference(m_commandData->m_pathToField);
    if (!field)
    {
        assert(false);
        return;
    }

    {
        QXmlStreamWriter xmlStream(&(m_commandData->m_previousValueAsString.v()));
        xmlStream.setAutoFormatting(true);
        xmlStream.writeStartElement("", "d");
        field->writeFieldData(xmlStream);
        xmlStream.writeEndElement();
    }

    // Transform the QVariant representing the new value to a text string using stream operators in Pdm
    // Do not directly call QVariant->toString(), as specialializations are handled internally by Pdm
    // Call the ususal setValueFromUi(), and transform the value into a QString using stream writer

    // This call will trigger the ownerObject->updateConnectedEditors
    field->setValueFromUi(m_commandData->m_newUiValue);

    if (m_commandData->m_newValueAsString().isEmpty())
    {
        QXmlStreamWriter xmlStream(&(m_commandData->m_newValueAsString.v()));
        xmlStream.setAutoFormatting(true);

        xmlStream.writeStartElement("", "d");
        field->writeFieldData(xmlStream);
        xmlStream.writeEndElement();
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void CmdFieldChangeExec::undo()
{
    PdmFieldHandle* field = ReferenceHelper::instance()->fieldFromReference(m_commandData->m_pathToField);
    if (!field)
    {
        assert(false);
        return;
    }
    
    QVariant oldValue = field->uiValue();

    QString streamString = m_commandData->m_previousValueAsString;
    QXmlStreamReader xmlStream(streamString);

    // See PdmObject::readFields and friends to match token count for reading field values
    // The stream is supposed to be pointing at the first token of field content when calling readFieldData()
    QXmlStreamReader::TokenType tt;
    int tokenCount = 3;
    for (int i = 0; i < tokenCount; i++)
    {
        tt = xmlStream.readNext();
    }
    field->readFieldData(xmlStream);

    QVariant newValue = field->uiValue();

    // readFieldData() will not notify about field change. Explicitly call updateConnectedEditors()
    field->ownerObject()->updateConnectedEditors();
    field->ownerObject()->fieldChangedByUi(field, oldValue, newValue);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
CmdFieldChangeExec::CmdFieldChangeExec()
{
    m_commandData = new CmdFieldChangeExecData;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
CmdFieldChangeExecData* CmdFieldChangeExec::commandData()
{
    return m_commandData;
}

} // end namespace caf
