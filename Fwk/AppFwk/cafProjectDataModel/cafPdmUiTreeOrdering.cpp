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


#include "cafPdmUiTreeOrdering.h"
#include "cafPdmField.h"
#include "cafPdmUiEditorHandle.h"

namespace caf
{


    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    void PdmUiTreeOrdering::add(PdmFieldHandle * field)
    {
        PdmUiTreeOrdering* child = new PdmUiTreeOrdering(this, field);
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    void PdmUiTreeOrdering::add(PdmObject* object)
    {
        PdmUiTreeOrdering* child = new PdmUiTreeOrdering(this, object);
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    PdmUiTreeOrdering* PdmUiTreeOrdering::add(const QString & title, const QString& iconResourceName)
    {
        PdmUiTreeOrdering* child = new PdmUiTreeOrdering(title, iconResourceName);

        this->appendChild(child);
        return child;
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    bool PdmUiTreeOrdering::containsField(const PdmFieldHandle* field)
    {
        assert (field);
        for (int cIdx = 0; cIdx < this->childCount(); ++cIdx)
        {
            PdmUiTreeOrdering* child = dynamic_cast<PdmUiTreeOrdering*>(this->child(cIdx));

            if (child->m_field == field)
            {
                return true;
            }
        }

        return false;
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    bool PdmUiTreeOrdering::containsObject(const PdmObject* object)
    {
        assert (object);
        for (int cIdx = 0; cIdx < this->childCount(); ++cIdx)
        {
            PdmUiTreeOrdering* child = dynamic_cast<PdmUiTreeOrdering*>(this->child(cIdx));

            if (child->isRepresentingObject() && (child->object() == object))
            {
                return true;
            }
        }

        return false;
    }


    //--------------------------------------------------------------------------------------------------
    ///  Creates an new root PdmUiTreeOrdering item, pointing at a PdmObject
    //--------------------------------------------------------------------------------------------------
    PdmUiTreeOrdering::PdmUiTreeOrdering(PdmObject* pdmObject) : 
        m_parentItem(NULL),
        m_field(NULL),
        m_forgetRemainingFields(false),
        m_isToIgnoreSubTree(false),
        m_uiItem(NULL),
        m_object(pdmObject),
        m_treeItemEditor(NULL)
    {
    }

    //--------------------------------------------------------------------------------------------------
    ///  Creates an new root PdmUiTreeOrdering item, pointing at a field
    //--------------------------------------------------------------------------------------------------
    PdmUiTreeOrdering::PdmUiTreeOrdering( PdmFieldHandle* pdmField ) : 
        m_parentItem(NULL),
        m_field(pdmField),
        m_forgetRemainingFields(false),
        m_isToIgnoreSubTree(false),
        m_uiItem(NULL),
        m_object(NULL),
        m_treeItemEditor(NULL)
    {
        if (pdmField) m_object = pdmField->ownerObject();
    }

    //--------------------------------------------------------------------------------------------------
    /// Creates an new root PdmUiTreeOrdering item, as a display item only
    //--------------------------------------------------------------------------------------------------
    PdmUiTreeOrdering::PdmUiTreeOrdering(const QString & title, const QString& iconResourceName) : 
        m_parentItem(NULL),
        m_field(NULL),
        m_forgetRemainingFields(false),
        m_isToIgnoreSubTree(false),
        m_uiItem(NULL),
        m_object(NULL),
        m_treeItemEditor(NULL)
    {
        m_uiItem = new PdmUiItem();
        m_uiItem->setUiName(title);
        m_uiItem->setUiIcon(QIcon(iconResourceName));
    }

    //--------------------------------------------------------------------------------------------------
    ///  Creates an new PdmUiTreeOrdering item, and adds it to parent. If position is -1, it is added 
    ///  at the end of parents existing child list.
    //--------------------------------------------------------------------------------------------------
    PdmUiTreeOrdering::PdmUiTreeOrdering(PdmUiTreeOrdering* parent, PdmObject* pdmObject) :
        m_parentItem(parent),
        m_field(NULL),
        m_forgetRemainingFields(false),
        m_isToIgnoreSubTree(false),
        m_uiItem(NULL),
        m_object(pdmObject),
        m_treeItemEditor(NULL)
    {
        if (m_parentItem)
        {
            m_parentItem->m_childItems.push_back( this);
        }
    }

    //--------------------------------------------------------------------------------------------------
    ///  Creates an new PdmUiTreeOrdering item, and adds it to parent. If position is -1, it is added 
    ///  at the end of parents existing child list.
    //--------------------------------------------------------------------------------------------------
    PdmUiTreeOrdering::PdmUiTreeOrdering(PdmUiTreeOrdering* parent , PdmFieldHandle* pdmField ) : 
        m_parentItem(parent),
        m_field(pdmField),
        m_forgetRemainingFields(false),
        m_isToIgnoreSubTree(false),
        m_uiItem(NULL),
        m_object(NULL),
        m_treeItemEditor(NULL)
    {
        if (m_parentItem)
        {
            m_parentItem->m_childItems.push_back( this);
        }
        
        if (pdmField) m_object = pdmField->ownerObject();
    }



    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    PdmUiTreeOrdering::~PdmUiTreeOrdering()
    {
        if (m_uiItem)
        {
            delete m_uiItem;
        }

        if (m_treeItemEditor)
        {
            delete m_treeItemEditor;
        }

        qDeleteAll(m_childItems);
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    PdmObject* PdmUiTreeOrdering::object() const
    {
        assert(isRepresentingObject()); return m_object;
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    PdmFieldHandle* PdmUiTreeOrdering::field() const
    {
        assert(isRepresentingField()); return m_field;
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    PdmUiItem* PdmUiTreeOrdering::uiItem() const
    {
        assert(isDisplayItemOnly()); return m_uiItem;
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    PdmUiItem* PdmUiTreeOrdering::activeItem() const 
    {
        if (isRepresentingObject()) return m_object; 
        if (isRepresentingField())  return m_field; 
        if (isDisplayItemOnly()) return m_uiItem;
        return NULL;
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    void PdmUiTreeOrdering::setEditor(PdmUiEditorHandle* editor)
    {
        m_treeItemEditor = editor;
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    PdmUiEditorHandle* PdmUiTreeOrdering::editor()
    {
        return m_treeItemEditor;
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    void PdmUiTreeOrdering::debugDump(int level) const
    {
        for (int i = 0; i < level; ++i)
        {
            std::cout << "  ";
        }

        if (isValid())
        {
            char type = 'I';
            if (isRepresentingObject()) type = 'O';
            if (isRepresentingField()) type = 'F';
            if (isDisplayItemOnly()) type = 'D';

            std::cout << type << ": " << activeItem()->uiName().toLatin1().data() << std::endl;
        }
        else
        {
            std::cout << "NULL" << std::endl;
        }

        for (int i = 0; i < childCount(); ++i)
        {
            child(i)->debugDump(level+1);
        }
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    PdmUiTreeOrdering* PdmUiTreeOrdering::child(int index) const
    {
        assert(index < m_childItems.size());
        return m_childItems.value(index);
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    int PdmUiTreeOrdering::childCount() const
    {
        return m_childItems.count();
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    PdmUiTreeOrdering* PdmUiTreeOrdering::parent() const
    {
        return m_parentItem;
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    int PdmUiTreeOrdering::indexInParent() const
    {
        if (m_parentItem)
        {
            return m_parentItem->m_childItems.indexOf(const_cast<PdmUiTreeOrdering*>(this));
        }

        return 0;
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    void PdmUiTreeOrdering::appendChild(PdmUiTreeOrdering* child)
    {
        m_childItems.append(child);
        child->m_parentItem = this;
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    void PdmUiTreeOrdering::insertChild(int position, PdmUiTreeOrdering* child)
    {
        m_childItems.insert(position, child);
        child->m_parentItem = this;
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    bool PdmUiTreeOrdering::removeChildren(int position, int count)
    {
        if (position < 0 || position + count > m_childItems.size())
            return false;

        for (int row = 0; row < count; ++row)
        {
            PdmUiTreeOrdering* uiItem = m_childItems.takeAt(position);

            delete uiItem;
        }

        return true;
    }

    //--------------------------------------------------------------------------------------------------
    /// 
    //--------------------------------------------------------------------------------------------------
    bool PdmUiTreeOrdering::removeChildrenNoDelete(int position, int count)
    {
        if (position < 0 || position + count > m_childItems.size())
            return false;

        for (int row = 0; row < count; ++row)
        {
            m_childItems.removeAt(position);
        }
        return true;
    }



} //End of namespace caf

