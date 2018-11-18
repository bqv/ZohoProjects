#pragma once

#include "..\ZohoProjectsApi\zoho.h"
#include "..\ZohoProjectsApi\api.h"

#include "Credentials.h"

class ZohoClient :
	private zoho::session
{
private:
	const Credential m_credential;

	void callback(const zoho::credential) override;

VSL_DECLARE_NOT_COPYABLE(ZohoClient)

public:
	ZohoClient();

	BOOL IsConnected() const;
	HRESULT Disconnect();
};
