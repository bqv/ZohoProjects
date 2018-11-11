#include "stdafx.h"
#include "oauth.h"

#include "zoho.h"

namespace oauth2
{
	codelistener::codelistener(web::uri listen_uri, web::http::oauth2::experimental::oauth2_config& config)
		: m_listener(new web::http::experimental::listener::http_listener(listen_uri)), m_config(config)
	{
		m_listener->support([this](web::http::http_request request) -> void
        {
            if (request.request_uri().path() == U("/") && request.request_uri().query() != U(""))
            {
                m_resplock.lock();

                m_config.token_from_redirected_uri(request.request_uri()).then([this,request](pplx::task<void> token_task) -> void
                {
                    try
                    {
                        token_task.wait();
                        m_tce.set(true);
                    }
                    catch (const web::http::oauth2::experimental::oauth2_exception& e)
                    {
                        ucout << "Error: " << e.what() << std::endl;
                        m_tce.set(false);
                    }
                });

				web::http::http_response response(web::http::status_codes::TemporaryRedirect);
				response.headers().add(U("Location"), utility::conversions::to_string_t(zoho::api::oauth::landing_url));
				request.reply(response);

                m_resplock.unlock();
            }
            else
            {
				web::http::http_response response(web::http::status_codes::NotFound);
				response.headers().set_content_type(U("text/html"));
				response.set_body(U("<script type='text/javascript'>window.open('','_self').close();</script>"));
				request.reply(response);
            }
        });

        m_listener->open().wait();
	}
	
    pplx::task<bool> codelistener::wait_code()
    {
        return pplx::create_task(m_tce);
    }

	codelistener::~codelistener()
	{
		m_listener->close().wait();
	}

	session::session(
		utility::string_t client_key,
		utility::string_t client_secret,
		utility::string_t auth_endpoint,
		utility::string_t token_endpoint,
		utility::string_t redirect_uri
	) : m_oauth_config(client_key, client_secret, auth_endpoint, token_endpoint, redirect_uri),
		m_listener(new codelistener(redirect_uri, m_oauth_config))
	{
		m_oauth_config.set_bearer_auth(true);
		m_oauth_config.set_http_basic_auth(false);
	}

	web::http::client::http_client_config session::get()
	{
		if (!m_oauth_config.token().is_valid_access_token() || m_oauth_config.token().expires_in() <= 0)
		{
			if (!m_oauth_config.token().refresh_token().empty() && refresh_flow().get())
			{
				m_http_config.set_oauth2(m_oauth_config);
				return m_http_config;
			}
			else if (authorization_code_flow().get())
			{
				m_http_config.set_oauth2(m_oauth_config);
				return m_http_config;
			}
			else
			{
				throw new std::exception("Authorization Failed");
			}
		}
		else
		{
			return m_http_config;
		}
	}

	void session::open_browser_auth()
	{
		auto auth_uri(m_oauth_config.build_authorization_uri(true));
#if defined(_WIN32) && !defined(__cplusplus_winrt)
		// NOTE: Windows desktop only.
		auto r = ShellExecuteA(NULL, "open", utility::conversions::utf16_to_utf8(auth_uri).c_str(), NULL, NULL, SW_SHOWNORMAL);
#elif defined(__APPLE__)
		// NOTE: OS X only.
		string_t browser_cmd(U("open \"") + auth_uri + U("\""));
		(void)system(browser_cmd.c_str());
#else
		// NOTE: Linux/X11 only.
		string_t browser_cmd(U("xdg-open \"") + auth_uri + U("\""));
		(void)system(browser_cmd.c_str());
#endif
	}

	pplx::task<bool> session::authorization_code_flow()
	{
		open_browser_auth();
		return m_listener->wait_code();
	}

	pplx::task<bool> session::refresh_flow()
	{
		m_oauth_config.token_from_refresh().then([this](pplx::task<void> token_task) -> void
		{
			try
			{
				token_task.wait();
				m_tce.set(true);
			}
			catch (const web::http::oauth2::experimental::oauth2_exception& e)
			{
				ucout << "Error: " << e.what() << std::endl;
				m_tce.set(false);
			}
		});
        return pplx::create_task(m_tce);
	}

	void session::expire()
	{
		web::http::oauth2::experimental::oauth2_token token(m_oauth_config.token());
		token.set_expires_in(0);
		m_oauth_config.set_token(token);
		m_http_config.set_oauth2(m_oauth_config);
	}
}
