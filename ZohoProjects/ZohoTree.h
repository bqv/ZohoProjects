#pragma once

#include <atlapp.h>
#include <atlwin.h>
#include <atlctrls.h>

#include "..\ZohoProjectsApi\zoho.h"
#include "..\ZohoProjectsApi\api.h"

#include "Client.h"

class ZohoClient;

class ZohoTreeItem
{
protected:
	ZohoTreeItem(ZohoClient& zoho);

	ZohoClient& m_zoho;
	TVINSERTSTRUCT m_insert;
	CAtlMap<zoho::id, ZohoTreeItem*> m_children;
	
public:
	operator LPTVINSERTSTRUCT();
	operator HTREEITEM();

	CTreeItem Insert(CTreeViewCtrlEx& tree);

	virtual BOOL Valid() = 0;
	virtual CAutoPtr<CAtlList<ZohoTreeItem*>> Enumerate() = 0;
};

class ZohoRootItem : public ZohoTreeItem
{
private:
	friend class ZohoPortalItem;

	CAtlString m_text;

public:
	ZohoRootItem(ZohoClient& zoho);

	BOOL Valid() override;
	CAutoPtr<CAtlList<ZohoTreeItem*>> Enumerate() override;
};

class ZohoPortalItem : public ZohoTreeItem
{
private:
	friend class ZohoProjectItem;

	CAtlString m_text;
	zoho::portal m_entity;

public:
	ZohoPortalItem(ZohoRootItem& zoho, const zoho::portal& entity);

	BOOL Valid() override;
	CAutoPtr<CAtlList<ZohoTreeItem*>> Enumerate() override;
};

class ZohoProjectItem : public ZohoTreeItem
{
private:
	CAtlString m_text;
	zoho::project m_entity;
	zoho::portal m_parent;

public:
	ZohoProjectItem(ZohoPortalItem& zoho, const zoho::project& entity);

	BOOL Valid() override;
	CAutoPtr<CAtlList<ZohoTreeItem*>> Enumerate() override;
};
