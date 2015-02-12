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


#include "cafPdmUiTableViewModel.h"

#include "cafCommandFeaturesCore.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmUiComboBoxEditor.h"
#include "cafPdmUiTableItemEditor.h"


namespace caf
{

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
PdmUiTableViewModel::PdmUiTableViewModel(QWidget* parent)
    : QAbstractTableModel(parent)
{
    m_pdmList = NULL;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
int PdmUiTableViewModel::rowCount(const QModelIndex &parent /*= QModelIndex( ) */) const
{
    if (!m_pdmList) return 0;

    size_t itemCount = m_pdmList->size();
    return static_cast<int>(itemCount);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
int PdmUiTableViewModel::columnCount(const QModelIndex &parent /*= QModelIndex( ) */) const
{
    std::vector<PdmObject*> listObjects;
    if (m_pdmList)
    {
        m_pdmList->childObjects(&listObjects);

        if (listObjects.size() > 0)
        {
            PdmObject* pdmObject = listObjects[0];
            if (pdmObject)
            {
                std::vector<PdmFieldHandle*> fields;
                pdmObject->fields(fields);

                return static_cast<int>(fields.size());
            }
        }
    }

    return 0;
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QVariant PdmUiTableViewModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole */) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            caf::PdmFieldHandle* field = getField(createIndex(0, section));
            if (field)
            {
                return field->uiName(m_currentConfigName);
            }
        }
        else if (orientation == Qt::Vertical)
        {
            return section;
        }
    }

    return QVariant();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
Qt::ItemFlags PdmUiTableViewModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    Qt::ItemFlags flagMask = QAbstractItemModel::flags(index);

    if (isRepresentingBoolean(index))
    {
        flagMask = flagMask | Qt::ItemIsUserCheckable;
    }
    else
    {
        flagMask = flagMask | Qt::ItemIsEditable;
    }

