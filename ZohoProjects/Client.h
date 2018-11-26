#pragma once

#include <atlbase.h>

#include "..\ZohoProjectsApi\zoho.h"
#include "..\ZohoProjectsApi\api.h"

#include "Credentials.h"
#include "ZohoTree.h"

class ZohoTreeItem;

class ZohoClient : public zoho::session
{
private:
	const Credential m_credential;

	void callback(const zoho::credential) override;

VSL_DECLARE_NOT_COPYABLE(ZohoClient)

public:
	ZohoClient();

	BOOL IsConnected() const;
	HRESULT Connect();
	HRESULT Disconnect();
	ZohoTreeItem& TreeHierarchy();
};
