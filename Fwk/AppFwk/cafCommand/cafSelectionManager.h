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


#pragma once

#include <vector>
#include "cafPdmPointer.h"

#include <QString>

namespace caf 
{

class PdmUiItem;


//==================================================================================================
/// 
//==================================================================================================
class SelectionManager
{
public:
    enum SelectionRole
    {
        APPLICATION_GLOBAL,
        CURRENT,
        CREATED_BY_COMMAND,
        UNDEFINED
    };

public:
    static SelectionManager* instance();

    PdmUiItem*  selectedItem(int role = SelectionManager::APPLICATION_GLOBAL);
    void        setSelectedItem(PdmUiItem* item, int role = SelectionManager::APPLICATION_GLOBAL);

    void        selectedItems(std::vector<PdmUiItem*>& items, int role = SelectionManager::APPLICATION_GLOBAL);
    void        setSelectedItems(const std::vector<PdmUiItem*>& items, int role = SelectionManager::APPLICATION_GLOBAL);

    void        selectionAsReferences(std::vector<QString>& referenceList, int role) const;
    void        setSelectionFromReferences(const std::vector<QString>& referenceList, int role);

    void        clear();

private:
    SelectionManager();

private:
    std::vector < std::vector<PdmUiItem*> > m_selectionForRole;
};



} // end namespace caf
