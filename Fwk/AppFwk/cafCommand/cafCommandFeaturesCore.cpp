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


#include "cafCommandFeaturesCore.h"

#include "cafAppExecCommandManager.h"
#include "cafCmdAddItemFeature.h"
#include "cafCmdDeleteItemFeature.h"
#include "cafCmdFieldChangeExec.h"
#include "cafCmdSelectionChangeExec.h"
#include "cafCommandFeatureManager.h"
#include "cafPdmField.h"
#include "cafReferenceHelper.h"
#include "cafSelectionManager.h"

#include <QAction>


namespace caf
{


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
PdmCommandFeaturesCore* PdmCommandFeaturesCore::instance()
{
    static PdmCommandFeaturesCore* singleton = new PdmCommandFeaturesCore;

    return singleton;
}

//--------------------------------------------------------------------------------------------------
/// Field changed execution command is completely handled by this function. This is a framework function
/// only to be called from UI editors, and does not require actions to be displayed in GUI
//--------------------------------------------------------------------------------------------------
void PdmCommandFeaturesCore::fieldChangedCommand(PdmFieldHandle* field, const QVariant& newUiValue)
{
    QVariant currentUiValue = field->uiValue();
    if (currentUiValue == newUiValue) return;

    QString reference = ReferenceHelper::instance()->referenceFromRootToField(field);
    if (reference.isEmpty())
    {
        assert (false);
        return;
    }

    CmdFieldChangeExec* fieldChangeExec = new CmdFieldChangeExec;

    fieldChangeExec->commandData()->m_newUiValue = newUiValue;
    fieldChangeExec->commandData()->m_pathToField = reference;

    AppExecCommandManager* appExecCommandManager = AppExecCommandManager::instance();
    appExecCommandManager->processExecuteCommand(fieldChangeExec);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
PdmObject* PdmCommandFeaturesCore::addItemToField(PdmPointersFieldHandle* listField)
{
    SelectionManager::instance()->setSelectedItem(listField, SelectionManager::CURRENT);

    PdmCommandFeatureManager::instance()->action("PdmListField_AddItem")->trigger();

    PdmUiItem* createdItem = SelectionManager::instance()->selectedItem(SelectionManager::CREATED_BY_COMMAND);

    PdmObject* pdmObj = dynamic_cast<PdmObject*>(createdItem);
    return pdmObj;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void PdmCommandFeaturesCore::selectItem(PdmObject* selection, SelectionManager::SelectionRole role)
{
    QString itemRef = ReferenceHelper::instance()->referenceFromRootToObject(selection);

    CmdSelectionChangeExec* selectionChangeExec = new CmdSelectionChangeExec;

    selectionChangeExec->commandData()->m_newSelection.v().push_back(itemRef);
    selectionChangeExec->commandData()->m_selectionRole.v() = role;

    SelectionManager::instance()->selectionAsReferences(selectionChangeExec->commandData()->m_previousSelection.v(), role);

    AppExecCommandManager* appExecCommandManager = AppExecCommandManager::instance();
    appExecCommandManager->processExecuteCommand(selectionChangeExec);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
PdmPointersFieldHandle* PdmCommandFeaturesCore::findHandleFromSelection(std::vector<PdmUiItem*> items)
{
    PdmPointersFieldHandle* pointersFieldHandle = NULL;

    for (size_t i = 0; i < items.size(); i++)
    {
        if (dynamic_cast<PdmPointersFieldHandle*>(items[i]))
        {
            pointersFieldHandle = dynamic_cast<PdmPointersFieldHandle*>(items[i]);
        }

        PdmObject* pdmObject = dynamic_cast<caf::PdmObject*>(items[i]);
        if (pdmObject)
        {
            std::vector<PdmFieldHandle*> parentFields;
            pdmObject->parentFields(parentFields);

            if (parentFields.size() == 1)
            {
                PdmPointersFieldHandle* parentPointersField = dynamic_cast<PdmPointersFieldHandle*>(parentFields[0]);

                if (parentPointersField)
                {
                    pointersFieldHandle = parentPointersField;
                }
            }
        }
    }

    return pointersFieldHandle;
}



} // end namespace caf
