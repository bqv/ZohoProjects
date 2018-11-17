//
// ProjectExplorer.h
//
// This file contains the implementation of a tool window that hosts a .NET user control
//

#pragma once

#include <initguid.h>
#include <AtlWin.h>
#include <VSLWindows.h>
#include <CommCtrl.h>

#include "..\ZohoProjectsApi\project.h"

#include "..\ZohoProjectsUI\Resource.h"

class ProjectExplorerHierarchy :
	public IVsUIHierarchy
{
public:
	zoho::project rootItem;
	zoho::project childItem1;
	zoho::project childItem2;
	CComPtr<IServiceProvider> m_serviceProvider;

	ProjectExplorerHierarchy()
		: rootItem(web::json::value::object())
		, childItem1(web::json::value::object())
		, childItem2(web::json::value::object())
	{
	}

	zoho::project * GetItem(int itemid)
	{
		switch (itemid)
		{
			case VSITEMID_ROOT:
				return &rootItem;
			case 1:
				return &childItem1;
			case 2:
				return &childItem2;
		}
		return NULL;
	}

	// Inherited via IVsUIHierarchy
	STDMETHOD(QueryInterface)(REFIID riid, void ** ppvObject) override
	{
		// Always set out parameter to NULL, validating it first.
		if (!ppvObject)
			return E_INVALIDARG;
		*ppvObject = NULL;
		if (riid == IID_IUnknown || riid == IID_UIHierarchy)
		{
			// Increment the reference count and return the pointer.
			*ppvObject = (LPVOID)this;
			AddRef();
			return NOERROR;
		}
		return E_NOINTERFACE;
	}
	STDMETHOD_(ULONG,AddRef)(void) override
	{
		InterlockedIncrement(&m_cRef);
		return m_cRef;
	}
	STDMETHOD_(ULONG,Release)(void) override
	{
		ULONG ulRefCount = InterlockedDecrement(&m_cRef);
		if (0 == m_cRef)
		{
			delete this;
		}
		return ulRefCount;
	}
	STDMETHOD(SetSite)(IServiceProvider * pSP) override 
	{
		m_serviceProvider = pSP;
		return S_OK;
	}
	STDMETHOD(GetSite)(IServiceProvider ** ppSP) override 
	{
		m_serviceProvider->QueryService(IID_IOleManager, ppSP);
		return S_OK;
	}
	STDMETHOD(QueryClose)(BOOL * pfCanClose) override 
	{
		*pfCanClose = TRUE;
		return S_OK;
	}
	STDMETHOD(Close)(void) override 
	{
		return S_OK;
	}
	STDMETHOD(GetGuidProperty)(VSITEMID itemid, VSHPROPID propid, GUID * pguid) override 
	{
		memset(pguid, 0, sizeof(GUID));
		return DISP_E_MEMBERNOTFOUND;
	}
	STDMETHOD(SetGuidProperty)(VSITEMID itemid, VSHPROPID propid, REFGUID rguid) override 
	{
		return E_NOTIMPL;
	}
	STDMETHOD(GetProperty)(VSITEMID itemid, VSHPROPID propid, VARIANT * pvar) override 
	{
		VariantClear(pvar);
		switch (propid)
		{
			case VSHPROPID_CmdUIGuid:
				*pvar = CComVariant(&CLSID_ZohoProjectsCmdSet);
				break;

			case (int)VSHPROPID_Parent:
				if (itemid == VSITEMID_ROOT)
					*pvar = CComVariant(VSITEMID_NIL);
				else
					*pvar = CComVariant(VSITEMID_ROOT);
				break;

			case (int)VSHPROPID_FirstChild:
				if (itemid == VSITEMID_ROOT)
					*pvar = CComVariant(childItem1.id);
				else
					*pvar = CComVariant(VSITEMID_NIL);
				break;

			case (int)VSHPROPID_NextSibling:
				if (itemid == childItem1.id)
					*pvar = CComVariant(childItem2.id);
				else
					*pvar = CComVariant(VSITEMID_NIL);
				break;

			case (int)VSHPROPID_Expandable:
				if (itemid == VSITEMID_ROOT)
					*pvar = CComVariant(true);
				else
					*pvar = CComVariant(false);
				break;

			case (int)VSHPROPID_IconImgList:
			case (int)VSHPROPID_OpenFolderIconHandle:
				*pvar = CComVariant((int)0);
				break;

			case (int)VSHPROPID_IconIndex:
			case (int)VSHPROPID_OpenFolderIconIndex:
				*pvar = CComVariant(0);
				break;

			case (int)VSHPROPID_Caption:
			case (int)VSHPROPID_SaveName:
				*pvar = CComVariant(0);
				break;

			case (int)VSHPROPID_ShowOnlyItemCaption:
				*pvar = CComVariant(true);
				break;

			case (int)VSHPROPID_ParentHierarchy:
				if (itemid == childItem1.id || itemid == childItem2.id)
					*pvar = CComVariant(this);
				break;
		}

		if (CComVariant() != *pvar)
			return S_OK;

		return DISP_E_MEMBERNOTFOUND;
	}
	STDMETHOD(SetProperty)(VSITEMID itemid, VSHPROPID propid, VARIANT var) override 
	{
		return E_NOTIMPL;
	}
	STDMETHOD(GetNestedHierarchy)(VSITEMID itemid, REFIID iidHierarchyNested, void ** ppHierarchyNested, VSITEMID * pitemidNested) override 
	{
		*ppHierarchyNested = NULL;
		*pitemidNested = 0;
		return E_NOTIMPL;
	}
	STDMETHOD(GetCanonicalName)(VSITEMID itemid, BSTR * pbstrName) override 
	{
		return E_INVALIDARG;
	}
	STDMETHOD(ParseCanonicalName)(LPCOLESTR pszName, VSITEMID * pitemid) override 
	{
		*pitemid = 0;
		return E_NOTIMPL;
	}
	STDMETHOD(Unused0)(void) override 
	{
		return E_NOTIMPL;
	}
	STDMETHOD(AdviseHierarchyEvents)(IVsHierarchyEvents * pEventSink, VSCOOKIE * pdwCookie) override
	{
		*pdwCookie = 0;
		return S_OK;
	}
	STDMETHOD(UnadviseHierarchyEvents)(VSCOOKIE dwCookie) override 
	{
		return S_OK;
	}
	STDMETHOD(Unused1)(void) override 
	{
		return E_NOTIMPL;
	}
	STDMETHOD(Unused2)(void) override 
	{
		return E_NOTIMPL;
	}
	STDMETHOD(Unused3)(void) override 
	{
		return E_NOTIMPL;
	}
	STDMETHOD(Unused4)(void) override 
	{
		return E_NOTIMPL;
	}
	STDMETHOD(QueryStatusCommand)(VSITEMID itemid, const GUID * pguidCmdGroup, ULONG cCmds, OLECMD prgCmds[], OLECMDTEXT * pCmdText) override 
	{
		return OLECMDERR_E_UNKNOWNGROUP;
	}
	STDMETHOD(ExecCommand)(VSITEMID itemid, const GUID * pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, VARIANT * pvaIn, VARIANT * pvaOut) override 
	{
		return OLECMDERR_E_NOTSUPPORTED;
	}

private:
	ULONG m_cRef;
};

