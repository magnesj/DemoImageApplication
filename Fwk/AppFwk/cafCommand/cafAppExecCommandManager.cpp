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


#include "cafAppExecCommandManager.h"

#include <QUndoCommand>
#include "cafExecuteCommand.h"


//--------------------------------------------------------------------------------------------------
/// Classed used to take over ownership of an execute command and wrap it in a QUndoCommand
//--------------------------------------------------------------------------------------------------
class UndoRedoWrapper : public QUndoCommand
{
public:
    UndoRedoWrapper(caf::PdmExecuteCommand* executeCommand)
    {
        m_executeCommand = executeCommand;

        setText(m_executeCommand->name());
    }

    ~UndoRedoWrapper()
    {
        delete m_executeCommand;
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    virtual void undo()
    {
        m_executeCommand->undo();
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    virtual void redo()
    {
        m_executeCommand->redo();
    }


private:
    caf::PdmExecuteCommand* m_executeCommand;
};


namespace caf
{

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
AppExecCommandManager::AppExecCommandManager()
{
    m_useUndoStack = false;
    m_undoStack = new QUndoStack();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
AppExecCommandManager* AppExecCommandManager::instance()
{
    static AppExecCommandManager* singleton = new AppExecCommandManager;
    return singleton;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void AppExecCommandManager::enableUndoStack(bool enable)
{
    m_useUndoStack = enable;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QUndoStack* AppExecCommandManager::undoStack()
{
    return m_undoStack;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void AppExecCommandManager::processExecuteCommand(PdmExecuteCommand* executeCommand)
{
    if (m_useUndoStack)
    {
        // Transfer ownership of execute command to wrapper object
        UndoRedoWrapper* undoRedoWrapper = new UndoRedoWrapper(executeCommand);

        m_undoStack->push(undoRedoWrapper);
    }
    else
    {
        // Execute command and delete the execute command
        
        executeCommand->redo();
        delete executeCommand;
    }
}


} // end namespace caf
