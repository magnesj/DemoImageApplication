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


#include "cafPdmField.h"
#include "cafPdmObject.h"

#include <iostream>

namespace caf
{

//--------------------------------------------------------------------------------------------------
///  PdmFieldHandle implementations
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void PdmFieldIOHelper::skipCharactersAndComments(QXmlStreamReader& xmlStream)
{
    QXmlStreamReader::TokenType type;
    while (!xmlStream.atEnd() && xmlStream.isCharacters() || xmlStream.isComment())
    {
        type = xmlStream.readNext();
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void PdmFieldIOHelper::skipComments(QXmlStreamReader& xmlStream)
{
    QXmlStreamReader::TokenType type;
    while (!xmlStream.atEnd() &&  xmlStream.isComment()) 
    {
         type = xmlStream.readNext();
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool PdmFieldHandle::assertValid() const
{
    if (m_keyword == "UNDEFINED")
    {
        std::cout << "PdmField: Detected use of non-initialized field. Did you forget to do CAF_PDM_InitField() on this field ?\n";
        return false;
    }

    if (!PdmObject::isValidXmlElementName(m_keyword))
    {
        std::cout << "PdmField: The supplied keyword: \"" << m_keyword.toStdString() << "\" is an invalid XML element name, and will break your file format!\n";
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void PdmFieldHandle::setKeyword(const QString& keyword)
{
    assert(PdmObject::isValidXmlElementName(keyword));

    m_keyword = keyword;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool PdmFieldHandle::hasChildObjects()
{
    std::vector<PdmObject*> children; 
    this->childObjects(&children); 
    return (children.size() > 0);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool PdmPointersFieldHandle::hasSameFieldCountForAllObjects()
{
    std::vector<PdmObject*> listObjects;
    this->childObjects(&listObjects);

    if (listObjects.size() == 0)
    {
        return true;
    }

    size_t fieldCount = 0;
    for (size_t i = 0; i < listObjects.size(); i++)
    {
        std::vector<PdmFieldHandle*> fields;
        listObjects[i]->fields(fields);

        if (i == 0)
        {
            fieldCount = fields.size();
        }
        else if (fieldCount != fields.size())
        {
            return false;
        }
    }

    return true;
}

//--------------------------------------------------------------------------------------------------
///  PdmObjectFactory implementations
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
PdmObject * PdmObjectFactory::create(const QString& classNameKeyword)
{
    std::map<QString, PdmObjectCreatorBase*>::iterator entryIt;
    entryIt = m_factoryMap.find(classNameKeyword);
    if (entryIt != m_factoryMap.end())
    {
        return entryIt->second->create();
    }
    else
    {
        return NULL;
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
PdmObjectFactory * PdmObjectFactory::instance()
{
    static PdmObjectFactory* fact = new PdmObjectFactory;
    return fact;
}

//--------------------------------------------------------------------------------------------------
/// Specialized read function for QStrings, because the >> operator only can read word by word
//--------------------------------------------------------------------------------------------------
template<>
void PdmFieldReader<QString>::readFieldData(PdmField<QString> & field, QXmlStreamReader& xmlStream)
{
    PdmFieldIOHelper::skipComments(xmlStream);
    if (!xmlStream.isCharacters()) return;

    field = xmlStream.text().toString();

    // Make stream point to end of element
    QXmlStreamReader::TokenType type;
    type = xmlStream.readNext();
    PdmFieldIOHelper::skipCharactersAndComments(xmlStream);
}


} //End of namespace caf

//--------------------------------------------------------------------------------------------------
/// Specialized read operation for Bool`s
//--------------------------------------------------------------------------------------------------
QTextStream& operator >> (QTextStream& str, bool& value)
{
    QString text;
    str >> text;
    if (text == "True" || text == "true" || text == "1" || text == "Yes" || text == "yes") value = true;
    else value = false;

    return str;
}

QTextStream& operator << (QTextStream& str, const bool& value)
{
    if (value) str << "True ";
    else str << "False ";

    return str;
}


//--------------------------------------------------------------------------------------------------
/// Specialized read operation for QDateTimes`s
//--------------------------------------------------------------------------------------------------
#include <QDateTime>
QTextStream&  operator >> (QTextStream& str, QDateTime& value)
{
    QString text;
    str >> text;
    value = QDateTime::fromString(text, "yyyy_MM_dd-HH:mm:ss");
    return str;
}

QTextStream&  operator << (QTextStream& str, const QDateTime& value)
{
    QString text = value.toString("yyyy_MM_dd-HH:mm:ss");
    str << text;
    return str;
}
