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

#include "MimDesignCase.h"
#include "MiuMainWindow.h"

#include "cafPdmUiFilePathEditor.h"

#include <QFile>

CAF_PDM_SOURCE_INIT(MimDesignCase, "MimDesignCase");


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
MimDesignCase::MimDesignCase(void)
{
    CAF_PDM_InitObject("DesignCase", ":/Folder.png", "", "");
    CAF_PDM_InitField(&name, "name", QString("Case"), "Name", "", "", "");
    CAF_PDM_InitField(&filename, "filename", QString("Filename"), "Filename", "", "", "");
    filename.setUiEditorTypeName(caf::PdmUiFilePathEditor::uiEditorTypeName());

    CAF_PDM_InitField(&imageFileName, "imageFileName", QString("ImageFilename"), "Image Filename", "", "", "");
    imageFileName.setUiEditorTypeName(caf::PdmUiFilePathEditor::uiEditorTypeName());
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
MimDesignCase::~MimDesignCase(void)
{
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
caf::PdmFieldHandle* MimDesignCase::userDescriptionField()
{
    return &name;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MimDesignCase::initAfterRead()
{
    updateTextEditContent();

    readImageFromFile();
    updateDisplayImage();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MimDesignCase::setupBeforeSave()
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << MiuMainWindow::instance()->textEditorContent();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MimDesignCase::fieldChangedByUi(const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue)
{
    if (changedField == &filename)
    {
        updateTextEditContent();
    }

    if (changedField == &imageFileName)
    {
        readImageFromFile();
        updateDisplayImage();
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MimDesignCase::updateTextEditContent()
{
    QString textString;

    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        textString = in.readAll();
    }

    MiuMainWindow::instance()->setTextEditorContent(textString);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MimDesignCase::defineUiOrdering(QString uiConfigName, caf::PdmUiOrdering& uiOrdering)
{

}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MimDesignCase::readImageFromFile()
{
    m_image = QImage(imageFileName);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MimDesignCase::updateDisplayImage()
{
    MiuMainWindow::instance()->setImage(m_image);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QImage& MimDesignCase::image()
{
    return m_image;
}

