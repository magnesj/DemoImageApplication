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


#include "cafReferenceHelper.h"
#include "cafPdmField.h"



namespace caf
{

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
ReferenceHelper* ReferenceHelper::instance()
{
    static ReferenceHelper* singleton = new ReferenceHelper;
    return singleton;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void ReferenceHelper::setRoot(PdmObject* root)
{
    m_root = root;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QString ReferenceHelper::referenceFromRootToObject(PdmObject* obj)
{
    QStringList objectNames = referenceFromRootToObjectAsStringList(obj);
    
    QString completeReference = objectNames.join(" ");
    return completeReference;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QString ReferenceHelper::referenceFromRootToField(PdmFieldHandle* field)
{
    if (field == NULL || m_root.isNull()) return QString();

    PdmObject* owner = field->ownerObject();
    if (!owner) return QString();

    QStringList refFromRootToField;

    refFromRootToField = referenceFromRootToObjectAsStringList(owner);

    refFromRootToField.push_front(field->keyword());

    QString completeReference = refFromRootToField.join(" ");
    return completeReference;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
PdmObject* ReferenceHelper::objectFromReference(const QString& reference)
{
    QStringList decodedReference = reference.split(" ");

    return objectFromReferenceStringList(decodedReference);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
PdmFieldHandle* ReferenceHelper::findField(PdmObject* object, const QString& fieldKeyword)
{
    if (object == NULL) return NULL;

    std::vector<PdmFieldHandle*> fields;
    object->fields(fields);

    for (size_t i = 0; i < fields.size(); i++)
    {
        if (fields[i]->keyword() == fieldKeyword)
        {
            return fields[i];
        }
    }

    return NULL;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QStringList ReferenceHelper::referenceFromRootToObjectAsStringList(PdmObject* obj)
{
    QStringList objectNames;
    
    if (obj != NULL && !m_root.isNull())
    {
        if (obj == m_root) return objectNames;

        PdmObject* currentObject = obj;

        bool continueParsing = true;
        while (continueParsing)
        {
            std::vector<caf::PdmFieldHandle*> parentFields;
            currentObject->parentFields(parentFields);

            assert(parentFields.size() == 1);

            std::vector<PdmObject*> childObjects;
            parentFields[0]->childObjects(&childObjects);

            if (childObjects.size() > 0)
            {
                int index = -1;

                for (size_t i = 0; i < childObjects.size(); i++)
                {
                    if (childObjects[i] == currentObject)
                    {
                        index = static_cast<int>(i);
                    }
                }

                objectNames.push_front(QString::number(index));
                objectNames.push_front(parentFields[0]->keyword());
            }
            else
            {
                continueParsing = false;
                continue;
            }

            std::vector<PdmObject*> parentObjects;
            currentObject->parentObjects(parentObjects);
            assert(parentObjects.size() == 1);

            PdmObject* firstParent = parentObjects[0];
            assert(firstParent);

            if (firstParent == m_root)
            {
                continueParsing = false;
                continue;
            }

            if (parentObjects.size() == 0)
            {
                continueParsing = false;
            }

            currentObject = firstParent;
        }
    }

    return objectNames;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
PdmFieldHandle* ReferenceHelper::fieldFromReference(const QString& reference)
{
    QStringList decodedReference = reference.split(" ");

    QString fieldKeyword = decodedReference[0];
    decodedReference.pop_front();

    PdmObject* parentObject = objectFromReferenceStringList(decodedReference);
    return findField(parentObject, fieldKeyword);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
PdmObject* ReferenceHelper::objectFromReferenceStringList(const QStringList& reference)
{
    PdmObject* currentObject = m_root;

    int i = 0;
    while (i < reference.size())
    {
        QString fieldKeyword = reference.at(i++);

        PdmFieldHandle* field = findField(currentObject, fieldKeyword);
        if (field)
        {
            std::vector<PdmObject*> childObjects;
            field->childObjects(&childObjects);

            if (childObjects.size() > 0)
            {
                QString fieldIndex = reference.at(i++);
                bool conversionOk = true;
                int index = fieldIndex.toInt(&conversionOk);
                if (!conversionOk)
                {
                    index = -1;
                }

                if (index > -1)
                {
                    PdmObject* listObject = childObjects[index];
                    currentObject = listObject;
                }
                else
                {
                    return NULL;
                }
            }
            else
            {
                return NULL;
            }
        }
    }

    return currentObject;
}

} // end namespace caf
