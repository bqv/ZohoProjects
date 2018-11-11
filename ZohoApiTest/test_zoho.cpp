#include "pch.h"

#include "../ZohoApi/zoho.h"
#include "../ZohoApi/api.h"

TEST(Zoho, Portals_API_Calls) {
	zoho::session session;
	std::vector<zoho::portal> portals = zoho::api::portals::all_portals(session);
	ASSERT_TRUE(portals.size() > 0);
}

TEST(Zoho, Projects_API_Calls) {
	zoho::session session;
	std::vector<zoho::portal> portals = zoho::api::portals::all_portals(session);
	ASSERT_TRUE(portals.size() > 0);
	std::vector<zoho::project> projects = zoho::api::projects::all_projects(session, portals[0]);
	ASSERT_TRUE(projects.size() > 0);
}