// {624ed9c3-bdfd-41fa-96c3-7c824ea32e3d}
DEFINE_GUID(EnvironmentColorsCategory, 
0x624ed9c3, 0xbdfd, 0x41fa, 0x96, 0xc3, 0x7c, 0x82, 0x4e, 0xa3, 0x2e, 0x3d);

#define VS_RGBA_TO_COLORREF(rgba) (rgba & 0x00FFFFFF)

class ProjectExplorerPane :
	public CComObjectRootEx<CComSingleThreadModel>,
	public VsWindowPaneFromResource<ProjectExplorerPane, IDD_ZohoProjects_ProjectExplorer>,
	public VsWindowFrameEventSink<ProjectExplorerPane>,
	public VSL::ISupportErrorInfoImpl<
	InterfaceSupportsErrorInfoList<IVsWindowPane,
	InterfaceSupportsErrorInfoList<IVsWindowFrameNotify,
	InterfaceSupportsErrorInfoList<IVsWindowFrameNotify3> > > >,
	public IVsBroadcastMessageEvents,
	public IVsToolWindowToolbar,
	public IVsWindowSearch
{
	VSL_DECLARE_NOT_COPYABLE(ProjectExplorerPane)

protected:
	enum map
	{
		TreeView = 1,
		Button = 2,
	};

	// Protected constructor called by CComObject<ProjectExplorerPane>::CreateInstance.
	ProjectExplorerPane() :
		VsWindowPaneFromResource(),
		m_TreeView(WC_TREEVIEW, this, map::TreeView),
		m_Button(WC_BUTTON, this, map::Button),
		m_hBackground(nullptr),
		m_BroadcastCookie(VSCOOKIE_NIL)
	{
	}

	~ProjectExplorerPane() {}
	
public:

BEGIN_COM_MAP(ProjectExplorerPane)
	COM_INTERFACE_ENTRY(IVsWindowPane)
	COM_INTERFACE_ENTRY(IVsWindowFrameNotify)
	COM_INTERFACE_ENTRY(IVsWindowFrameNotify3)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IVsBroadcastMessageEvents)
	COM_INTERFACE_ENTRY(IVsToolWindowToolbar)
	COM_INTERFACE_ENTRY(IVsWindowSearch)
