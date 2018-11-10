#include "pch.h"

#include "../ZohoProjects/OAuth2.h"
#include "../ZohoProjects/Zoho.h"
#include "cpprest/details/basic_types.h"

TEST(AuthenticationCode, OAuth) {
	oauth2::session sess(
		utility::conversions::to_string_t(zoho::client_id),
		utility::conversions::to_string_t(zoho::client_secret),
		utility::conversions::to_string_t(zoho::authorization_url()),
		utility::conversions::to_string_t(zoho::token_url()),
		utility::conversions::to_string_t(zoho::redirect_url));
	sess.run();
}