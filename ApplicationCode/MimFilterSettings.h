/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2014 Ceetron Solutions AS
// 
//  <APPLICATION_NAME> is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
// 
//  <APPLICATION_NAME> is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
// 
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#pragma once



#include "cafPdmObject.h"
#include "cafPdmField.h"
#include "cafAppEnum.h"


class MimFilterSettings : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;
public:

    enum FilterType
    {
        FILTER_SMALL, FILTER_MEDIUM, FILTER_LARGE, FILTER_CUSTOM
    };

    MimFilterSettings(void);
    virtual ~MimFilterSettings(void);

    // Fields
    caf::PdmField<QString> name;
    
    caf::PdmField<QString> imageToManipulate;
    
    caf::PdmField< caf::AppEnum<FilterType> > filterType;

    caf::PdmField<int> nx;
    caf::PdmField<int> ny;
    caf::PdmField<int> nz;

    caf::PdmField<bool> apply;


    
    // 
    virtual caf::PdmFieldHandle* userDescriptionField();

    virtual void fieldChangedByUi(const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue);

    virtual void defineEditorAttribute(const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute * attribute);
    virtual void defineUiOrdering(QString uiConfigName, caf::PdmUiOrdering& uiOrdering);

    virtual QList<caf::PdmOptionItemInfo> calculateValueOptions(const caf::PdmFieldHandle* fieldNeedingOptions, bool * useOptionsOnly);

private:
    void applyFilter();

};
