//##################################################################################################
//
//   Custom Visualization Core library
//   Copyright (C) 2014 Ceetron Solution AS
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

#include <QObject>
#include <map>
#include <string>
#include "cafFactory.h"

class QAction;
class QIcon;
class QString;

#define CAF_CMD_HEADER_INIT \
    public: \
    static const std::string& idNameStatic()

#define CAF_CMD_SOURCE_INIT(ClassName, CommandIdName)\
    const std::string& ClassName::idNameStatic() { static std::string id = CommandIdName; return id;} \
    CAF_FACTORY_REGISTER(caf::PdmCommandFeature, ClassName, std::string, ClassName::idNameStatic())


namespace caf 
{

class PdmExecuteCommand;

//==================================================================================================
/// 1. If a direct command with no UI is requested, create an ExecuteCommand object and execute command
/// 2. If UI is required, create a CommandUI object and display to user. When user accepts data input,
///    create an ExecuteCommand object and fill in the data provided by the user
/// 3. If a user process is required, create start an CommandUserProcess. When the process is complete,
///    create an ExecuteCommand object and fill in the data provided by the user
//==================================================================================================
class PdmCommandFeature : public QObject
{
    Q_OBJECT
public:
    PdmCommandFeature() {}
    virtual ~PdmCommandFeature() {}

    QAction*            action();
    QAction*            action(QString customText);
    void                refreshEnabledState();

public slots:
    void actionTriggered(bool isChecked) { this->onActionTriggered(isChecked); }

protected:
    virtual void onActionTriggered(bool isChecked) = 0;
    virtual void setupActionLook(QAction* actionToSetup) =  0;
    virtual bool isCommandEnabled() = 0;

private:
    std::map<QString, QAction*> m_customTextToActionMap;
};



} // end namespace caf