    return flagMask;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool PdmUiTableViewModel::setData(const QModelIndex &index, const QVariant &value, int role /*= Qt::EditRole*/)
{
    if (role == Qt::CheckStateRole)
    {
        if (isRepresentingBoolean(index))
        {
            bool toggleOn = (value == Qt::Checked);
            caf::PdmFieldHandle* field = getField(index);
            PdmCommandFeaturesCore::instance()->fieldChangedCommand(field, toggleOn);

            return true;
        }
    }   

     return false;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QVariant PdmUiTableViewModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole */) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        caf::PdmFieldHandle* field = getField(index);
        if (field)
        {
            bool fromMenuOnly = false;
            QList<PdmOptionItemInfo> valueOptions = field->valueOptions(&fromMenuOnly);
            if (!valueOptions.isEmpty())
            {
                QStringList uiTexts = PdmOptionItemInfo::extractUiTexts(valueOptions);
                int listIndex = field->uiValue().toInt();
                return uiTexts.at(listIndex);
            }

            return field->uiValue();
        }
        else
        {
            assert(false);
        }
    }
    else if (role == Qt::CheckStateRole)
    {
        if (isRepresentingBoolean(index))
        {
            caf::PdmFieldHandle* field = getField(index);
            QVariant val = field->uiValue();
            bool isToggledOn = val.toBool();
            if (isToggledOn)
            {
                return Qt::Checked;
            }
            else
            {
                return Qt::Unchecked;
            }
        }
    }

    return QVariant();
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void PdmUiTableViewModel::setPdmData(PdmPointersFieldHandle* listField, const QString& configName)
{
    beginResetModel();

    m_pdmList = listField;
    m_currentConfigName = configName;

    PdmUiOrdering config;

    std::vector<PdmObject*> listObjects;
    if (listField)
    {
        listField->childObjects(&listObjects);
    }

    if (listObjects.size() > 0)
    {
        PdmObject* firstObject = listObjects[0];
        firstObject->uiOrdering(configName, config);
    }

    const std::vector<PdmUiItem*>& uiItems = config.uiItems();

    // Set all fieldViews to be unvisited
    std::map<QString, PdmUiFieldEditorHandle*>::iterator it;
    for (it = m_fieldEditors.begin(); it != m_fieldEditors.end(); ++it)
    {
        it->second->setField(NULL);
    }

    m_modelColumnIndexToFieldIndex.clear();

    for (size_t i = 0; i < uiItems.size(); ++i)
    {
        if (uiItems[i]->isUiHidden(configName)) continue;

        if (uiItems[i]->isUiGroup()) continue;

        {
            PdmFieldHandle* field = dynamic_cast<PdmFieldHandle*>(uiItems[i]);
            PdmUiFieldEditorHandle* fieldEditor = NULL;

            // Find or create FieldEditor
            std::map<QString, PdmUiFieldEditorHandle*>::iterator it;
            it = m_fieldEditors.find(field->keyword());

            if (it == m_fieldEditors.end())
            {
                // If editor type is specified, find in factory
                if ( !uiItems[i]->uiEditorTypeName(configName).isEmpty() )
                {
                    fieldEditor = caf::Factory<PdmUiFieldEditorHandle, QString>::instance()->create(field->uiEditorTypeName(configName));
                }
                else
                { 
                    // Find the default field editor

                    QString editorTypeName = qStringTypeName(*field);

                    // Handle a single value field with valueOptions: Make a combobox

                    if (field->uiValue().type() != QVariant::List)
                    {
                        bool useOptionsOnly = true; 
                        QList<PdmOptionItemInfo> options = field->valueOptions( &useOptionsOnly);

                        if (!options.empty())
                        {
                            editorTypeName = PdmUiComboBoxEditor::uiEditorTypeName();
                        }
                    }

                    fieldEditor = caf::Factory<PdmUiFieldEditorHandle, QString>::instance()->create(editorTypeName);
                }

                if (fieldEditor)
                {
                    m_fieldEditors[field->keyword()] = fieldEditor;
                }
            }
            else
            {
                fieldEditor = it->second;
            }

            if (fieldEditor)
            {
                fieldEditor->setField(field); 

                //TODO: Create/update is not required at this point, as UI is recreated in getEditorWidgetAndTransferOwnership()
                // Can be moved, but a move will require changes in PdmUiFieldEditorHandle
                fieldEditor->createWidgets(NULL);
                fieldEditor->updateUi(configName);

                m_modelColumnIndexToFieldIndex.push_back(static_cast<int>(i));
            }
        }
    }


    // Remove all fieldViews not mentioned by the configuration from the layout

    std::vector< QString > fvhToRemoveFromMap;
    for (it = m_fieldEditors.begin(); it != m_fieldEditors.end(); ++it)
    {
        if (it->second->field() == 0)
        {
            PdmUiFieldEditorHandle* fvh = it->second;
            delete fvh;
            fvhToRemoveFromMap.push_back(it->first);               
        }
    }

    for (size_t i = 0; i < fvhToRemoveFromMap.size(); ++i)
    {
        m_fieldEditors.erase(fvhToRemoveFromMap[i]);
    }

    recreateTableItemEditors();

    endResetModel();
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
caf::PdmFieldHandle* PdmUiTableViewModel::getField(const QModelIndex &index) const
{
    std::vector<PdmObject*> listObjects;
    if (m_pdmList)
    {
        m_pdmList->childObjects(&listObjects);
    }

    if (index.row() < listObjects.size())
    {
        PdmObject* pdmObject = listObjects[index.row()];
        if (pdmObject)
        {
            std::vector<PdmFieldHandle*> fields;
            pdmObject->fields(fields);

            size_t fieldIndex = m_modelColumnIndexToFieldIndex[index.column()];
            if (fieldIndex < fields.size())
            {
                return fields[fieldIndex];
            }
            else
            {
                assert(false);
            }
        }
    }

    return NULL;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
PdmUiFieldEditorHandle* PdmUiTableViewModel::getEditor(const QModelIndex &index)
{
    PdmFieldHandle* field = getField(index);
    if (!field)
    {
        return NULL;
    }

    PdmUiFieldEditorHandle* editor = NULL;
    
    std::map<QString, PdmUiFieldEditorHandle*>::iterator it;
    it = m_fieldEditors.find(field->keyword());

    if (it != m_fieldEditors.end())
    {
        editor = it->second;
        if (editor)
        {
            editor->setField(field);
        }
    }
    
    return editor;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QWidget* PdmUiTableViewModel::getEditorWidgetAndTransferOwnership(QWidget* parent, const QModelIndex &index)
{
    PdmUiFieldEditorHandle* editor = getEditor(index);
    if (editor)
    {
        // Recreate editor widget, as the delegate takes ownership of the QWidget and destroys it when
        // edit is completed
        editor->createWidgets(parent);
        QWidget* editorWidget = editor->editorWidget();
        editorWidget->setParent(parent);

        return editorWidget;
    }

    return NULL;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void PdmUiTableViewModel::notifyDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    emit dataChanged(topLeft, bottomRight);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void PdmUiTableViewModel::recreateTableItemEditors()
{
    m_tableItemEditors.clear();

    std::vector<PdmObject*> listObjects;
    if (m_pdmList)
    {
        m_pdmList->childObjects(&listObjects);
    }

    for (size_t i = 0; i < listObjects.size(); i++)
    {
        PdmObject* pdmObject = listObjects[i];
        m_tableItemEditors.push_back(new PdmUiTableItemEditor(this, pdmObject, static_cast<int>(i)));
    }

}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
PdmObject* PdmUiTableViewModel::pdmObjectForRow(int row)
{
    std::vector<PdmObject*> listObjects;
    if (m_pdmList)
    {
        m_pdmList->childObjects(&listObjects);
    }

    if (row < listObjects.size())
    {
        return listObjects[row];
    }

    return NULL;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool PdmUiTableViewModel::isRepresentingBoolean(const QModelIndex &index) const
{
    caf::PdmFieldHandle* field = getField(index);
    if (field)
    {
        QVariant val = field->uiValue();
        if (val.type() == QVariant::Bool)
        {
            return true;
        }
    }

    return false;
}



} // end namespace caf
