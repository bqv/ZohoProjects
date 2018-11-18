#pragma once

#include <atlbase.h>
#include <atlcom.h>

#include "..\ZohoProjectsApi\zoho.h"
#include "..\ZohoProjectsApi\api.h"

#include "Credentials.h"

class IZohoClientImpl :
	public IZohoClient,
	private zoho::session
{
private:
	const Credential m_credential;

	void callback(const zoho::credential);

public:
	IZohoClientImpl();

	STDMETHOD(IsConnected)();
	STDMETHOD(Disconnect)();
};

class CZohoClient :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CZohoClient, &CLSID_ZohoClient>,
	public IZohoClientImpl
{
public:
	CZohoClient();

DECLARE_REGISTRY_RESOURCEID(102)

BEGIN_COM_MAP(CZohoClient)
	COM_INTERFACE_ENTRY(IZohoClient)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}
};

OBJECT_ENTRY_AUTO(__uuidof(ZohoClient), CZohoClient)
