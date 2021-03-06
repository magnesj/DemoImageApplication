//##################################################################################################
//
//   Custom Visualization Core library
//   Copyright (C) Ceetron Solutions AS
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


#include "cafPdmUiTableView.h"
#include "cafPdmObject.h"
#include "cafPdmUiTableViewEditor.h"

#include <QHBoxLayout>


namespace caf
{


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
PdmUiTableView::PdmUiTableView(QWidget* parent, Qt::WindowFlags f)
    : QWidget (parent, f)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    setLayout(layout);

    m_listViewEditor = new PdmUiTableViewEditor();

    QWidget* widget = m_listViewEditor->createWidget(this);
    layout->addWidget(widget);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
PdmUiTableView::~PdmUiTableView()
{
    if (m_listViewEditor) delete m_listViewEditor;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void PdmUiTableView::setListField(PdmPointersFieldHandle* listField)
{
    assert(m_listViewEditor);

    m_listViewEditor->setListField(listField);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QTableView* PdmUiTableView::tableView()
{
    assert(m_listViewEditor);

    return m_listViewEditor->tableView();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void PdmUiTableView::enableDefaultContextMenu(bool enable)
{
    m_listViewEditor->enableDefaultContextMenu(enable);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void PdmUiTableView::setSelectionRole(SelectionManager::SelectionRole role)
{
    m_listViewEditor->setSelectionRole(role);
}


} //End of namespace caf