END_COM_MAP()

BEGIN_MSG_MAP(ProjectExplorerPane)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDlg)
ALT_MSG_MAP(map::TreeView)
ALT_MSG_MAP(map::Button)
END_MSG_MAP()

	// Function called by VsWindowPaneFromResource at the end of SetSite; at this point the
	// window pane is constructed and sited and can be used, so this is where we can initialize
	// the event sink by siting it.
	void PostSited(IVsPackageEnums::SetSiteResult /*result*/)
	{
		VsWindowFrameEventSink<ProjectExplorerPane>::SetSite(GetVsSiteCache());
		CComPtr<IVsShell> spShell = GetVsSiteCache().GetCachedService<IVsShell, SID_SVsShell>();
		spShell->AdviseBroadcastMessages(this, &m_BroadcastCookie);
		InitVSColors();
	}

	// Function called by VsWindowPaneFromResource at the end of ClosePane.
	// Perform necessary cleanup.
	void PostClosed()
	{
		if (nullptr != m_hBackground)
		{
			::DeleteBrush(m_hBackground);
			m_hBackground = nullptr;
		}

		CComPtr<IVsShell> spShell = GetVsSiteCache().GetCachedService<IVsShell, SID_SVsShell>();
		if (nullptr != spShell && VSCOOKIE_NIL != m_BroadcastCookie)
		{
			spShell->UnadviseBroadcastMessages(m_BroadcastCookie);
			m_BroadcastCookie = VSCOOKIE_NIL;
		}
	}

	// Callback function called by ToolWindowBase when the size of the window changes.
	void OnFrameSize(int x, int y, int w, int h)
	{
	}

	// Handled to set the color that should be used to draw the background of the Window Pane.
	LRESULT OnCtlColorDlg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if (nullptr != m_hBackground)
		{
			bHandled = TRUE;
		}
		else
		{
			bHandled = FALSE;
		}

		return (LRESULT)m_hBackground;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		RECT rc;
		GetClientRect(&rc);
		rc.right -= rc.left;
		rc.bottom -= rc.top;
		rc.top = rc.left = 0;

		m_TreeView.Create(GetHWND(), rc, U("Tree View"),
			TVS_HASBUTTONS | TVS_DISABLEDRAGDROP);
		VSL_CHECKBOOLEAN(m_TreeView.m_hWnd != NULL, E_UNEXPECTED);

		m_Button.Create(GetHWND(), RECT({20,20,50,50}), U("Button"),
			0);
		VSL_CHECKBOOLEAN(m_Button.m_hWnd != NULL, E_UNEXPECTED);

		return FALSE;
	}

