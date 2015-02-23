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


class MimImage : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;
public:
    MimImage(void);
    virtual ~MimImage(void);

    // Fields
    caf::PdmField<QString> name;
    caf::PdmField<QString> imageFileName;

    // Public interface
    QImage& image();
    void    updateDisplayImage();
    void    restoreOriginalImage();

    // Application Framework
    virtual caf::PdmFieldHandle* userDescriptionField();
    virtual void fieldChangedByUi(const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue);

protected:

    // Application Framework
    virtual void initAfterRead();

private:
    void readImageFromFile();

private:
    QImage m_image;
    QImage m_originalImage;
};
