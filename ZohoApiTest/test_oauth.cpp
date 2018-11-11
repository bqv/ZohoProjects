#include "pch.h"

#include "../ZohoApi/oauth.h"
#include "../ZohoApi/zoho.h"
#include "cpprest/http_client.h"

TEST(OAuth, Code_Authentication) {
	oauth2::session sess(
		utility::conversions::to_string_t(zoho::api::oauth::client_id),
		utility::conversions::to_string_t(zoho::api::oauth::client_secret),
		utility::conversions::to_string_t(zoho::api::oauth::authorization_url()),
		utility::conversions::to_string_t(zoho::api::oauth::token_url()),
		utility::conversions::to_string_t(zoho::api::oauth::redirect_url));
	web::http::client::http_client_config http_config = sess.get();
	ASSERT_TRUE(http_config.oauth2()->is_enabled());
}

TEST(OAuth, Code_Refresh) {
	oauth2::session sess(
		utility::conversions::to_string_t(zoho::api::oauth::client_id),
		utility::conversions::to_string_t(zoho::api::oauth::client_secret),
		utility::conversions::to_string_t(zoho::api::oauth::authorization_url()),
		utility::conversions::to_string_t(zoho::api::oauth::token_url()),
		utility::conversions::to_string_t(zoho::api::oauth::redirect_url));
	sess.get();
	sess.expire();
	web::http::client::http_client_config http_config = sess.get();
	ASSERT_TRUE(http_config.oauth2()->is_enabled());
}
