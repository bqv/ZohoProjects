#pragma once
#include "stdafx.h"

#include <string>
#include <vector>
#include <ctime>
#include <memory>

#include "lib.h"
#include "oauth.h"
#include "types.h"

namespace zoho
{
	class DLLEXPORT credential : public web::http::oauth2::experimental::oauth2_token
	{
	public:
		credential(utility::string_t&, utility::string_t&, utility::string_t&, utility::string_t&, int64_t);

	private:
		credential(const web::http::oauth2::experimental::oauth2_token&);

		friend class session;
	};

	class DLLEXPORT session : protected oauth2::session
	{
	private:
		void callback(const web::http::oauth2::experimental::oauth2_token&) final;

	protected:
		virtual void callback(const credential);

	public:
		session();

		web::http::client::http_client client(std::string);
		void set(const web::http::oauth2::experimental::oauth2_token&);
		explicit operator bool() const;
	};

	class DLLEXPORT codes
	{
	private:
		codes() = delete;

	public:
		static bool is_success(web::http::status_code);
	};
}
