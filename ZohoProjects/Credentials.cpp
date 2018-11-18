#include "stdafx.h"
#include "Credentials.h"

Credential::Credential(LPCTSTR pTargetName, DWORD pPersist, DWORD pType)
	: m_Type(pType)
	, m_TargetName(pTargetName)
	, m_Persist(pPersist)
{
}

HRESULT Credential::Write(LPCTSTR pUsername, LPBYTE pBlob, DWORD pBlobSize) const
{
	CREDENTIAL cred = { 0 };
	cred.Type = m_Type;
	cred.TargetName = const_cast<LPTSTR>((LPCTSTR)m_TargetName);
	cred.CredentialBlobSize = pBlobSize;
	cred.CredentialBlob = (LPBYTE)pBlob;
	cred.Persist = m_Persist;
	cred.UserName = const_cast<LPTSTR>(pUsername);

	BOOL ok = ::CredWrite(&cred, 0);

	return ok ? S_OK : HRESULT_FROM_WIN32(::GetLastError());
}

HRESULT Credential::Read(CString& pUsername, CAtlArray<BYTE>& pBlob) const
{
	PCREDENTIAL pcred;
	BOOL ok = ::CredRead(m_TargetName, m_Type, 0, &pcred);
	if (!ok) return HRESULT_FROM_WIN32(::GetLastError());

	pUsername = pcred->UserName;
	pBlob.SetCount(pcred->CredentialBlobSize);
	CopyMemory(pBlob.GetData(), pcred->CredentialBlob, pcred->CredentialBlobSize);

	::CredFree(pcred);
	return S_OK;
}

HRESULT Credential::Delete() const
{
	BOOL ok = ::CredDelete(m_TargetName, m_Type, 0);

	return ok ? S_OK : HRESULT_FROM_WIN32(::GetLastError());
}
