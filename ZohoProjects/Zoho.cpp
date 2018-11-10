#include "stdafx.h"
#include "Zoho.h"

#include <vector>
#include <sstream>
#include <numeric>

const std::string zoho::base_url = "https://projectsapi.zoho.com/restapi";
const std::string zoho::client_id = "1000.0ESNDLDTNIT733894WBY544KIGZAEA";
const std::string zoho::client_secret = "15300e4a0f75ec7f400e7442c3bb5ccd250e1e2a8b";
const std::string zoho::scopes[] = {
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
const std::string zoho::redirect_url = "http://localhost:8702";
const std::string zoho::accounts_url = "https://accounts.zoho.com";

zoho::zoho()
{
}

std::string zoho::authorization_url()
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

std::string zoho::token_url()
{
	std::ostringstream oss;
	oss << "https://accounts.zoho.com/oauth/v2/token";
	return oss.str();
}
