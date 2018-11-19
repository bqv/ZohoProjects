#include "stdafx.h"
#include "CppUnitTest.h"

#include "../ZohoProjectsApi/oauth.h"
#include "../ZohoProjectsApi/zoho.h"
#include "../ZohoProjectsApi/util.h"
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
					util::string(zoho::api::oauth::client_id()),
					util::string(zoho::api::oauth::client_secret()),
					util::string(zoho::api::oauth::authorization_url()),
					util::string(zoho::api::oauth::token_url()),
					util::string(zoho::api::oauth::redirect_url()));
				web::http::client::http_client_config http_config = sess.get();
				Assert::IsTrue(http_config.oauth2()->is_enabled());
			}

			TEST_METHOD(Code_Refresh) {
				oauth2::session sess(
					util::string(zoho::api::oauth::client_id()),
					util::string(zoho::api::oauth::client_secret()),
					util::string(zoho::api::oauth::authorization_url()),
					util::string(zoho::api::oauth::token_url()),
					util::string(zoho::api::oauth::redirect_url()));
				sess.get();
				sess.expire();
				web::http::client::http_client_config http_config = sess.get();
				Assert::IsTrue(http_config.oauth2()->is_enabled());
			}
		};
	};
};
