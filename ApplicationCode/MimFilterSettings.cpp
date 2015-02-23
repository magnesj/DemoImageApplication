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

#include "MimFilterSettings.h"

#include "MiuMainWindow.h"

#include "MimProject.h"
#include "MiaApplication.h"
#include "MimImage.h"

#include "cafPdmUiPushButtonEditor.h"
#include "cafPdmUiComboBoxEditor.h"
#include "cafPdmUiLineEditor.h"

#define _USE_MATH_DEFINES
#include <math.h>



template<>
void caf::AppEnum<MimFilterSettings::FilterType>::setUp()
{
    addItem(MimFilterSettings::FILTER_SMALL,    "SMALL",    "Small");
    addItem(MimFilterSettings::FILTER_MEDIUM,   "MEDIUM",   "Medium");
    addItem(MimFilterSettings::FILTER_LARGE,    "LARGE",    "Large");
    addItem(MimFilterSettings::FILTER_CUSTOM,   "CUSTOM",   "Custom");
    setDefault(MimFilterSettings::FILTER_SMALL);

}

CAF_PDM_SOURCE_INIT(MimFilterSettings, "MimFilterSettings");


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
MimFilterSettings::MimFilterSettings(void)
{
    CAF_PDM_InitObject("MimFilterSettings", "", "", "");

    CAF_PDM_InitField(&name, "name", QString("Filter"), "Name", "", "", "");
    
    CAF_PDM_InitFieldNoDefault(&imageToManipulate, "imageToManipulate", "Image to filter", "", "", "");
    imageToManipulate.setUiEditorTypeName(caf::PdmUiComboBoxEditor::uiEditorTypeName());

    CAF_PDM_InitField(&filterType, "FilterType", caf::AppEnum<FilterType>(FILTER_SMALL), "Filter Type", "", "", "");

    CAF_PDM_InitField(&nx, "NX", 1, "NX", "", "", "");
    CAF_PDM_InitField(&ny, "NY", 1, "NX", "", "", "");

    CAF_PDM_InitField(&applyButton, "Apply", false, "Apply", "", "", "");
    applyButton.setIOReadable(false);
    applyButton.setIOWritable(false);
    applyButton.setUiEditorTypeName(caf::PdmUiPushButtonEditor::uiEditorTypeName());
    applyButton.setUiLabelPosition(caf::PdmUiItemInfo::HIDDEN);

    CAF_PDM_InitField(&restoreButton, "Restore", false, "Restore", "", "", "");
    restoreButton.setIOReadable(false);
    restoreButton.setIOWritable(false);
    restoreButton.setUiEditorTypeName(caf::PdmUiPushButtonEditor::uiEditorTypeName());
    restoreButton.setUiLabelPosition(caf::PdmUiItemInfo::HIDDEN);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
MimFilterSettings::~MimFilterSettings(void)
{
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
caf::PdmFieldHandle* MimFilterSettings::userDescriptionField()
{
    return &name;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MimFilterSettings::fieldChangedByUi(const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue)
{
    if (changedField == &imageToManipulate)
    {
        MimImage* dc = findImageObjectByName(imageToManipulate);
        if (dc)
        {
            dc->redrawImageInDisplayWidget();
        }
    }
    else if (changedField == &applyButton)
    {
        applyFilter();
    }
    else if (changedField == &restoreButton)
    {
        MimImage* dc = findImageObjectByName(imageToManipulate);
        if (dc)
        {
            dc->restoreOriginalImage();
            dc->redrawImageInDisplayWidget();
        }
    }
    else if (changedField == &filterType)
    {
        if (filterType == FILTER_SMALL)
        {
            nx = 1;
            ny = 1;
        }
        else if (filterType == FILTER_MEDIUM)
        {
            nx = 2;
            ny = 2;
        }
        else if (filterType == FILTER_LARGE)
        {
            nx = 5;
            ny = 5;
        }

        updateConnectedEditors();
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MimFilterSettings::defineEditorAttribute(const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute * attribute)
{
    if (&applyButton == field)
    {
        caf::PdmUiPushButtonEditorAttribute* attrib = dynamic_cast<caf::PdmUiPushButtonEditorAttribute*> (attribute);
        attrib->m_buttonText = "Apply";
    }
    else if (&restoreButton == field)
    {
        caf::PdmUiPushButtonEditorAttribute* attrib = dynamic_cast<caf::PdmUiPushButtonEditorAttribute*> (attribute);
        attrib->m_buttonText = "Restore";
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MimFilterSettings::defineUiOrdering(QString uiConfigName, caf::PdmUiOrdering& uiOrdering)
{
    bool customFilter = this->filterType == FILTER_CUSTOM;
    
    nx.setUiReadOnly(!customFilter);
    ny.setUiReadOnly(!customFilter);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QList<caf::PdmOptionItemInfo> MimFilterSettings::calculateValueOptions(const caf::PdmFieldHandle* fieldNeedingOptions, bool * useOptionsOnly)
{
    if (fieldNeedingOptions == &imageToManipulate)
    {
        QList<caf::PdmOptionItemInfo> optionList;
        optionList.push_back(caf::PdmOptionItemInfo("Undefined", "Undefined"));

        MimProject* proj = MiaApplication::instance()->project();
        if (proj)
        {
            for (size_t i = 0; i < proj->images.size(); i++)
            {
                MimImage* mimImage = proj->images()[i];
                optionList.push_back(caf::PdmOptionItemInfo(mimImage->name(), mimImage->name()));
            }
        }

        return optionList;
    }

    return QList<caf::PdmOptionItemInfo>();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MimFilterSettings::applyFilter()
{
    MimImage* mimImage = findImageObjectByName(imageToManipulate);
    if (mimImage)
    {
        mimImage->restoreOriginalImage();
        QImage& img = mimImage->image();

        float weight = 1.0f;

        {
            std::vector<float> kernel = compute1dGaussianKernel(nx, weight);

            float blurR = 0.0f;
            float blurG = 0.0f;
            float blurB = 0.0f;

            for (int y = 0; y < img.height(); y++)
            {
                for (int x = 0; x < img.width(); x++)
                {
                    blurR = 0.0f;
                    blurG = 0.0f;
                    blurB = 0.0f;

                    for (int offset = 0; offset < kernel.size(); offset++)
                    {
                        int sx = qBound(0, img.width() - 1, (x - nx) + offset);
                        QRgb col = img.pixel(sx, y);

                        blurR += qRed(col)   * kernel[offset];
                        blurG += qGreen(col) * kernel[offset];
                        blurB += qBlue(col)  * kernel[offset];
                    }

                    img.setPixel(x, y, qRgb(blurR, blurG, blurB));
                }
            }
        }
                
        {
            std::vector<float> kernel = compute1dGaussianKernel(ny, weight);

            float blurR = 0.0f;
            float blurG = 0.0f;
            float blurB = 0.0f;

            for (int x = 0; x < img.width(); x++)
            {
                for (int y = 0; y < img.height(); y++)
                {
                    blurR = 0.0f;
                    blurG = 0.0f;
                    blurB = 0.0f;
                            
                    for (int offset = 0; offset < kernel.size(); offset++)
                    {
                        int sy = qBound(0, img.height() - 1, (y - ny) + offset);
                        QRgb col = img.pixel(x, sy);

                        blurR += qRed(col)   * kernel[offset];
                        blurG += qGreen(col) * kernel[offset];
                        blurB += qBlue(col)  * kernel[offset];
                    }

                    img.setPixel(x, y, qRgb(blurR, blurG, blurB));
                }
            }
        }

        mimImage->redrawImageInDisplayWidget();
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
std::vector<float> MimFilterSettings::compute1dGaussianKernel(int inRadius, float inWeight)
{
    std::vector<float> kernel;
    
    int mem_amount = (inRadius * 2) + 1;

    float twoRadiusSquaredRecip = 1.0 / (2.0 * inRadius * inRadius);
    float sqrtTwoPiTimesRadiusRecip = 1.0 / (sqrt(2.0 * M_PI) * inRadius);
    float radiusModifier = inWeight;

    // Create Gaussian Kernel
    int r = -inRadius;
    float sum = 0.0f;
    for (int i = 0; i < mem_amount; i++)
    {
        float x = r * radiusModifier;
        x *= x;
        float v = sqrtTwoPiTimesRadiusRecip * exp(-x * twoRadiusSquaredRecip);
        kernel.push_back(v);

        sum += v;
        r++;
    }

    // Normalize distribution
    float div = sum;
    for (int i = 0; i < mem_amount; i++)
        kernel[i] /= div;

    return kernel;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
MimImage* MimFilterSettings::findImageObjectByName(const QString& caseName) const
{
    MimProject* proj = MiaApplication::instance()->project();
    if (proj)
    {
        for (size_t i = 0; i < proj->images.size(); i++)
        {
            MimImage* mimImage = proj->images()[i];

            if (mimImage->name() == caseName)
            {
                return mimImage;
            }
        }
    }

    return NULL;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void MimFilterSettings::updateDisplayOfConnectedImage()
{
    MimImage* mimImage = findImageObjectByName(imageToManipulate);
    if (mimImage)
    {
        mimImage->redrawImageInDisplayWidget();
    }
}

