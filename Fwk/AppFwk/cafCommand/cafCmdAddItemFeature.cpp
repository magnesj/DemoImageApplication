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


#include "cafCmdAddItemFeature.h"

#include "cafCmdAddItemExec.h"
#include "cafCmdAddItemExecData.h"
#include "cafCommandFeaturesCore.h"
#include "cafCommandFeatureManager.h"
#include "cafPdmField.h"
#include "cafReferenceHelper.h"
#include "cafSelectionManager.h"

#include <QAction>
#include "cafAppExecCommandManager.h"



namespace caf
{

    CAF_CMD_SOURCE_INIT(CmdAddItemFeature, "PdmListField_AddItem");

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
PdmExecuteCommand* CmdAddItemFeature::createExecuteCommand()
{
    std::vector<PdmUiItem*> items;
    SelectionManager::instance()->selectedItems(items, SelectionManager::CURRENT);
    
    caf::PdmPointersFieldHandle* listField = PdmCommandFeaturesCore::findHandleFromSelection(items);
    if (!listField) return NULL;
    
    caf::PdmObject* currentPdmObject = NULL;
    for (size_t i = 0; i < items.size(); i++)
    {
        if (dynamic_cast<caf::PdmObject*>(items[i]))
        {
            currentPdmObject = dynamic_cast<caf::PdmObject*>(items[i]);
        }
    }

    int indexAfter = -1;
    if (currentPdmObject)
    {
        std::vector<PdmObject*> childObjects;
        listField->childObjects(&childObjects);

        for (size_t i = 0; i < childObjects.size(); i++)
        {
            if (childObjects[i] == currentPdmObject)
            {
                indexAfter = static_cast<int>(i);
            }
        }

        // Did not find currently selected pdm object in the current list field
        assert(indexAfter != -1);
    }

    CmdAddItemExec* addItemExec = new CmdAddItemExec;
        
    CmdAddItemExecData* data = addItemExec->commandData();
    data->m_pathToField = ReferenceHelper::instance()->referenceFromRootToField(listField);
    data->m_indexAfter = indexAfter;

    return addItemExec;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool CmdAddItemFeature::isCommandEnabled() 
{
    std::vector<PdmUiItem*> items;
    SelectionManager::instance()->selectedItems(items, SelectionManager::CURRENT);
    
    caf::PdmPointersFieldHandle* listField = PdmCommandFeaturesCore::findHandleFromSelection(items);

    if (listField)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void CmdAddItemFeature::onActionTriggered(bool isChecked)
{
    if (isCommandEnabled())
    {
        PdmExecuteCommand* exeCmd = createExecuteCommand();
        if (exeCmd)
        {
            AppExecCommandManager* appExecCommandManager = AppExecCommandManager::instance();
            appExecCommandManager->processExecuteCommand(exeCmd);
        }
        else
        {
            assert(0);
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void CmdAddItemFeature::setupActionLook(QAction* actionToSetup)
{
    actionToSetup->setText("Add new object");
}

} // end namespace caf
