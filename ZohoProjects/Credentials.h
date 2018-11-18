#pragma once

#include <wincred.h>

class Credential
{
private:
	DWORD m_Type;
	CString m_TargetName;
	DWORD m_Persist;

public:
	Credential(LPCTSTR pTargetName, DWORD pPersist = CRED_PERSIST_LOCAL_MACHINE, DWORD pType = CRED_TYPE_GENERIC);

	HRESULT Write(LPCTSTR pUsername, LPBYTE pBlob, DWORD pBlobSize) const;
	HRESULT Read(CString& pUsername, CAtlArray<BYTE>& pBlob) const;
	HRESULT Delete() const;
};
