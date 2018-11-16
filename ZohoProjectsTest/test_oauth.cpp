#include "stdafx.h"
#include "CppUnitTest.h"

#include "../ZohoProjectsApi/oauth.h"
#include "../ZohoProjectsApi/zoho.h"
#include "../ZohoProjectsApi/api.h"
#include "cpprest/http_client.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZohoProjects
{
	namespace Test
	{
		TEST_CLASS(OAuth)
		{
		public:
			TEST_METHOD(Code_Authentication) {
				oauth2::session sess(
					utility::conversions::to_string_t(zoho::api::oauth::client_id),
					utility::conversions::to_string_t(zoho::api::oauth::client_secret),
					utility::conversions::to_string_t(zoho::api::oauth::authorization_url()),
					utility::conversions::to_string_t(zoho::api::oauth::token_url()),
					utility::conversions::to_string_t(zoho::api::oauth::redirect_url));
				web::http::client::http_client_config http_config = sess.get();
				Assert::IsTrue(http_config.oauth2()->is_enabled());
			}

			TEST_METHOD(Code_Refresh) {
				oauth2::session sess(
					utility::conversions::to_string_t(zoho::api::oauth::client_id),
					utility::conversions::to_string_t(zoho::api::oauth::client_secret),
					utility::conversions::to_string_t(zoho::api::oauth::authorization_url()),
					utility::conversions::to_string_t(zoho::api::oauth::token_url()),
					utility::conversions::to_string_t(zoho::api::oauth::redirect_url));
				sess.get();
				sess.expire();
				web::http::client::http_client_config http_config = sess.get();
				Assert::IsTrue(http_config.oauth2()->is_enabled());
			}
		};
	};
};
