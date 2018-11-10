#include "pch.h"

#include "../ZohoApi/oauth.h"
#include "../ZohoApi/zoho.h"
#include "cpprest/details/basic_types.h"

TEST(Authentication_Code, OAuth) {
	oauth2::session sess(
		utility::conversions::to_string_t(zoho::client_id),
		utility::conversions::to_string_t(zoho::client_secret),
		utility::conversions::to_string_t(zoho::authorization_url()),
		utility::conversions::to_string_t(zoho::token_url()),
		utility::conversions::to_string_t(zoho::redirect_url));
	sess.run();
}