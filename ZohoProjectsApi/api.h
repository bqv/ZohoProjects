#pragma once

#include <string>
#include <vector>

#include "lib.h"
#include "oauth.h"
#include "zoho.h"
#include "portal.h"
#include "project.h"
#include "dashboard.h"
#include "milestone.h"
#include "tasklist.h"
#include "task.h"
#include "timesheet.h"
#include "bug.h"
#include "event.h"
#include "forum.h"
#include "user.h"

namespace zoho
{
	namespace api
	{
		class DLLEXPORT exception : public std::exception
		{
		public:
			const std::exception inner;
			const std::string message;

			exception(const std::exception&, const std::string&);
			const char* what() const throw ();
		};

		class DLLEXPORT oauth
		{
		private:
			oauth() = delete;

		public:
			static const char* authorization_url();
			static const char* token_url();
			static const char* client_id();
			static const char* client_secret();
			static const char* redirect_url();
			static const char* landing_url();
		};

		class DLLEXPORT portals
		{
		private:
			portals() = delete;

		public:
			inline static std::string portals_endpoint();

			static std::vector<portal> all_portals(session&);
		};

		class DLLEXPORT projects
		{
		private:
			projects() = delete;

		public:
			inline static std::string projects_endpoint(portal&);
			inline static std::string project_endpoint(portal&, project&);
			inline static std::string customfields_endpoint(portal&);
			inline static std::string groups_endpoint(portal&);

			static std::vector<project> all_projects(session&, portal&,
				const int = 0,
				const int = 0,
				const std::string& = "",
				const std::string& = "",
				const std::string& = "",
				const web::json::value& = web::json::value());
			static project project_details(session&, portal&, project&);
			static std::vector<customfield> get_customfields(session&, portal&);
			static std::vector<group> get_groups(session&, portal&);
			static project create_project(session&, portal&,
				const std::string&,
				const id = 0,
				const std::string& = "",
				const id = 0,
				const std::chrono::system_clock::time_point& = std::chrono::system_clock::from_time_t(0),
				const std::chrono::system_clock::time_point& = std::chrono::system_clock::from_time_t(0),
				const std::string& = "",
				const std::map<std::string, std::string> = {});
			static project update_project(session&, portal&, project&,
				const std::string&,
				const id = 0,
				const std::string& = "",
				const id = 0,
				const std::chrono::system_clock::time_point& = std::chrono::system_clock::from_time_t(0),
				const std::chrono::system_clock::time_point& = std::chrono::system_clock::from_time_t(0),
				const std::string& = "",
				const std::map<std::string, std::string> = {});
			static void delete_project(session&, portal&, project&);
		};

		class DLLEXPORT dashboard
		{
		private:
			dashboard() = delete;

		public:
			inline static std::string activities_endpoint(portal&, project&);
			inline static std::string statuses_endpoint(portal&, project&);

			static std::vector<activity> project_activities(session&, portal&, project&,
				const int = 0,
				const int = 0);
			static std::vector<status> project_statuses(session&, portal&, project&,
				const int = 0,
				const int = 0);
			static status add_status(session&, portal&, project&,
				const std::string& = "");
		};

		class DLLEXPORT milestones
		{
		private:
			milestones() = delete;

		public:
			inline static std::string milestones_endpoint(portal&, project&);
			inline static std::string milestone_endpoint(portal&, project&, milestone&);
			inline static std::string status_endpoint(portal&, project&, milestone&);
		};

		class DLLEXPORT tasklists
		{
		private:
			tasklists() = delete;

		public:
			inline static std::string tasklists_endpoint(portal&, project&);
			inline static std::string tasklist_endpoint(portal&, project&, tasklist&);
		};

		class DLLEXPORT tasks
		{
		private:
			tasks() = delete;

