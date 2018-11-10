#include "stdafx.h"
#include "ZohoProjects.h"

#include <vector>
#include <sstream>
#include <numeric>

const std::string ZohoProjects::baseUrl = "https://projectsapi.zoho.com/restapi";
const std::string ZohoProjects::clientId = "1000.0ESNDLDTNIT733894WBY544KIGZAEA";
const std::string ZohoProjects::clientSecret = "15300e4a0f75ec7f400e7442c3bb5ccd250e1e2a8b";
const std::string ZohoProjects::scopes[] = {
	"ZohoProjects.portals.READ",
	"ZohoProjects.projects.ALL",
	"ZohoProjects.activities.READ",
	"ZohoProjects.milestones.ALL",
	"ZohoProjects.tasklists.ALL",
	"ZohoProjects.tasks.ALL",
	"ZohoProjects.timesheets.ALL",
	"ZohoProjects.bugs.ALL",
	"ZohoProjects.events.ALL",
	"ZohoProjects.forums.ALL",
	"ZohoProjects.users.ALL",
};
const std::string ZohoProjects::redirectUrl = "http://localhost:8702";
const std::string ZohoProjects::zohoUrl = "https://accounts.zoho.com";

ZohoProjects::ZohoProjects()
{
}

std::string ZohoProjects::authorizationUrl()
{
	std::ostringstream oss;
	oss << "https://accounts.zoho.com/oauth/v2/auth";
	oss << "?scope=" << std::accumulate(std::begin(scopes), std::end(scopes), std::string(),
		[](const std::string& a, const std::string& b) -> std::string {
			return a + (a.length() > 0 ? "," : "") + b;
		});
	oss << "&access_type=" << "offline";
	oss << "&prompt=" << "consent";
	return oss.str();
}

std::string ZohoProjects::tokenUrl()
{
	std::ostringstream oss;
	oss << "https://accounts.zoho.com/oauth/v2/token";
	return oss.str();
}
