#include "stdafx.h"
#include "zoho.h"

#include <vector>
#include <sstream>
#include <numeric>

#include "api.h"
#include "cpprest/http_client.h"

namespace zoho
{
	session::session() : oauth2::session(
		util::string(api::oauth::client_id()),
		util::string(api::oauth::client_secret()),
		util::string(api::oauth::authorization_url()),
		util::string(api::oauth::token_url()),
		util::string(api::oauth::redirect_url()))
	{
	}

	web::http::client::http_client session::client(std::string p_endpoint)
	{
		web::uri uri(util::string(p_endpoint));
		web::http::client::http_client client(uri, get());
		return client;
	}

	void session::callback(const web::http::oauth2::experimental::oauth2_token& p_token)
	{
		callback(credential(p_token));
	}

	void session::callback(const credential p_cred)
	{
	}

	void session::set(const web::http::oauth2::experimental::oauth2_token& p_token)
	{
		m_oauth_config.set_token(p_token);
		m_http_config.set_oauth2(m_oauth_config);
	}

	session::operator bool() const
	{
		const web::http::oauth2::experimental::oauth2_token& token = m_oauth_config.token();
		return token.is_valid_access_token() && token.expires_in() > 0;
	}

	credential::credential(
		utility::string_t& p_access_token,
		utility::string_t& p_refresh_token,
		utility::string_t& p_token_type,
		utility::string_t& p_scope,
		int64_t p_expires_in)
		: oauth2_token(p_access_token)
	{
		set_refresh_token(p_refresh_token);
		set_token_type(p_token_type);
    	set_scope(p_scope);
    	set_expires_in(p_expires_in);
	}

	credential::credential(const web::http::oauth2::experimental::oauth2_token& p_config)
		: oauth2_token(p_config)
	{
	}

	bool codes::is_success(web::http::status_code p_code)
	{
		switch (p_code)
		{
		case web::http::status_codes::OK:
		case web::http::status_codes::Created:
		case web::http::status_codes::NoContent:
			return true;

		default:
			return false;
		}
	}
}
