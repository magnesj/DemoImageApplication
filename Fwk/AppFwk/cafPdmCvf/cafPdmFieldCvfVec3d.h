//##################################################################################################
//
//   Custom Visualization Core library
//   Copyright (C) 2014 Ceetron Solutions AS
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


#pragma once

#include "cvfAssert.h"
#include "cvfBase.h"
#include "cvfVector3.h"

#include "cafPdmFieldImpl.h"

#include <QVariant>
#include <QTextStream>


//==================================================================================================
/// QTextStream Stream operators
//==================================================================================================

void operator >> (QTextStream& str, cvf::Vec3d& value);
void operator << (QTextStream& str, const cvf::Vec3d& value);

namespace caf 
{

// Forward declarations
template <typename T> class PdmField;


//==================================================================================================
/// Partial specialization for PdmField< cvf::Vec3d >
//==================================================================================================

template <>
class PdmFieldTypeSpecialization < cvf::Vec3d >
{
public:
    /// Convert the field value into a QVariant
    static QVariant convert(const cvf::Vec3d& value)
    {
        QString str;

        QTextStream textStream(&str);
        textStream << value;

        return QVariant(str);
    }

    /// Set the field value from a QVariant
    static void setFromVariant(const QVariant& variantValue, cvf::Vec3d& value)
    {
        QString str = variantValue.toString();
        
        QTextStream textStream(&str);

        textStream >> value;
    }

    /// Methods to get a list of options for a field
    static QList<PdmOptionItemInfo> valueOptions( bool* useOptionsOnly, const cvf::Vec3d& )
    {
        return QList<PdmOptionItemInfo>();
    }

    /// Methods to retrieve the possible PdmObject pointed to by a field
    static void childObjects(const PdmField< cvf::Vec3d >& field, std::vector<PdmObject*>* objects)
    { }

};

}

