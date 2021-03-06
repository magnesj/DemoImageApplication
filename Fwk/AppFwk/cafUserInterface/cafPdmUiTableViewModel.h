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

#pragma once

#include "cafPdmUiTreeOrdering.h"

#include <QAbstractItemModel>

namespace caf
{

class PdmObject;
class PdmUiItem;
class PdmUiTreeViewEditor;
class PdmUiTreeOrdering;
class PdmPointersFieldHandle;
class PdmUiFieldEditorHandle;
class PdmUiTableItemEditor;

//==================================================================================================
//
//
//==================================================================================================
//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
class PdmUiTableViewModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    PdmUiTableViewModel(QWidget* parent);

    void                    setPdmData(PdmPointersFieldHandle* pdmObject, const QString& configName);
    PdmObject*              pdmObjectForRow(int row);

    // Qt overrides
    virtual int             rowCount( const QModelIndex &parent = QModelIndex( ) ) const;
    virtual int             columnCount( const QModelIndex &parent = QModelIndex( ) ) const;
    virtual QVariant        data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
    virtual QVariant        headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

    virtual Qt::ItemFlags   flags(const QModelIndex &index) const;
    virtual bool            setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    void                    selectedUiItems(std::vector<PdmUiItem*>& objects);

    PdmUiFieldEditorHandle* getEditor(const QModelIndex &index);
    QWidget*                getEditorWidgetAndTransferOwnership(QWidget* parent, const QModelIndex &index);
    void                    notifyDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);

    bool                    isRepresentingBoolean(const QModelIndex &index) const;

private:
    PdmFieldHandle*         getField(const QModelIndex &index) const;
    void                    recreateTableItemEditors();

private:
    PdmPointersFieldHandle*                     m_pdmList;
    QString                                     m_currentConfigName;

    std::map<QString, PdmUiFieldEditorHandle*>  m_fieldEditors;
    std::vector<int>                            m_modelColumnIndexToFieldIndex;

    std::vector<PdmUiTableItemEditor*>          m_tableItemEditors;
};




} // End of namespace caf
