#include "stdafx.h"
#include "ZohoTree.h"

ZohoTreeItem::ZohoTreeItem(ZohoClient& zoho)
	: m_zoho(zoho), m_children()
{
}

ZohoTreeItem::operator LPTVINSERTSTRUCT()
{
	return &m_insert;
}

ZohoTreeItem::operator HTREEITEM()
{
	return m_insert.item.hItem;
}

CTreeItem ZohoTreeItem::Insert(CTreeViewCtrlEx& tree)
{
	m_insert.item.hItem = tree.InsertItem(*this);
	CTreeItem ti(m_insert.item.hItem, &tree);
	ti.SetData((DWORD_PTR)this);
	return ti;
}

ZohoRootItem::ZohoRootItem(ZohoClient& zoho)
	: ZohoTreeItem(zoho), m_text(U("Zoho"))
{
	m_insert.hParent = NULL;
	m_insert.hInsertAfter = TVI_ROOT;
	m_insert.item.mask = TVIF_TEXT | TVIF_CHILDREN;
	m_insert.item.cchTextMax = m_text.GetLength();
	m_insert.item.pszText = m_text.GetBuffer();
	m_insert.item.cChildren = 1;
	m_insert.item.lParam = (LPARAM)this;

	try
	{
		for (const zoho::portal& entity : zoho::api::portals::all_portals(zoho))
		{
			ZohoTreeItem *oldItem = NULL;
			if (m_children.Lookup(entity.id, oldItem))
			{
				// TODO: Update oldItem
				continue;
			}
			else
			{
				m_children[entity.id] = new ZohoPortalItem(*this, entity);
			}
		}
	}
	catch (const std::exception& e)
	{
		auto msg = util::string(e.what());
		__debugbreak();
		m_zoho.Disconnect();
		VSL_CHECKHRESULT_EX(E_UNEXPECTED, msg.c_str());
	}
}

BOOL ZohoRootItem::Valid()
{
	return true;
}

CAutoPtr<CAtlList<ZohoTreeItem*>> ZohoRootItem::Enumerate()
{
	CAtlList<ZohoTreeItem*> *children = new CAtlList<ZohoTreeItem*>();
	for (POSITION pos = m_children.GetStartPosition(); pos != NULL; m_children.GetNext(pos))
	{
		ZohoTreeItem *item = m_children.GetValueAt(pos);
		LPTVINSERTSTRUCT is = static_cast<LPTVINSERTSTRUCT>(*item);
		is->hParent = *this;
		children->AddTail(item);
	}
	return CAutoPtr<CAtlList<ZohoTreeItem*>>(children);
}

ZohoPortalItem::ZohoPortalItem(ZohoRootItem& parent, const zoho::portal& entity)
	: ZohoTreeItem(parent.m_zoho), m_entity(entity), m_text(entity.name.c_str())
{
	m_insert.hParent = NULL;
	m_insert.hInsertAfter = TVI_LAST;
	m_insert.item.mask = TVIF_TEXT | TVIF_CHILDREN;
	m_insert.item.cchTextMax = m_text.GetLength();
	m_insert.item.pszText = m_text.GetBuffer();
	m_insert.item.cChildren = 1;
	m_insert.item.lParam = (LPARAM)this;

	try
	{
		for (const zoho::project& entity : zoho::api::projects::all_projects(m_zoho, m_entity))
		{
			ZohoTreeItem *oldItem = NULL;
			if (m_children.Lookup(entity.id, oldItem))
			{
				// TODO: Update oldItem
				continue;
			}
			else
			{
				m_children[entity.id] = new ZohoProjectItem(*this, entity);
			}
		}
	}
	catch (const std::exception& e)
	{
		auto msg = util::string(e.what());
		__debugbreak();
		m_zoho.Disconnect();
		VSL_CHECKHRESULT_EX(E_UNEXPECTED, msg.c_str());
	}
}

BOOL ZohoPortalItem::Valid()
{
	try
	{
		for (zoho::portal& entity : zoho::api::portals::all_portals(m_zoho))
		{
			if (entity.id == m_entity.id)
				return true;
		}
	}
	catch (const std::exception& e)
	{
		auto msg = util::string(e.what());
		__debugbreak();
		m_zoho.Disconnect();
		VSL_CHECKHRESULT_EX(E_UNEXPECTED, msg.c_str());
	}
	return false;
}

CAutoPtr<CAtlList<ZohoTreeItem*>> ZohoPortalItem::Enumerate()
{
	CAtlList<ZohoTreeItem*> *children = new CAtlList<ZohoTreeItem*>();
	for (POSITION pos = m_children.GetStartPosition(); pos != NULL; m_children.GetNext(pos))
	{
		ZohoTreeItem *item = m_children.GetValueAt(pos);
		LPTVINSERTSTRUCT is = static_cast<LPTVINSERTSTRUCT>(*item);
		is->hParent = *this;
		children->AddTail(item);
	}
	return CAutoPtr<CAtlList<ZohoTreeItem*>>(children);
}

ZohoProjectItem::ZohoProjectItem(ZohoPortalItem& parent, const zoho::project& entity)
	: ZohoTreeItem(parent.m_zoho), m_parent(parent.m_entity), m_entity(entity), m_text(entity.name.c_str())
{
	m_insert.hParent = NULL;
	m_insert.hInsertAfter = TVI_LAST;
	m_insert.item.mask = TVIF_TEXT | TVIF_CHILDREN;
	m_insert.item.cchTextMax = m_text.GetLength();
	m_insert.item.pszText = m_text.GetBuffer();
	m_insert.item.cChildren = 1;
	m_insert.item.lParam = (LPARAM)this;
}

BOOL ZohoProjectItem::Valid()
{
	try
	{
		for (zoho::project& entity : zoho::api::projects::all_projects(m_zoho, m_parent))
		{
			if (entity.id == m_entity.id)
				return true;
		}
	}
	catch (const std::exception& e)
	{
		auto msg = util::string(e.what());
		__debugbreak();
		m_zoho.Disconnect();
		VSL_CHECKHRESULT_EX(E_UNEXPECTED, msg.c_str());
	}
	return false;
}

CAutoPtr<CAtlList<ZohoTreeItem*>> ZohoProjectItem::Enumerate()
{
	CAtlList<ZohoTreeItem*> *children = new CAtlList<ZohoTreeItem*>();
	for (POSITION pos = m_children.GetStartPosition(); pos != NULL; m_children.GetNext(pos))
	{
		ZohoTreeItem *item = m_children.GetValueAt(pos);
		LPTVINSERTSTRUCT is = static_cast<LPTVINSERTSTRUCT>(*item);
		is->hParent = *this;
		children->AddTail(item);
	}
	return CAutoPtr<CAtlList<ZohoTreeItem*>>(children);
}
