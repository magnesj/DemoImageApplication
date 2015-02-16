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


class MimDesignCase : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;
public:
    MimDesignCase(void);
    virtual ~MimDesignCase(void);

    // Fields
    caf::PdmField<QString> name;
    
    // 
    virtual caf::PdmFieldHandle* userDescriptionField();

    // 
    void updateModelAndRedraw();
    
private:
};
