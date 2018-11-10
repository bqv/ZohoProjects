#include "pch.h"

#include "../ZohoProjects/OAuth2.h"
#include "../ZohoProjects/ZohoProjects.h"
#include "cpprest/details/basic_types.h"

TEST(AuthenticationCode, OAuth) {
	oauth2::session sess(
		utility::conversions::to_string_t(ZohoProjects::clientId),
		utility::conversions::to_string_t(ZohoProjects::clientSecret),
		utility::conversions::to_string_t(ZohoProjects::authorizationUrl()),
		utility::conversions::to_string_t(ZohoProjects::tokenUrl()),
		utility::conversions::to_string_t(ZohoProjects::redirectUrl));
	sess.run();
}