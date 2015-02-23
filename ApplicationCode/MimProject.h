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

#include "cafPdmDocument.h"

class MimImage;
class MimFilterSettings;

//==================================================================================================
///  
///  
//==================================================================================================
class MimProject : public caf::PdmDocument
{
     CAF_PDM_HEADER_INIT;

public:
    MimProject(void);
    virtual ~MimProject(void);

    caf::PdmPointersField<MimImage*> images;
    caf::PdmPointersField<MimFilterSettings*> filterSettings;
};