		public:
			inline static std::string tasks_endpoint(portal&, project&);
			inline static std::string tasklist_tasks_endpoint(portal&, project&, tasklist&);
			inline static std::string task_endpoint(portal&, project&, task&);
			inline static std::string reorder_endpoint(portal&, project&, task&);
			inline static std::string subtasks_endpoint(portal&, project&, task&);
			inline static std::string my_tasks_endpoint(portal&);
			inline static std::string comments_endpoint(portal&, project&, task&);
			inline static std::string comment_endpoint(portal&, project&, task&, comment<task>&);
			inline static std::string attachments_endpoint(portal&, project&, task&);
			inline static std::string attachment_endpoint(portal&, project&, task&, attachment<task>&);
			inline static std::string layouts_endpoint(portal&);
			inline static std::string layouts_endpoint(portal&, project&);
		};

		class DLLEXPORT timesheets
		{
		private:
			timesheets() = delete;

		public:
			inline static std::string project_timelogs_endpoint(portal&, project&);
			inline static std::string my_timelogs_endpoint(portal&);
			inline static std::string task_timelogs_endpoint(portal&, project&, task&);
			inline static std::string approve_task_timelogs_endpoint(portal&, project&, task&, log<task>&);
			inline static std::string task_timelog_endpoint(portal&, project&, task&, log<task>&);
			inline static std::string bug_timelogs_endpoint(portal&, project&, bug&);
			inline static std::string approve_bug_timelogs_endpoint(portal&, project&, bug&, log<bug>&);
			inline static std::string bug_timelog_endpoint(portal&, project&, bug&, log<bug>&);
			inline static std::string general_timelogs_endpoint(portal&, project&);
			inline static std::string approve_general_timelogs_endpoint(portal&, project&, log<project>&);
			inline static std::string general_timelog_endpoint(portal&, project&, log<project>&);
		};

		class DLLEXPORT bugs
		{
		private:
			bugs() = delete;

		public:
			inline static std::string bugs_endpoint(portal&, project&);
			inline static std::string bug_endpoint(portal&, project&, bug&);
			inline static std::string comments_endpoint(portal&, project&);
			inline static std::string comments_endpoint(portal&, project&, bug&);
			inline static std::string comment_endpoint(portal&, project&, bug&, comment<bug>&);
			inline static std::string timer_endpoint(portal&, project&, bug&);
			inline static std::string customviews_endpoint(portal&, project&);
			inline static std::string attachments_endpoint(portal&, project&, bug&);
			inline static std::string resolution_endpoint(portal&, project&, bug&);
			inline static std::string followers_endpoint(portal&, project&, bug&);
			inline static std::string defaultfields_endpoint(portal&, project&);
			inline static std::string customfields_endpoint(portal&, project&);
			inline static std::string activities_endpoint(portal&, project&, bug&);
			inline static std::string renamedfields_endpoint(portal&, project&);
			inline static std::string my_bugs_endpoint(portal&);
		};

		class DLLEXPORT events
		{
		private:
			events() = delete;

		public:
			inline static std::string events_endpoint(portal&, project&);
			inline static std::string event_endpoint(portal&, project&, event&);
		};

		class DLLEXPORT forums
		{
		private:
			forums() = delete;

		public:
			inline static std::string forums_endpoint(portal&, project&);
			inline static std::string forum_endpoint(portal&, project&, forum&);
			inline static std::string follow_endpoint(portal&, project&, forum&);
			inline static std::string unfollow_endpoint(portal&, project&, forum&);
			inline static std::string categories_endpoint(portal&, project&);
			inline static std::string comments_endpoint(portal&, project&, forum&);
			inline static std::string comment_endpoint(portal&, project&, forum&, comment<forum>&);
			inline static std::string bestanswer_endpoint(portal&, project&, forum&, comment<forum>&);
			inline static std::string category_endpoint(portal&, project&, category&);
		};

		class DLLEXPORT users
		{
		private:
			users() = delete;

		public:
			inline static std::string users_endpoint(portal&, project&);
			inline static std::string user_endpoint(portal&, project&, user&);
			inline static std::string avail_endpoint(portal&);
			inline static std::string users_endpoint(portal&);
			inline static std::string activation_endpoint(portal&);
			inline static std::string company_endpoint(portal&, company&);
		};
	}
}
