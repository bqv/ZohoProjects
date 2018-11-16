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
		utility::conversions::to_string_t(api::oauth::client_id),
		utility::conversions::to_string_t(api::oauth::client_secret),
		utility::conversions::to_string_t(api::oauth::authorization_url()),
		utility::conversions::to_string_t(api::oauth::token_url()),
		utility::conversions::to_string_t(api::oauth::redirect_url))
	{
	}

	web::http::client::http_client session::client(std::string p_endpoint)
	{
		web::uri uri(utility::conversions::to_string_t(p_endpoint));
		web::http::client::http_client client(uri, get());
		return client;
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
