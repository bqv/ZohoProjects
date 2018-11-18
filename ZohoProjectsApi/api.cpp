#include "stdafx.h"
#include "api.h"

#include <vector>
#include <sstream>
#include <numeric>
#include <iomanip>
#include <ctime>

#include "zoho.h"
#include "types.h"
#include "cpprest/http_client.h"

namespace zoho
{
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

		const char* oauth::authorization_url()
		{
			static std::string s_authorization_url;

			const std::string scopes[] = {
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

			std::ostringstream oss;
			oss << "https://accounts.zoho.com/oauth/v2/auth";
			oss << "?scope=" << std::accumulate(std::begin(scopes), std::end(scopes), std::string(),
				[](const std::string& a, const std::string& b) -> std::string {
				return a + (a.length() > 0 ? "," : "") + b;
			});
			oss << "&access_type=" << "offline";
			oss << "&prompt=" << "consent";
			s_authorization_url = oss.str();
			return s_authorization_url.c_str();
		}

		const char* oauth::token_url()
		{
			static std::string s_token_url;

			std::ostringstream oss;
			oss << "https://accounts.zoho.com/oauth/v2/token";
			s_token_url = oss.str();
			return s_token_url.c_str();
		}

		const char* oauth::client_id()
		{
			static std::string s_client_id;
			s_client_id = "1000.0ESNDLDTNIT733894WBY544KIGZAEA";
			return s_client_id.c_str();
		}

		const char* oauth::client_secret()
		{
			static std::string s_client_secret;
			s_client_secret = "15300e4a0f75ec7f400e7442c3bb5ccd250e1e2a8b";
			return s_client_secret.c_str();
		}

		const char* oauth::redirect_url()
		{
			static std::string s_redirect_url;
			s_redirect_url = "http://localhost:8702";
			return s_redirect_url.c_str();
		}

		const char* oauth::landing_url()
		{
			static std::string s_landing_url;
			s_landing_url = "https://accounts.zoho.com";
			return s_landing_url.c_str();
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

		std::vector<project> projects::all_projects(session& p_sess, portal& p_portal,
			const int p_index,
			const int p_range,
			const std::string& p_status,
			const std::string& p_sort_column,
			const std::string& p_sort_order,
			const web::json::value& p_json_string)
		{
			std::vector<project> projects;
			web::http::client::http_client client = p_sess.client(projects_endpoint(p_portal));
			try
			{
				auto query = web::uri_builder();
				if (p_index != 0) query.append_query(U("index"), util::string(std::to_string(p_index)));
				if (p_range != 0) query.append_query(U("range"), util::string(std::to_string(p_range)));
				if (!p_status.empty()) query.append_query(U("status"), util::string(p_status));
				if (!p_sort_column.empty()) query.append_query(U("sort_column"), util::string(p_sort_column));
				if (!p_sort_order.empty()) query.append_query(U("sort_order"), util::string(p_sort_order));
				if (!p_json_string.is_null()) query.append_query(U("json_string"), p_json_string.serialize());
				web::http::http_response response = client.request(web::http::methods::GET, query.to_string()).get();
				web::json::value json = response.extract_json().get();
				OutputDebugString(json.serialize().c_str());
				if (codes::is_success(response.status_code()))
				{
					web::json::object object = json.as_object();
					for (web::json::value item : json.as_object()[U("projects")].as_array())
					{
						projects.emplace_back(item);
					}
					return projects;
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

		inline std::string projects::project_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + '/';
		}

		project projects::project_details(session& p_sess, portal& p_portal, project& p_project)
		{
			web::http::client::http_client client = p_sess.client(project_endpoint(p_portal, p_project));
			try
			{
				web::http::http_response response = client.request(web::http::methods::GET).get();
				web::json::value json = response.extract_json().get();
				OutputDebugString(json.serialize().c_str());
				if (codes::is_success(response.status_code()))
				{
					web::json::object object = json.as_object();
					return json.as_object()[U("projects")].as_array()[0];
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

		inline std::string projects::customfields_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/customfields/";
		}

		std::vector<customfield> projects::get_customfields(session& p_sess, portal& p_portal)
		{
			std::vector<customfield> fields;
			web::http::client::http_client client = p_sess.client(customfields_endpoint(p_portal));
			try
			{
				web::http::http_response response = client.request(web::http::methods::GET).get();
				web::json::value json = response.extract_json().get();
				OutputDebugString(json.serialize().c_str());
				if (codes::is_success(response.status_code()))
				{
					web::json::object object = json.as_object();
					for (web::json::value item : json.as_object()[U("project_custom_fields")].as_array())
					{
						fields.emplace_back(item);
					}
					return fields;
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

		inline std::string projects::groups_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/groups";
		}

		std::vector<group> projects::get_groups(session& p_sess, portal& p_portal)
		{
			std::vector<group> groups;
			web::http::client::http_client client = p_sess.client(groups_endpoint(p_portal));
			try
			{
				web::http::http_response response = client.request(web::http::methods::GET).get();
				web::json::value json = response.extract_json().get();
				OutputDebugString(json.serialize().c_str());
				if (codes::is_success(response.status_code()))
				{
					web::json::object object = json.as_object();
					for (web::json::value item : json.as_object()[U("groups")].as_array())
					{
						groups.emplace_back(item);
					}
					return groups;
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

		project projects::create_project(session& p_sess, portal& p_portal,
				const std::string& p_name,
				const id p_owner,
				const std::string& p_description,
				const id p_template_id,
				const std::chrono::system_clock::time_point& p_start_date,
				const std::chrono::system_clock::time_point& p_end_date,
				const std::string& p_strict_project,
				const std::map<std::string, std::string> p_custom_fields)
		{
			web::http::client::http_client client = p_sess.client(projects_endpoint(p_portal));
			try
			{
				web::json::value params;
				params[U("name")] = web::json::value::string(util::string(p_name));
				if (p_owner != 0) params[U("owner")] = web::json::value::number(p_owner);
				if (!p_description.empty()) params[U("description")] = web::json::value::string(util::string(p_description));
				if (p_template_id != 0) params[U("template_id")] = web::json::value::number(p_template_id);
				if (p_start_date.time_since_epoch().count() != 0)
				{
					auto tt = std::chrono::system_clock::to_time_t(p_start_date);
					struct DLLEXPORT tm time;
					::localtime_s(&time, &tt);
					std::ostringstream oss;
					oss << std::put_time(&time, "%m-%d-%Y");
					params[U("start_date")] = web::json::value::string(util::string(oss.str()));
				}
				if (p_end_date.time_since_epoch().count() != 0)
				{
					auto tt = std::chrono::system_clock::to_time_t(p_end_date);
					struct DLLEXPORT tm time;
					::localtime_s(&time, &tt);
					std::ostringstream oss;
					oss << std::put_time(&time, "%m-%d-%Y");
					params[U("start_date")] = web::json::value::string(util::string(oss.str()));
				}
				if (!p_strict_project.empty()) params[U("strict_project")] = web::json::value::string(util::string(p_strict_project));
				for (auto cf : p_custom_fields) params[util::string(cf.first)] = web::json::value::string(util::string(cf.second));
				web::http::http_response response = client.request(web::http::methods::POST, U("/"), params).get();
				web::json::value json = response.extract_json().get();
				OutputDebugString(json.serialize().c_str());
				if (codes::is_success(response.status_code()))
				{
					web::json::object object = json.as_object();
					return json.as_object()[U("groups")].as_array()[0];
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

		project projects::update_project(session& p_sess, portal& p_portal, project& p_project,
				const std::string& p_name,
				const id p_owner,
				const std::string& p_description,
				const id p_template_id,
				const std::chrono::system_clock::time_point& p_start_date,
				const std::chrono::system_clock::time_point& p_end_date,
				const std::string& p_strict_project,
				const std::map<std::string, std::string> p_custom_fields)
		{
			web::http::client::http_client client = p_sess.client(projects_endpoint(p_portal));
			try
			{
				web::json::value params;
				params[U("name")] = web::json::value::string(util::string(p_name));
				if (p_owner != 0) params[U("owner")] = web::json::value::number(p_owner);
				if (!p_description.empty()) params[U("description")] = web::json::value::string(util::string(p_description));
				if (p_template_id != 0) params[U("template_id")] = web::json::value::number(p_template_id);
				if (p_start_date.time_since_epoch().count() != 0)
				{
					auto tt = std::chrono::system_clock::to_time_t(p_start_date);
					struct DLLEXPORT tm time;
					::localtime_s(&time, &tt);
					std::ostringstream oss;
					oss << std::put_time(&time, "%m-%d-%Y");
					params[U("start_date")] = web::json::value::string(util::string(oss.str()));
				}
				if (p_end_date.time_since_epoch().count() != 0)
				{
					auto tt = std::chrono::system_clock::to_time_t(p_end_date);
					struct DLLEXPORT tm time;
					::localtime_s(&time, &tt);
					std::ostringstream oss;
					oss << std::put_time(&time, "%m-%d-%Y");
					params[U("start_date")] = web::json::value::string(util::string(oss.str()));
				}
				if (!p_strict_project.empty()) params[U("strict_project")] = web::json::value::string(util::string(p_strict_project));
				for (auto cf : p_custom_fields) params[util::string(cf.first)] = web::json::value::string(util::string(cf.second));
				web::http::http_response response = client.request(web::http::methods::POST, U("/"), params).get();
				web::json::value json = response.extract_json().get();
				OutputDebugString(json.serialize().c_str());
				if (codes::is_success(response.status_code()))
				{
					web::json::object object = json.as_object();
					return json.as_object()[U("groups")].as_array()[0];
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

		void projects::delete_project(session& p_sess, portal& p_portal, project& p_project)
		{
			web::http::client::http_client client = p_sess.client(project_endpoint(p_portal, p_project));
			try
			{
				web::http::http_response response = client.request(web::http::methods::DEL).get();
				web::json::value json = response.extract_json().get();
				OutputDebugString(json.serialize().c_str());
				if (codes::is_success(response.status_code()))
				{
					web::json::object object = json.as_object();
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

		inline std::string dashboard::activities_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/activities/";
		}

		std::vector<activity> dashboard::project_activities(session& p_sess, portal& p_portal, project& p_project,
			const int p_index,
			const int p_range)
		{
			std::vector<activity> activities;
			web::http::client::http_client client = p_sess.client(activities_endpoint(p_portal, p_project));
			try
			{
				auto query = web::uri_builder();
				if (p_index != 0) query.append_query(U("index"), util::string(std::to_string(p_index)));
				if (p_range != 0) query.append_query(U("range"), util::string(std::to_string(p_range)));
				web::http::http_response response = client.request(web::http::methods::GET).get();
				web::json::value json = response.extract_json().get();
				OutputDebugString(json.serialize().c_str());
				if (codes::is_success(response.status_code()))
				{
					web::json::object object = json.as_object();
					for (web::json::value item : json.as_object()[U("groups")].as_array())
					{
						activities.emplace_back(item);
					}
					return activities;
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

		inline std::string dashboard::statuses_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id_str() + "/projects/" + p_project.id_str() + "/statuses/";
		}

		std::vector<status> dashboard::project_statuses(session& p_sess, portal& p_portal, project& p_project,
			const int p_index,
			const int p_range)
		{
			std::vector<status> statuses;
			web::http::client::http_client client = p_sess.client(statuses_endpoint(p_portal, p_project));
			try
			{
				auto query = web::uri_builder();
				if (p_index != 0) query.append_query(U("index"), util::string(std::to_string(p_index)));
				if (p_range != 0) query.append_query(U("range"), util::string(std::to_string(p_range)));
				web::http::http_response response = client.request(web::http::methods::GET).get();
				web::json::value json = response.extract_json().get();
				OutputDebugString(json.serialize().c_str());
				if (codes::is_success(response.status_code()))
				{
					web::json::object object = json.as_object();
					for (web::json::value item : json.as_object()[U("groups")].as_array())
					{
						statuses.emplace_back(item);
					}
					return statuses;
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

		status dashboard::add_status(session& p_sess, portal& p_portal, project& p_project,
			const std::string& p_content)
		{
			web::http::client::http_client client = p_sess.client(statuses_endpoint(p_portal, p_project));
			try
			{
				web::json::value params;
				params[U("content")] = web::json::value::string(util::string(p_content));
				web::http::http_response response = client.request(web::http::methods::POST, U("/"), params).get();
				web::json::value json = response.extract_json().get();
				OutputDebugString(json.serialize().c_str());
				if (codes::is_success(response.status_code()))
				{
					web::json::object object = json.as_object();
					return json.as_object()[U("groups")].as_array()[0];
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
}
