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

#include <shellAPI.h>
#include <atlapp.h>
#include <atlwin.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atlctrlw.h>
#include <atlctrlx.h>

#include <cmath>

#include <Client.h>

#include "..\ZohoProjectsUI\Resource.h"

#include "Messages.h"
#include "Package.h"

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
};

// {624ed9c3-bdfd-41fa-96c3-7c824ea32e3d}
DEFINE_GUID(EnvironmentColorsCategory, 
0x624ed9c3, 0xbdfd, 0x41fa, 0x96, 0xc3, 0x7c, 0x82, 0x4e, 0xa3, 0x2e, 0x3d);

// {92ecf08e-8b13-4cf4-99e9-ae2692382185}
DEFINE_GUID(TreeViewColorsCatetory,
0x92ecf08e, 0x8b13, 0x4cf4, 0x99, 0xe9, 0xae, 0x26, 0x92, 0x38, 0x21, 0x85);

// GUID_HeaderColorsCatetory: {4997f547-1379-456e-b985-2f413cdfa536}
// GUID_SearchControlColorsCatetory: {f1095fad-881f-45f1-8580-589e10325eb8}

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
		Nil,
		Resize,
		TreeView,
		PlaceholderLabel,
	};

	// Protected constructor called by CComObject<ProjectExplorerPane>::CreateInstance.
	ProjectExplorerPane() :
		VsWindowPaneFromResource(),
		m_TreeView(WC_TREEVIEW, this, map::TreeView),
		m_PlaceholderLabel(WC_STATIC, this, map::PlaceholderLabel),
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
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
	MESSAGE_HANDLER(WM_ZOHO_RELOAD, OnZohoReload)
