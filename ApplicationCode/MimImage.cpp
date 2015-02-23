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

#include "MimImage.h"
#include "MiuMainWindow.h"

#include "cafPdmUiFilePathEditor.h"

#include <QFile>

CAF_PDM_SOURCE_INIT(MimImage, "MimImage");


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
MimImage::MimImage(void)
{
    CAF_PDM_InitObject("MimImage", ":/Folder.png", "", "");
    CAF_PDM_InitField(&name, "name", QString("Case"), "Name", "", "", "");
    CAF_PDM_InitField(&imageFileName, "imageFileName", QString("ImageFilename"), "Image Filename", "", "", "");
    imageFileName.setUiEditorTypeName(caf::PdmUiFilePathEditor::uiEditorTypeName());
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
MimImage::~MimImage(void)
{
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
caf::PdmFieldHandle* MimImage::userDescriptionField()
{
    return &name;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MimImage::initAfterRead()
{
    readImageFromFile();
    redrawImageInDisplayWidget();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MimImage::fieldChangedByUi(const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue)
{
    if (changedField == &imageFileName)
    {
        readImageFromFile();
        redrawImageInDisplayWidget();
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MimImage::readImageFromFile()
{
    m_image = QImage(imageFileName);
    m_originalImage = m_image;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MimImage::redrawImageInDisplayWidget()
{
    MiuMainWindow::instance()->setImage(m_image);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QImage& MimImage::image()
{
    return m_image;
}

void MimImage::restoreOriginalImage()
{
    m_image = m_originalImage;
}

