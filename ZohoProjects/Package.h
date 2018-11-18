// Package.h
#pragma once

#include <atlstr.h>
#include <VSLCommandTarget.h>

#include "Resource.h"       // main symbols
#include "Guids.h"

#include "..\ZohoProjectsUI\Resource.h"
#include "..\ZohoProjectsUI\CommandIds.h"

#include "Client.h"
#include "ProjectExplorer.h"
#include <commctrl.h>

using namespace VSL;

class ATL_NO_VTABLE CZohoProjectsPackage : 
	// CComObjectRootEx and CComCoClass are used to implement a non-thread safe COM object, and 
	// a partial implementation for IUnknown (the COM map below provides the rest).
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CZohoProjectsPackage, &CLSID_ZohoProjects>,
	// Provides the implementation for IVsPackage to make this COM object into a VS Package.
	public IVsPackageImpl<CZohoProjectsPackage, &CLSID_ZohoProjects>,
	public IOleCommandTargetImpl<CZohoProjectsPackage>,
	// Provides consumers of this object with the ability to determine which interfaces support
	// extended error information.
	public ATL::ISupportErrorInfoImpl<&__uuidof(IVsPackage)>,
	public IZohoClientImpl
{
public:

// Provides a portion of the implementation of IUnknown, in particular the list of interfaces
// the CZohoProjectsPackage object will support via QueryInterface
BEGIN_COM_MAP(CZohoProjectsPackage)
	COM_INTERFACE_ENTRY(IVsPackage)
	COM_INTERFACE_ENTRY(IOleCommandTarget)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IZohoClient)
END_COM_MAP()

// COM objects typically should not be cloned, and this prevents cloning by declaring the 
// copy constructor and assignment operator private (NOTE:  this macro includes the declaration of
// a private section, so everything following this macro and preceding a public or protected 
// section will be private).
VSL_DECLARE_NOT_COPYABLE(CZohoProjectsPackage)

public:
	CZohoProjectsPackage()
		: m_ProjectExplorer(GetVsSiteCache())
	{
	}
	
	~CZohoProjectsPackage()
	{
	}

	// This function provides the error information if it is not possible to load
	// the UI dll. It is for this reason that the resource IDS_E_BADINSTALL must
	// be defined inside this dll's resources.
	static const LoadUILibrary::ExtendedErrorInfo& GetLoadUILibraryErrorInfo()
	{
		static LoadUILibrary::ExtendedErrorInfo errorInfo(IDS_E_BADINSTALL);
		return errorInfo;
	}

	// DLL is registered with VS via a pkgdef file. Don't do anything if asked to
	// self-register.
	static HRESULT WINAPI UpdateRegistry(BOOL bRegister)
	{
		return S_OK;
	}

// NOTE - the arguments passed to these macros can not have names longer then 30 characters
// Definition of the commands handled by this package
VSL_BEGIN_COMMAND_MAP()
    VSL_COMMAND_MAP_ENTRY(CLSID_ZohoProjectsCmdSet, zohoCreateTask, NULL, CommandHandler::ExecHandler(&OnMyCommand))
    VSL_COMMAND_MAP_ENTRY(CLSID_ZohoProjectsCmdSet, zohoProjectExplorer, NULL, CommandHandler::ExecHandler(&OnProjectExplorer))
    VSL_COMMAND_MAP_ENTRY(CLSID_ZohoProjectExplorerCmdSet, zohoProjectExplorerToolbar, NULL, CommandHandler::ExecHandler(&OnProjectExplorerToolbar))
VSL_END_VSCOMMAND_MAP()


// The tool map implements IVsPackage::CreateTool that is called by VS to create a tool window 
// when appropriate.
VSL_BEGIN_TOOL_MAP()
    VSL_TOOL_ENTRY(CLSID_guidPersistanceSlot, m_ProjectExplorer.CreateAndShow())
VSL_END_TOOL_MAP()

// Command handler called when the user selects the command to show the toolwindow.
void OnProjectExplorer(CommandHandler* /*pSender*/, DWORD /*flags*/, VARIANT* /*pIn*/, VARIANT* /*pOut*/)
{
    m_ProjectExplorer.CreateAndShow();
}

void OnProjectExplorerToolbar(CommandHandler* /*pSender*/, DWORD /*flags*/, VARIANT* /*pIn*/, VARIANT* /*pOut*/)
{
	OnMyCommand(NULL, NULL, NULL, NULL);
}

// Command handler called when the user selects the "My Command" command.
void OnMyCommand(CommandHandler* /*pSender*/, DWORD /*flags*/, VARIANT* /*pIn*/, VARIANT* /*pOut*/)
{
	// Get the string for the title of the message box from the resource dll.
	CComBSTR bstrTitle;
	VSL_CHECKBOOL_GLE(bstrTitle.LoadStringW(_AtlBaseModule.GetResourceInstance(), IDS_PROJNAME));
	// Get a pointer to the UI Shell service to show the message box.
	CComPtr<IVsUIShell> spUiShell = this->GetVsSiteCache().GetCachedService<IVsUIShell, SID_SVsUIShell>();
	LONG lResult;
	HRESULT hr = spUiShell->ShowMessageBox(
	                             0,
	                             CLSID_NULL,
	                             bstrTitle,
	                             W2OLE(L"Inside CZohoProjectsPackage::Exec"),
	                             NULL,
	                             0,
	                             OLEMSGBUTTON_OK,
	                             OLEMSGDEFBUTTON_FIRST,
	                             OLEMSGICON_INFO,
	                             0,
	                             &lResult);
	VSL_CHECKHRESULT(hr);
}

private:
    ProjectExplorer m_ProjectExplorer;
};

// This exposes CZohoProjectsPackage for instantiation via DllGetClassObject; however, an instance
// can not be created by CoCreateInstance, as CZohoProjectsPackage is specifically registered with
// VS, not the the system in general.
OBJECT_ENTRY_AUTO(CLSID_ZohoProjects, CZohoProjectsPackage)