ALT_MSG_MAP(map::TreeView)
ALT_MSG_MAP(map::PlaceholderLabel)
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
		RECT rc;
		{
			rc.top = m_ClientBorder.top;
			rc.left = m_ClientBorder.left;
			rc.bottom = h - m_ClientBorder.bottom;
			rc.right = w - m_ClientBorder.right;
		}

		m_TreeView.SetWindowPos(NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 0);

		LONG padding = static_cast<LONG>(std::fmin(
			(rc.right - rc.left) * 0.20,
			(rc.bottom - rc.top) * 0.20
		));

		RECT p20;
		p20.right = rc.right - padding;
		p20.bottom = rc.bottom - padding;
		p20.top = p20.left = padding;

		m_PlaceholderLabel.SetWindowPos(NULL, p20.left, p20.top, p20.right - p20.left, p20.bottom - p20.top, 0);
	}

	// Handled to set the color that should be used to draw the background of the Window Pane.
	LRESULT OnCtlColorDlg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		m_TreeView.SetBkColor(VS_RGBA_TO_COLORREF(m_BkColor));
		m_TreeView.SetTextColor(VS_RGBA_TO_COLORREF(m_TextColor));

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

	LRESULT OnCtlColorStatic(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
        HDC hdcStatic = (HDC) wParam;
		HWND hWnd = (HWND)lParam;

		if (nullptr != m_hBackground && ::GetDlgCtrlID(hWnd) == IDC_PLACEHOLDERLABEL)
		{
			SetBkMode(hdcStatic, TRANSPARENT);
			SetTextColor(hdcStatic, VS_RGBA_TO_COLORREF(m_TextColor));
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
		CComPtr<IUIHostLocale> spUIHostLocale;
		HRESULT hr = GetVsSiteCache().QueryCachedService<IUIHostLocale, SID_SUIHostLocale>(&spUIHostLocale);
		VSL_CHECKHRESULT(hr);
		CLogFont dialogFont;
		spUIHostLocale->GetDialogFont(&dialogFont);

		SetWindowFont(GetHWND(), dialogFont.CreateFontIndirect(), TRUE);

		RECT emptyRect = { 0,0,0,0 };

		m_TreeView.Create(GetHWND(), emptyRect, U("Tree View"),
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
			TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_DISABLEDRAGDROP | TVS_SHOWSELALWAYS,
			0, IDC_TREEVIEW, 0);
		VSL_CHECKBOOLEAN(m_TreeView.m_hWnd != NULL, E_UNEXPECTED);

		CComBSTR placeholderText;
		VSL_CHECKBOOLEAN_GLE(placeholderText.LoadString(_AtlBaseModule.GetResourceInstance(),
			IDS_E_NOTCONNECTED));

		m_PlaceholderLabel.Create(GetHWND(), emptyRect, placeholderText,
			WS_CHILD | WS_VISIBLE | SS_CENTER,
			0, IDC_PLACEHOLDERLABEL, 0);
		VSL_CHECKBOOLEAN(m_PlaceholderLabel.m_hWnd != NULL, E_UNEXPECTED);

		m_PlaceholderLabel.SetFont(dialogFont.CreateFontIndirect());

		BOOL bReloaded;
		OnZohoReload(WM_ZOHO_RELOAD, NULL, NULL, bReloaded);

		return FALSE;
	}

	LRESULT OnZohoReload(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		ZohoClient* zoho = GetZohoClient();
		if (zoho->IsConnected())
		{
			m_TreeView.ShowWindow(SW_SHOW);
			m_PlaceholderLabel.ShowWindow(SW_HIDE);
		}
		else
		{
			m_TreeView.ShowWindow(SW_HIDE);
			m_PlaceholderLabel.ShowWindow(SW_SHOW);
		}

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
		m_ClientBorder = *pbw;
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

protected:
	ZohoClient* GetZohoClient()
	{
		IVsPackage *pPackage;
		CComPtr<IVsShell> spShell = GetVsSiteCache().GetCachedService<IVsShell, SID_SVsShell>();
		VSL_CHECKHRESULT(spShell->IsPackageLoaded(guidZohoProjectsPkg, &pPackage));
		CComQIPtr<IZohoProjectsPackage> pZohoPackage(pPackage);
		VSL_CHECKBOOL(!!pZohoPackage, E_UNEXPECTED);

		VARIANT vZohoClient;
		VSL_CHECKHRESULT(pZohoPackage.p->GetClient(&vZohoClient));
		return (ZohoClient*)vZohoClient.pvRecord;
	}

private:
	// Initialize colors that are used to render the Window Pane.
	void InitVSColors()
	{
		// Obtain IVsUIShell2 from IVsUIShell
		CComQIPtr<IVsUIShell2> spIVsUIShell2(GetVsSiteCache().GetCachedService<IVsUIShell, SID_SVsUIShell>());

		if (nullptr != m_hBackground)
		{
			::DeleteBrush(m_hBackground);
			m_hBackground = nullptr;
		}

		if (nullptr != spIVsUIShell2)
		{
			if (SUCCEEDED(spIVsUIShell2->GetVSSysColorEx(VSCOLOR_WINDOW, &m_BkColor)))
			{
				COLORREF crBackground = VS_RGBA_TO_COLORREF(m_BkColor);
				m_hBackground = ::CreateSolidBrush(crBackground);
			}

			spIVsUIShell2->GetVSSysColorEx(VSCOLOR_TOOLWINDOW_TEXT, &m_TextColor);
		}
		
		if (::IsWindow(this->m_hWnd))
		{
			::InvalidateRect(this->m_hWnd, nullptr /* lpRect */, TRUE /* bErase */);
		}
	}

	RECT m_ClientBorder;
	VS_RGBA m_BkColor, m_TextColor;
	HBRUSH m_hBackground;
	VSCOOKIE m_BroadcastCookie;
	CContainedWindowT<CTreeViewCtrl> m_TreeView;
	CContainedWindowT<CStatic> m_PlaceholderLabel;
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

	void Reload()
	{
		auto pane = static_cast<ProjectExplorerPane*>(
						static_cast<IVsWindowPane*>(
							&*m_spView));
		pane->SendMessage(WM_ZOHO_RELOAD, NULL, NULL);
	}

private:
	CComPtr<IUnknown> m_spView;
	CComPtr<IVsToolWindowToolbarHost> m_ToolbarHost;
};
