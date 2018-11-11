#include "stdafx.h"
#include "zoho.h"

#include <vector>
#include <sstream>
#include <numeric>

#include "cpprest/http_client.h"

namespace zoho
{
	session::session() : oauth2::session(
		utility::conversions::to_string_t(api::oauth::client_id),
		utility::conversions::to_string_t(api::oauth::client_secret),
		utility::conversions::to_string_t(api::oauth::authorization_url()),
		utility::conversions::to_string_t(api::oauth::token_url()),
		utility::conversions::to_string_t(api::oauth::redirect_url))
	{
	}

	web::http::client::http_client session::client(std::string p_endpoint)
	{
		web::uri uri(utility::conversions::to_string_t(p_endpoint));
		web::http::client::http_client client(uri, get());
		return client;
	}

	bool codes::is_success(web::http::status_code code)
	{
		switch (code)
		{
		case web::http::status_codes::OK:
		case web::http::status_codes::Created:
		case web::http::status_codes::NoContent:
			return true;

		default:
			return false;
		}
	}

	namespace api
	{
		exception::exception(const std::exception& p_inner, const std::string& p_message)
			: inner(p_inner)
			, message(std::string("Zoho API Exception: ") + p_message)
		{
		}

		const char* exception::what() const throw ()
		{
			return message.c_str();
		}

		const std::string base_url = "https://projectsapi.zoho.com/restapi";
		const std::string oauth::client_id = "1000.0ESNDLDTNIT733894WBY544KIGZAEA";
		const std::string oauth::client_secret = "15300e4a0f75ec7f400e7442c3bb5ccd250e1e2a8b";
		const std::string oauth::scopes[] = {
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
		const std::string oauth::redirect_url = "http://localhost:8702";
		const std::string oauth::landing_url = "https://accounts.zoho.com";

		std::string oauth::authorization_url()
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

		std::string oauth::token_url()
		{
			std::ostringstream oss;
			oss << "https://accounts.zoho.com/oauth/v2/token";
			return oss.str();
		}

		inline std::string portals::portals_endpoint()
		{
			return base_url + "/portals/";
		}

		std::vector<portal> portals::all_portals(session& p_sess)
		{
			std::vector<portal> portals;
			web::http::client::http_client client = p_sess.client(portals_endpoint());
			try
			{
				web::http::http_response response = client.request(web::http::methods::GET).get();
				web::json::value json = response.extract_json().get();
				OutputDebugString(json.serialize().c_str());
				if (codes::is_success(response.status_code()))
				{
					web::json::object object = json.as_object();
					for (web::json::value item : json.as_object()[U("portals")].as_array())
					{
						portals.emplace_back(item);
					}
					return portals;
				}
				else
				{
					throw std::runtime_error(utility::conversions::to_utf8string(json.serialize()).c_str());
				}
			}
			catch (const std::exception& e)
			{
				throw api::exception(e, e.what());
			}
		}

		inline std::string projects::projects_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/";
		}

		inline std::string projects::project_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + '/';
		}

