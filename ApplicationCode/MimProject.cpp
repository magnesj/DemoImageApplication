/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2014 Ceetron Solutions AS
// 
//  DemoImageApplication is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
// 
//  DemoImageApplication is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
// 
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#include "MimProject.h"
#include "MimImage.h"
#include "MimFilterSettings.h"

CAF_PDM_SOURCE_INIT(MimProject, "DemoImageApplicationProject");
//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
MimProject::MimProject(void)
{
    CAF_PDM_InitObject("Project", "", "", "");

    CAF_PDM_InitFieldNoDefault(&images, "DesignCases", "DesignCases", "", "", "");
    CAF_PDM_InitFieldNoDefault(&filterSettings, "FilterSettings", "FilterSettings", "", "", "");

    images.push_back(new MimImage);
    filterSettings.push_back(new MimFilterSettings);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
MimProject::~MimProject(void)
{
    std::vector<caf::PdmFieldHandle*> fields;
    this->fields(fields);
    for (size_t i = 0; i < fields.size(); i++)
    {
        caf::PdmPointersFieldHandle* listField = dynamic_cast<caf::PdmPointersFieldHandle*>(fields[i]);
        if (listField)
        {
            listField->deleteAllChildObjects();
        }
    }
}
