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

#include "cafPdmUiTreeEditorHandle.h"
#include "cafPdmUiFieldEditorHandle.h"

#include <QAbstractItemModel>
#include <QPointer>
#include <QWidget>


class MySortFilterProxyModel;

class QGridLayout;
class QMenu;
class QTreeView;
class QVBoxLayout;

namespace caf 
{

class PdmUiItem;
class PdmUiTreeViewModel;

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
class PdmUiTreeViewEditorAttribute : public PdmUiEditorAttribute
{
public:
    PdmUiTreeViewEditorAttribute()
    {
    }

public:
    QStringList columnHeaders;
};


class PdmUiTreeViewEditor : public PdmUiTreeEditorHandle
{
    Q_OBJECT
public:
    PdmUiTreeViewEditor();
    ~PdmUiTreeViewEditor();

    void                enableDefaultContextMenu(bool enable);

    QTreeView*          treeView();

    void                selectedUiItems(std::vector<PdmUiItem*>& objects);
    QWidget*            createWidget(QWidget* parent);


protected:
    virtual void        configureAndUpdateUi(const QString& uiConfigName);

    virtual void        updateMySubTree(PdmUiItem* uiItem);

    void                updateContextMenuSignals();

private slots:
    void                customMenuRequested(QPoint pos);
    void                slotCurrentChanged(const QModelIndex & current, const QModelIndex & previous);

private:
    void                uiItemsFromModelIndexList(const QModelIndexList& modelIndexList, std::vector<PdmUiItem*>& objects);


private:
    QPointer<QWidget>   m_mainWidget;
    QVBoxLayout*        m_layout;

    QTreeView*          m_treeView;
    PdmUiTreeViewModel* m_treeViewModel;

    PdmUiTreeViewEditorAttribute m_editorAttributes;

    bool                m_useDefaultContextMenu;
};



} // end namespace caf