#pragma region
	// Inherited via IVsToolWindowToolbar
	STDMETHOD(GetBorder)(RECT *prc) override
	{
		GetClientRect(prc);
		return S_OK;
	}

	STDMETHOD(SetBorderSpace)(LPCBORDERWIDTHS pbw) override
	{
		return S_OK;
	}
#pragma endregion IVsToolwindowToolbar

#pragma region
	// Inherited via IVsWindowSearch
	STDMETHOD(get_SearchEnabled)(VARIANT_BOOL *pfEnabled) override
	{
		*pfEnabled = TRUE;
		return S_OK;
	}

	STDMETHOD(get_Category)(GUID *pguidCategoryId) override
	{
		return S_OK;
	}

	STDMETHOD(CreateSearch)(VSCOOKIE dwCookie, IVsSearchQuery *pSearchQuery, IVsSearchCallback *pSearchCallback, IVsSearchTask **ppSearchTask) override
	{
		return S_OK;
	}

	STDMETHOD(ClearSearch)(void) override
	{
		return S_OK;
	}

	STDMETHOD(ProvideSearchSettings)(IVsUIDataSource *pSearchSettings) override
	{
		return S_OK;
	}

	STDMETHOD(get_SearchFiltersEnum)(IVsEnumWindowSearchFilters **ppEnum) override
	{
		return S_OK;
	}

	STDMETHOD(get_SearchOptionsEnum)(IVsEnumWindowSearchOptions **ppEnum) override
	{
		return S_OK;
	}

	STDMETHOD(OnNavigationKeyDown)(VSSEARCHNAVIGATIONKEY dwNavigationKey, VSUIACCELMODIFIERS dwModifiers, VARIANT_BOOL *pfHandled) override
	// Inherited via IVsWindowSearch
	{
		return S_OK;
	}
#pragma endregion IVsWindowSearch

#pragma region
	// Inherited via IVsWindowPane
	STDMETHOD(OnBroadcastMessage)(UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		switch (uMsg)
		{
		case WM_SYSCOLORCHANGE:
		case WM_PALETTECHANGED:
			// Re-initialize VS colors when the theme changes.
			InitVSColors();
			break;
		}

		return S_OK;
	}
#pragma endregion IVsWindowPane

private:
	// Initialize colors that are used to render the Window Pane.
	void InitVSColors()
	{
		// Obtain IVsUIShell5 from IVsUIShell
		CComQIPtr<IVsUIShell5> spIVsUIShell5(GetVsSiteCache().GetCachedService<IVsUIShell, SID_SVsUIShell>());
		VS_RGBA vsColor;

		if (nullptr != m_hBackground)
		{
			::DeleteBrush(m_hBackground);
			m_hBackground = nullptr;
		}

		if (nullptr != spIVsUIShell5 && SUCCEEDED(spIVsUIShell5->GetThemedColor(EnvironmentColorsCategory, L"Window", TCT_Background, &vsColor)))
		{
			COLORREF crBackground = VS_RGBA_TO_COLORREF(vsColor);
			m_hBackground = ::CreateSolidBrush(crBackground);
		}
		
		if (::IsWindow(this->m_hWnd))
		{
			::InvalidateRect(this->m_hWnd, nullptr /* lpRect */ , TRUE /* bErase */);
		}
	}

	HBRUSH m_hBackground;
	VSCOOKIE m_BroadcastCookie;
	CContainedWindow m_TreeView;
	CContainedWindow m_Button;
};

