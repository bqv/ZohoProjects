#pragma once

#include <mutex>

#include "lib.h"
#include "cpprest/http_client.h"

#if defined(_WIN32) && !defined(__cplusplus_winrt)
// Extra includes for Windows desktop.
#include <windows.h>
#include <Shellapi.h>
#endif

#include "cpprest/http_listener.h"
#include "cpprest/http_client.h"

namespace oauth2
{
	class DLLEXPORT codelistener
	{
	private:
		std::unique_ptr<web::http::experimental::listener::http_listener> m_listener;
		pplx::task_completion_event<bool> m_tce;
		web::http::oauth2::experimental::oauth2_config& m_config;
		std::mutex m_resplock;

	public:
		codelistener(web::uri, web::http::oauth2::experimental::oauth2_config&);
		~codelistener();

		pplx::task<bool> wait_code();
	};

	class DLLEXPORT session
	{
	private:
		std::unique_ptr<codelistener> m_listener;
		web::http::client::http_client_config m_http_config;
		web::http::oauth2::experimental::oauth2_config m_oauth_config;
		pplx::task_completion_event<bool> m_tce;

		void open_browser_auth();
		pplx::task<bool> authorization_code_flow();
		pplx::task<bool> refresh_flow();

	public:
		session(utility::string_t, utility::string_t, utility::string_t, utility::string_t, utility::string_t);

		web::http::client::http_client_config get();
		void expire();
	};
}
