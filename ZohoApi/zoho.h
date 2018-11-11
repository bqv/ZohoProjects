#pragma once

#include <string>
#include <vector>
#include <ctime>

#include "oauth.h"
#include "types.h"

namespace zoho
{
	class session : private oauth2::session
	{
	public:
		session();

		web::http::client::http_client client(std::string endpoint);
	};

	class codes
	{
	private:
		codes() = delete;

	public:
		static bool is_success(web::http::status_code);
	};
}
