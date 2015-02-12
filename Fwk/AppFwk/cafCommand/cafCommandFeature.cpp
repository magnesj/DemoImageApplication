//##################################################################################################
//
//   Custom Visualization Core library
//   Copyright (C) 2014 Ceetron Solutions AS
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

#include "cafCommandFeature.h"

#include "cafAppExecCommandManager.h"
#include "cafCommandFeatureManager.h"

#include <assert.h>

#include <QAction>


namespace caf
{


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QAction* PdmCommandFeature::action()
{
    return this->action(QString(""));
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QAction* PdmCommandFeature::action(QString customText)
{
    std::map<QString, QAction*>::iterator it;
    it = m_customTextToActionMap.find(customText);
    if (it != m_customTextToActionMap.end() && it->second != NULL)
    {
        return it->second;
    }
    else
    {
        QAction* action = new QAction(this);
        this->setupActionLook(action);
        if (!customText.isEmpty())
        {
            action->setText(customText);
        }
        connect(action, SIGNAL(triggered(bool)), SLOT(actionTriggered(bool)));
        m_customTextToActionMap[customText]= action;
        return action;
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void PdmCommandFeature::refreshEnabledState()
{
    std::map<QString, QAction*>::iterator it;
    bool isEnabled = this->isCommandEnabled();

    for (it = m_customTextToActionMap.begin(); it != m_customTextToActionMap.end(); ++it)
    {
        it->second->setEnabled(isEnabled);
    }
}

} // end namespace caf