class ProjectExplorer :
	public VSL::ToolWindowBase<ProjectExplorer>
{
public:
	// Constructor of the tool window object.
	// The goal of this constructor is to initialize the base class with the site cache
	// of the owner package.
	ProjectExplorer(const PackageVsSiteCache& rPackageVsSiteCache):
		ToolWindowBase(rPackageVsSiteCache)
	{
	}

	// Caption of the tool window.
	const wchar_t* const GetCaption() const
	{
		static CStringW strCaption;
		// Avoid to load the string from the resources more that once.
		if (0 == strCaption.GetLength())
		{
			VSL_CHECKBOOL_GLE(
				strCaption.LoadStringW(_AtlBaseModule.GetResourceInstance(), IDS_ProjectExplorer_Title));
		}
		return strCaption;
	}

	// Creation flags for this tool window.
	VSCREATETOOLWIN GetCreationFlags() const
	{
		return CTW_fInitNew|CTW_fForceCreate|CTW_fToolbarHost;
	}

	// Return the GUID of the persistence slot for this tool window.
	const GUID& GetToolWindowGuid() const
	{
		return CLSID_guidPersistanceSlot;
	}

	IUnknown* GetViewObject()
	{
		// Should only be called once per-instance
		VSL_CHECKBOOLEAN_EX(m_spView == NULL, E_UNEXPECTED, IDS_E_GETVIEWOBJECT_CALLED_AGAIN);

		// Create the object that implements the window pane for this tool window.
		CComObject<ProjectExplorerPane>* pViewObject;
		VSL_CHECKHRESULT(CComObject<ProjectExplorerPane>::CreateInstance(&pViewObject));

		// Get the pointer to IUnknown for the window pane.
		HRESULT hr = pViewObject->QueryInterface(IID_IUnknown, (void**)&m_spView);
		if (FAILED(hr))
		{
			// If QueryInterface failed, then there is something wrong with the object.
			// Delete it and throw an exception for the error.
			delete pViewObject;
			VSL_CHECKHRESULT(hr);
		}

		return m_spView;
	}

	// This method is called by the base class after the tool window is created.
	// We use it to set the icon for this window.
	void PostCreate()
	{
		CComVariant srpvt;
		srpvt.vt = VT_I4;
		srpvt.intVal = IDB_IMAGES;
		// We don't want to make the window creation fail only becuase we can not set
		// the icon, so we will not throw if SetProperty fails.
		if (SUCCEEDED(GetIVsWindowFrame()->SetProperty(VSFPROPID_BitmapResource, srpvt)))
		{
			srpvt.intVal = 1;
			GetIVsWindowFrame()->SetProperty(VSFPROPID_BitmapIndex, srpvt);
		}

		VARIANT variant;
		VSL_CHECKHRESULT(GetIVsWindowFrame()->GetProperty(VSFPROPID_DocView, &variant));
		CComPtr<IUnknown> docview = variant.punkVal;
		auto pane = static_cast<ProjectExplorerPane*>(
						static_cast<IVsWindowPane*>(
							static_cast<IUnknown*>(
								docview)));

		CComPtr<IVsUIShell> spUIShell;
		VSL_CHECKHRESULT(GetVsSiteCache().QueryService(SID_SVsUIShell, &spUIShell));
		VSL_CHECKHRESULT(spUIShell->SetupToolbar(pane->GetHWND(), (IVsToolWindowToolbar*)pane, &m_ToolbarHost));
		VSL_CHECKBOOLEAN(m_ToolbarHost != nullptr, E_UNEXPECTED);
		VSL_CHECKHRESULT(m_ToolbarHost->AddToolbar(VSTWT_TOP, &CLSID_ZohoProjectExplorerCmdSet, zohoProjectExplorerToolbar));
	}

private:
	CComPtr<IUnknown> m_spView;
	CComPtr<IVsToolWindowToolbarHost> m_ToolbarHost;
	ProjectExplorerHierarchy hierarchy;
};
