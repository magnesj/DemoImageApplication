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


#include "cafPdmUiTableViewEditor.h"

#include "cafPdmDocument.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmUiCheckBoxDelegate.h"
#include "cafPdmUiEditorHandle.h"
#include "cafPdmUiTableViewDelegate.h"
#include "cafPdmUiTableViewModel.h"
#include "cafUiTreeModelPdm.h"

#include <QWidget>
#include <QGridLayout>
#include <QTableView>
#include <QLabel>
#include <QMenu>
#include "cafCommandFeatureManager.h"



namespace caf
{

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
PdmUiTableViewEditor::PdmUiTableViewEditor()
{
    m_layout = NULL;;
    m_tableView = NULL;
    m_tableModelPdm = NULL;
    m_pdmListField = NULL;
    m_delegate = NULL;

    m_useDefaultContextMenu = false;

    m_checkboxDelegate = new PdmUiCheckBoxDelegate(this);

    m_selectionRole = SelectionManager::UNDEFINED;
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
PdmUiTableViewEditor::~PdmUiTableViewEditor()
{
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QWidget* PdmUiTableViewEditor::createWidget(QWidget* parent)
{
    m_mainWidget = new QWidget(parent);
    m_layout     = new QVBoxLayout();
    m_mainWidget->setLayout(m_layout);

    m_tableModelPdm = new PdmUiTableViewModel(m_mainWidget);

    m_delegate = new PdmUiTableViewDelegate(this, m_tableModelPdm);

    m_tableView = new QTableView(m_mainWidget);
    m_tableView->setShowGrid(true);
    m_tableView->setModel(m_tableModelPdm);

    connect(m_tableView->selectionModel(), SIGNAL(selectionChanged( const QItemSelection & , const QItemSelection & )), SLOT(slotSelectionChanged( const QItemSelection & , const QItemSelection & )));
    connect(m_tableView->selectionModel(), SIGNAL(currentChanged( const QModelIndex & , const QModelIndex & )), SLOT(slotCurrentChanged( const QModelIndex& , const QModelIndex& )));

    updateContextMenuSignals();

    m_tableHeading = new QLabel(m_mainWidget);

    m_layout->addWidget(m_tableHeading);
    m_layout->addWidget(m_tableView);

  
    return m_mainWidget;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void PdmUiTableViewEditor::configureAndUpdateUi(const QString& uiConfigName)
{
    m_tableModelPdm->setPdmData(m_pdmListField, uiConfigName);

    if (m_tableModelPdm->rowCount() > 0)
    {
        for (int i = 0; i < m_tableModelPdm->columnCount(); i++)
        {
            if (m_tableModelPdm->isRepresentingBoolean(m_tableModelPdm->index(0, i)))
            {
                m_tableView->setItemDelegateForColumn(i, m_checkboxDelegate);
            }
            else
            {
                m_tableView->setItemDelegateForColumn(i, m_delegate);
            }
        }
    }

    if (m_pdmListField)
    {
        m_tableHeading->setText(m_pdmListField->uiName(uiConfigName));
    }
    else
    {
        m_tableHeading->setText("");
    }
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void PdmUiTableViewEditor::setListField(PdmPointersFieldHandle* pdmListField)
{
    m_pdmListField = pdmListField;

    this->bindToPdmItem(m_pdmListField);

    if (!m_pdmListField)
    {
        m_tableModelPdm->setPdmData(NULL, "");
        m_tableHeading->setText("");
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void PdmUiTableViewEditor::selectedUiItems(const QModelIndexList& modelIndexList, std::vector<PdmUiItem*>& objects)
{
    for (int i = 0; i < modelIndexList.size(); i++)
    {
        QModelIndex mi = modelIndexList[i];
        int row = mi.row();

        PdmObject* pdmObj = m_tableModelPdm->pdmObjectForRow(row);

        if (pdmObj)
        {
            objects.push_back(pdmObj);
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void PdmUiTableViewEditor::customMenuRequested(QPoint pos)
{
    QModelIndex mi = m_tableView->indexAt(pos);
    if (!mi.isValid())
    {
        std::vector<PdmUiItem*> items;
        items.push_back(m_pdmListField);

        SelectionManager::instance()->setSelectedItems(items, SelectionManager::CURRENT);
    }

    QMenu menu;

    caf::PdmCommandFeatureManager* commandManager = caf::PdmCommandFeatureManager::instance();

    menu.addAction(commandManager->action("PdmListField_AddItem", "Add new row"));
    menu.addAction(commandManager->action("PdmListField_DeleteItem", "Delete row"));

    // Qt doc: QAbstractScrollArea and its subclasses that map the context menu event to coordinates of the viewport().
    QPoint globalPos = m_tableView->viewport()->mapToGlobal(pos);

    menu.exec(globalPos);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QTableView* PdmUiTableViewEditor::tableView()
{
    return m_tableView;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void PdmUiTableViewEditor::enableDefaultContextMenu(bool enable)
{
    m_useDefaultContextMenu = enable;

    updateContextMenuSignals();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void PdmUiTableViewEditor::updateContextMenuSignals()
{
    if (!m_tableView) return;

    if (m_useDefaultContextMenu)
    {
        m_tableView->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(m_tableView, SIGNAL(customContextMenuRequested(QPoint)), SLOT(customMenuRequested(QPoint)));
    }
    else
    {
        m_tableView->setContextMenuPolicy(Qt::DefaultContextMenu);
        disconnect(m_tableView, 0, this, 0);
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void PdmUiTableViewEditor::setSelectionRole(SelectionManager::SelectionRole role)
{
    m_selectionRole = role;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void PdmUiTableViewEditor::slotSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
    // The current index is used to select the row for add/delete commands


//     if (m_selectionRole != SelectionManager::UNDEFINED)
//     {
//         std::vector<PdmUiItem*> items;
//         selectedUiItems(selected.indexes(), items);
// 
//         SelectionManager::instance()->setSelectedItems(items, m_selectionRole);
//     }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void PdmUiTableViewEditor::slotCurrentChanged(const QModelIndex & current, const QModelIndex & previous)
{
    if (m_selectionRole != SelectionManager::UNDEFINED)
    {
        std::vector<PdmUiItem*> items;
        QModelIndexList list;
        list.append(current);
        selectedUiItems(list, items);

        SelectionManager::instance()->setSelectedItems(items, m_selectionRole);
    }
}


} // end namespace caf