		inline std::string projects::customfields_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/customfields/";
		}

		inline std::string projects::groups_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/groups";
		}

		inline std::string dashboard::activities_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/activities/";
		}

		inline std::string dashboard::statuses_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/statuses/";
		}

		inline std::string milestones::milestones_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/milestones/";
		}

		inline std::string milestones::milestone_endpoint(portal& p_portal, project& p_project, milestone& p_milestone)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/milestones/" + p_milestone.id_str() + '/';
		}

		inline std::string milestones::status_endpoint(portal& p_portal, project& p_project, milestone& p_milestone)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/milestones/" + p_milestone.id_str() + "/status/";
		}

		inline std::string tasklists::tasklists_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/tasklists/";
		}

		inline std::string tasklists::tasklist_endpoint(portal& p_portal, project& p_project, tasklist& p_tasklist)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/tasklists/" + p_tasklist.id_str() + '/';
		}

		inline std::string tasks::tasks_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/tasks/";
		}

		inline std::string tasks::tasklist_tasks_endpoint(portal& p_portal, project& p_project, tasklist& p_tasklist)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/tasklists/" + p_tasklist.id_str() + "/tasks/";
		}

		inline std::string tasks::task_endpoint(portal& p_portal, project& p_project, task& p_task)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/tasks/" + p_task.id_str() + '/';
		}

		inline std::string tasks::reorder_endpoint(portal& p_portal, project& p_project, task& p_task)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/tasks/" + p_task.id_str() + "/reorder";
		}

		inline std::string tasks::subtasks_endpoint(portal& p_portal, project& p_project, task& p_task)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/tasks/" + p_task.id_str() + "/subtasks/";
		}

		inline std::string tasks::my_tasks_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/mytasks/";
		}

		inline std::string tasks::comments_endpoint(portal& p_portal, project& p_project, task& p_task)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/tasks/" + p_task.id_str() + "/comments/";
		}

		inline std::string tasks::comment_endpoint(portal& p_portal, project& p_project, task& p_task, comment<task>& p_comment)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/tasks/" + p_task.id_str() + "/comments/" + p_comment.id_str() + '/';
		}

		inline std::string tasks::attachments_endpoint(portal& p_portal, project& p_project, task& p_task)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/tasks/" + p_task.id_str() + "/attachments/";
		}

		inline std::string tasks::attachment_endpoint(portal& p_portal, project& p_project, task& p_task, attachment<task>& p_attachment)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/tasks/" + p_task.id_str() + "/attachments/" + p_attachment.id_str() + '/';
		}

		inline std::string tasks::layouts_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/tasklayouts/";
		}

		inline std::string tasks::layouts_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/tasklayouts/";
		}

		inline std::string timesheets::project_timelogs_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/logs/";
		}

		inline std::string timesheets::my_timelogs_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/logs/";
		}

		inline std::string timesheets::task_timelogs_endpoint(portal& p_portal, project& p_project, task& p_task)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/tasks/" + p_task.id_str() + "/logs/";
		}

		inline std::string timesheets::approve_task_timelogs_endpoint(portal& p_portal, project& p_project, task& p_task, log<task>& p_log)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/tasks/" + p_task.id_str() + "/logs/" + p_log.id_str() + "/approval";
		}

		inline std::string timesheets::task_timelog_endpoint(portal& p_portal, project& p_project, task& p_task, log<task>& p_log)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/tasks/" + p_task.id_str() + "/logs/" + p_log.id_str() + '/';
		}

		inline std::string timesheets::bug_timelogs_endpoint(portal& p_portal, project& p_project, bug& p_bug)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/bugs/" + p_bug.id_str() + "/logs/";
		}

		inline std::string timesheets::approve_bug_timelogs_endpoint(portal& p_portal, project& p_project, bug& p_bug, log<bug>& p_log)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/bugs/" + p_bug.id_str() + "/logs/" + p_log.id_str() + "/approval";
		}

		inline std::string timesheets::bug_timelog_endpoint(portal& p_portal, project& p_project, bug& p_bug, log<bug>& p_log)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/bugs/" + p_bug.id_str() + "/logs/" + p_log.id_str() + '/';
		}

		inline std::string timesheets::general_timelogs_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/logs/";
		}

		inline std::string timesheets::approve_general_timelogs_endpoint(portal& p_portal, project& p_project, log<project>& p_log)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/logs/" + p_log.id_str() + "/approval";
		}

		inline std::string timesheets::general_timelog_endpoint(portal& p_portal, project& p_project, log<project>& p_log)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/logs/" + p_log.id_str() + '/';
		}

		inline std::string bugs::bugs_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/bugs/";
		}

		inline std::string bugs::bug_endpoint(portal& p_portal, project& p_project, bug& p_bug)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/bugs/" + p_bug.id_str() + '/';
		}

		inline std::string bugs::comments_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/bugs/comments/";
		}

		inline std::string bugs::comments_endpoint(portal& p_portal, project& p_project, bug& p_bug)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/bugs/" + p_bug.id_str() + "/comments/";
		}

		inline std::string bugs::comment_endpoint(portal& p_portal, project& p_project, bug& p_bug, comment<bug>& p_comment)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/bugs/" + p_bug.id_str() + "/comments/" + p_comment.id_str();
		}

		inline std::string bugs::timer_endpoint(portal& p_portal, project& p_project, bug& p_bug)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/bugs/" + p_bug.id_str() + "/timer";
		}

		inline std::string bugs::customviews_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/bugs/customviews/";
		}

		inline std::string bugs::attachments_endpoint(portal& p_portal, project& p_project, bug& p_bug)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/bugs/" + p_bug.id_str() + "/attachments/";
		}

		inline std::string bugs::resolution_endpoint(portal& p_portal, project& p_project, bug& p_bug)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/bugs/" + p_bug.id_str() + "/resolution/";
		}

		inline std::string bugs::followers_endpoint(portal& p_portal, project& p_project, bug& p_bug)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/bugs/" + p_bug.id_str() + "/bugfollowers/";
		}

		inline std::string bugs::defaultfields_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/bugs/defaultfields/";
		}

		inline std::string bugs::customfields_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/bugs/customfields/";
		}

		inline std::string bugs::activities_endpoint(portal& p_portal, project& p_project, bug& p_bug)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/bugs/" + p_bug.id_str() + "/activities/";
		}

		inline std::string bugs::renamedfields_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/bugs/renamedfields/";
		}

		inline std::string bugs::my_bugs_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/mybugs/";
		}

		inline std::string events::events_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/events/";
		}

		inline std::string events::event_endpoint(portal& p_portal, project& p_project, event& p_event)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/events/" + p_event.id_str() + '/';
		}

		inline std::string forums::forums_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/forums/";
		}

		inline std::string forums::forum_endpoint(portal& p_portal, project& p_project, forum& p_forum)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/forums/" + p_forum.id_str() + '/';
		}

		inline std::string forums::follow_endpoint(portal& p_portal, project& p_project, forum& p_forum)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/forums/" + p_forum.id_str() + "/follow";
		}

		inline std::string forums::unfollow_endpoint(portal& p_portal, project& p_project, forum& p_forum)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/forums/" + p_forum.id_str() + "/unfollow";
		}

		inline std::string forums::categories_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/categories/";
		}

		inline std::string forums::comments_endpoint(portal& p_portal, project& p_project, forum& p_forum)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/forums/" + p_forum.id_str() + "/comments/";
		}

		inline std::string forums::comment_endpoint(portal& p_portal, project& p_project, forum& p_forum, comment<forum>& p_comment)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/forums/" + p_forum.id_str() + "/comments/" + p_comment.id_str() + '/';
		}

		inline std::string forums::bestanswer_endpoint(portal& p_portal, project& p_project, forum& p_forum, comment<forum>& p_comment)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/forums/" + p_forum.id_str() + "/comments/" + p_comment.id_str() + "/markbestanswer";
		}

		inline std::string forums::category_endpoint(portal& p_portal, project& p_project, category& p_category)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/categories/" + p_category.id_str() + '/';
		}

		inline std::string users::users_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/users/";
		}

		inline std::string users::user_endpoint(portal& p_portal, project& p_project, user& p_user)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/users/" + p_user.id_str() + '/';
		}

		inline std::string users::avail_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/users/availcount/";
		}

		inline std::string users::users_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/users/";
		}

		inline std::string users::activation_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/users/activate/";
		}

		inline std::string users::company_endpoint(portal& p_portal, company& p_company)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/clients/" + p_company.id_str() + "/users";
		}
	}

	entity::entity(const int p_id)
		: id(p_id)
	{
	}

	portal::portal(const web::json::value& p_json)
		: entity::entity(JSON_GET_INTEGER(p_json, "id", -1))
		, name(JSON_GET_STRING(p_json, "name", ""))
		, is_default(JSON_GET_BOOL(p_json, "default", false))
		, gmt_time_zone(JSON_GET_STRING(p_json, "gmt_time_zone", ""))
		, role(JSON_GET_STRING(p_json, "role", ""))
		, project_count({
			JSON_GET_GET_INTEGER(p_json, "project_count", "template", 0),
			JSON_GET_GET_INTEGER(p_json, "project_count", "archived", 0),
			JSON_GET_GET_INTEGER(p_json, "project_count", "active", 0),
		  })
		, settings({
			JSON_GET_GET_STRING(p_json, "settings", "company_name", ""),
			JSON_GET_GET_STRING(p_json, "settings", "website_url", ""),
			JSON_GET_GET_STRING(p_json, "settings", "time_zone", ""),
			JSON_GET_GET_STRING(p_json, "settings", "date_format", ""),
		  })
		, locale({
			JSON_GET_GET_STRING(p_json, "locale", "code", ""),
			JSON_GET_GET_STRING(p_json, "locale", "language", ""),
			JSON_GET_GET_STRING(p_json, "locale", "country", ""),
		  })
		, link({{
			JSON_GET_GET_GET_STRING(p_json, "link", "project", "url", ""),
		  }})
	{
	}
}
