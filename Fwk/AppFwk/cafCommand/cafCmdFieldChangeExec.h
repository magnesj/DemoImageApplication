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

#include "cafExecuteCommand.h"
#include "cafPdmField.h"

namespace caf 
{

class PdmPointersFieldHandle;

//==================================================================================================
/// 
//==================================================================================================
class CmdFieldChangeExecData : public PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    CmdFieldChangeExecData()
    {
        CAF_PDM_InitObject("CmdFieldChangeExecData uiName", "", "CmdFieldChangeExecData tooltip", "CmdFieldChangeExecData whatsthis");

        CAF_PDM_InitField(&m_pathToField,           "PathToField",      QString(), "PathToField",   "", "PathToField tooltip",      "PathToField whatsthis");
        CAF_PDM_InitField(&m_previousValueAsString, "previousValue",    QString(), "previousValue", "", "previousValue tooltip",    "previousValue whatsthis");
        CAF_PDM_InitField(&m_newValueAsString,      "newValue",         QString(), "newValue",      "", "newValue tooltip",         "newValue whatsthis");
    }

    PdmField<QString>  m_pathToField;
    PdmField<QString>  m_previousValueAsString;
    PdmField<QString>  m_newValueAsString;

    QVariant           m_newUiValue;
};


//==================================================================================================
/// 
//==================================================================================================
class CmdFieldChangeExec : public PdmExecuteCommand
{
public:
    CmdFieldChangeExec();

    CmdFieldChangeExecData* commandData();

    virtual QString name();
    virtual void redo();
    virtual void undo();

private:
    CmdFieldChangeExecData* m_commandData;
};



} // end namespace caf
