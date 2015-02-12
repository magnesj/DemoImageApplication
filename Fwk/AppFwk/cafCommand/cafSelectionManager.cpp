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


#include "cafSelectionManager.h"
#include "cafPdmObject.h"
#include "cafReferenceHelper.h"



namespace caf
{

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
SelectionManager* SelectionManager::instance()
{
    static SelectionManager* singleton = new SelectionManager;
    return singleton;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void SelectionManager::selectedItems(std::vector<PdmUiItem*>& items, int role /*= SelectionManager::APPLICATION_GLOBAL*/)
{
    std::vector<PdmUiItem*>& selection = m_selectionForRole[role];

    for (size_t i = 0; i < selection.size(); i++)
    {
        if (selection[i])
        {
            items.push_back(selection[i]);
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void SelectionManager::setSelectedItems(const std::vector<PdmUiItem*>& items, int role /*= SelectionManager::APPLICATION_GLOBAL*/)
{
    std::vector<PdmUiItem*>& selection = m_selectionForRole[role];

    selection.clear();

    for (size_t i = 0; i < items.size(); i++)
    {
        selection.push_back(items[i]);
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
PdmUiItem* SelectionManager::selectedItem(int role /*= SelectionManager::APPLICATION_GLOBAL*/)
{
    std::vector<PdmUiItem*>& selection = m_selectionForRole[role];

    if (selection.size() == 1)
    {
        return selection[0];
    }

    return NULL;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void SelectionManager::setSelectedItem(PdmUiItem* item, int role /*= SelectionManager::APPLICATION_GLOBAL*/)
{
    std::vector<PdmUiItem*> singleSelection;
    singleSelection.push_back(item);

    setSelectedItems(singleSelection, role);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
SelectionManager::SelectionManager()
{
    m_selectionForRole.resize(UNDEFINED);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void SelectionManager::selectionAsReferences(std::vector<QString>& referenceList, int role) const
{
    const std::vector<PdmUiItem*>& selection = m_selectionForRole[role];

    for (size_t i = 0; i < selection.size(); i++)
    {
        PdmObject* pdmObj = dynamic_cast<PdmObject*>(selection[i]);
        if (pdmObj)
        {
            QString itemRef = ReferenceHelper::instance()->referenceFromRootToObject(pdmObj);

            referenceList.push_back(itemRef);
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void SelectionManager::setSelectionFromReferences(const std::vector<QString>& referenceList, int role)
{
    std::vector<PdmUiItem*> uiItems;

    for (int i = 0; i < referenceList.size(); i++)
    {
        QString reference = referenceList[i];

        PdmObject* pdmObj = ReferenceHelper::instance()->objectFromReference(reference);
        if (pdmObj)
        {
            uiItems.push_back(pdmObj);
        }
    }

    setSelectedItems(uiItems, role);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void SelectionManager::clear()
{
    for (int i = 0; i < m_selectionForRole.size(); i++)
    {
        m_selectionForRole[i].clear();
    }
}

} // end namespace caf
