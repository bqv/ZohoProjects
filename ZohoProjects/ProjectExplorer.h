//
// ProjectExplorer.h
//
// This file contains the implementation of a tool window that hosts a .NET user control
//

#pragma once

#include <initguid.h>
#include <AtlWin.h>
#include <VSLWindows.h>

#include "..\ZohoProjectsUI\Resource.h"

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
	public IVsToolWindowToolbar
{
	VSL_DECLARE_NOT_COPYABLE(ProjectExplorerPane)

protected:

	// Protected constructor called by CComObject<ProjectExplorerPane>::CreateInstance.
	ProjectExplorerPane() :
		VsWindowPaneFromResource(),
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
END_COM_MAP()

BEGIN_MSG_MAP(ProjectExplorerPane)
	MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDlg)
END_MSG_MAP()

	void InitToolbar()
	{
		CComPtr<IVsUIShell> spUIShell;
		VSL_CHECKHRESULT(GetVsSiteCache().QueryService(SID_SVsUIShell, &spUIShell));
		VSL_CHECKHRESULT(spUIShell->SetupToolbar(GetHWND(), (IVsToolWindowToolbar*)this, &m_ToolbarHost));
		VSL_CHECKBOOLEAN(m_ToolbarHost != nullptr, E_UNEXPECTED);
		VSL_CHECKHRESULT(m_ToolbarHost->AddToolbar(VSTWT_TOP, &CLSID_ZohoProjectExplorerCmdSet, zohoProjectExplorerToolbar));
	}

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
	CComPtr<IVsToolWindowToolbarHost> m_ToolbarHost;
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
				strCaption.LoadStringW(_AtlBaseModule.GetResourceInstance(), IDS_WINDOW_TITLE));
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

		static_cast<ProjectExplorerPane*>(
			static_cast<IVsWindowPane*>(
				static_cast<IUnknown*>(
					m_spView)))->InitToolbar();
	}

private:
	CComPtr<IUnknown> m_spView;
};
