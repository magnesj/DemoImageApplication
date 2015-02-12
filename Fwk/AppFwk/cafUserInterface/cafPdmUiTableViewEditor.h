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


#pragma once

#include "cafPdmUiFieldEditorHandle.h"
#include "cafPdmUiObjectEditorHandle.h"
#include "cafPdmDocument.h"
#include "cafSelectionManager.h"

#include <QWidget>
#include <QPointer>
#include <QAbstractItemModel>

class QTableView;
class QLabel;
class QItemSelection;
class QMenu;

namespace caf 
{

class PdmUiCheckBoxDelegate;
class PdmUiFieldEditorHandle;
class PdmUiItem;
class PdmUiTableViewDelegate;
class PdmUiTableViewModel;


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------

class PdmUiTableViewEditor : public PdmUiEditorHandle
{
    Q_OBJECT

public:
    PdmUiTableViewEditor();
    ~PdmUiTableViewEditor();

    void            enableDefaultContextMenu(bool enable);
    void            setSelectionRole(SelectionManager::SelectionRole role);

    void            setListField(PdmPointersFieldHandle* pdmListField);
    QWidget*        createWidget(QWidget* parent);

    QTableView*     tableView();

protected:
    virtual void    configureAndUpdateUi(const QString& uiConfigName);

private:
    void            updateContextMenuSignals();
    void            selectedUiItems(const QModelIndexList& modelIndexList, std::vector<PdmUiItem*>& objects);

private slots:
    void            customMenuRequested(QPoint pos);
    void            slotSelectionChanged(const QItemSelection &, const QItemSelection &);
    void            slotCurrentChanged(const QModelIndex & current, const QModelIndex & previous);

private:
    std::map<QString, PdmUiFieldEditorHandle*>  m_fieldViews; 

    QPointer<QWidget>       m_mainWidget;
    QLayout*                m_layout;
    QLabel*                 m_tableHeading;

    QTableView*             m_tableView;
    PdmUiTableViewModel*    m_tableModelPdm;

    PdmPointersFieldHandle* m_pdmListField;
    PdmUiTableViewDelegate* m_delegate;
    PdmUiCheckBoxDelegate*  m_checkboxDelegate;

    bool                    m_useDefaultContextMenu;
    SelectionManager::SelectionRole m_selectionRole;

};



} // end namespace caf
