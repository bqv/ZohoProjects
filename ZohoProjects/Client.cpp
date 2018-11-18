#include "stdafx.h"

#include "Resource.h"
#include "..\ZohoProjectsUI\Resource.h"
#include "Guids.h"
#include "..\ZohoProjectsUI\CommandIds.h"

#include "ZohoProjects.h"
#include "Client.h"

#include "Serializer.h"

static LPCTSTR CREDENTIAL_TARGET = U("Zohorojects/tokens");
static UINT8 CREDENTIAL_VERSION = 1;

CZohoClient::CZohoClient()
	: IZohoClientImpl()
{
}

IZohoClientImpl::IZohoClientImpl()
	: zoho::session()
	, m_credential(CREDENTIAL_TARGET)
{
	CString username;
	CAtlArray<BYTE> blob;

	HRESULT hr;
	hr = m_credential.Read(username, blob);
	if (FAILED(hr)) return;

	Serializer sr;
	sr.Append(blob.GetData(), blob.GetCount());

	UINT8 ver, nil;
	if (!(sr >> ver) || (ver != CREDENTIAL_VERSION)) return;
	if (!(sr >> nil) || (nil != 0)) return;

	std::basic_string<TCHAR> accesstoken, refreshtoken, tokentype, scope;
	UINT64 expiresin;
	if (!(sr >> accesstoken)) return;
	if (!(sr >> refreshtoken)) return;
	if (!(sr >> tokentype)) return;
	if (!(sr >> scope)) return;
	if (!(sr >> expiresin)) return;

	zoho::session::set(zoho::credential(accesstoken, refreshtoken, tokentype, scope, expiresin));
}

void IZohoClientImpl::callback(const zoho::credential p_cred)
{
	Serializer sr;

	sr << (UINT8)CREDENTIAL_VERSION;
	sr << (UINT8)0;
	sr << p_cred.access_token();
	sr << p_cred.refresh_token();
	sr << p_cred.token_type();
	sr << p_cred.scope();
	sr << p_cred.expires_in();

	HRESULT hr;
	hr = m_credential.Write(CREDENTIAL_TARGET, (LPBYTE)sr.GetData(), sr.SizeBytes());
	if (FAILED(hr)) return;
}

STDMETHODIMP IZohoClientImpl::IsConnected()
{
	return *this ? S_OK : E_NOT_SET;
}

STDMETHODIMP IZohoClientImpl::Disconnect()
{
	zoho::session::expire();

	HRESULT hr;
	hr = m_credential.Delete();
	return SUCCEEDED(hr) ? S_OK : HRESULT_FROM_WIN32(::GetLastError());
}
