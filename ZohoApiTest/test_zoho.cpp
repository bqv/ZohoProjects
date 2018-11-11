#include "pch.h"

#include "../ZohoApi/zoho.h"

TEST(Zoho, Portals_API_Call) {
	zoho::session session;
	std::vector<zoho::portal> portals = zoho::api::portals::all_portals(session);
	ASSERT_TRUE(portals.size() > 0);
}
