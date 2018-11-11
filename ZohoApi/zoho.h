#pragma once

#include <string>
#include <vector>

#include "oauth.h"

#define JSON_GET_STRING(json, key, value) (json.has_field(U(key)) ? json.at(U(key)).as_string() : U(value))
#define JSON_GET_BOOL(json, key, value) (json.has_field(U(key)) ? json.at(U(key)).as_bool() : value)
#define JSON_GET_INTEGER(json, key, value) (json.has_field(U(key)) ? json.at(U(key)).as_integer() : value)
#define JSON_GET_GET_STRING(json, key1, key2, value) (json.has_field(U(key1)) ? JSON_GET_STRING(json.at(U(key1)), key2, value) : U(value))
#define JSON_GET_GET_INTEGER(json, key1, key2, value) (json.has_field(U(key1)) ? JSON_GET_INTEGER(json.at(U(key1)), key2, value) : value)
#define JSON_GET_GET_GET_STRING(json, key1, key2, key3, value) (json.has_field(U(key1)) ? JSON_GET_GET_STRING(json.at(U(key1)), key2, key3, value) : U(value))

namespace zoho
{
	class session : private oauth2::session
	{
	public:
		session();

		web::http::client::http_client client(std::string endpoint);
	};

	class codes
	{
	private:
		codes() = delete;

	public:
		static bool is_success(web::http::status_code);
	};

	class entity
	{
	public:
		const int id;

		const std::string id_str()
		{
			return std::to_string(id);
		}

	protected:
		entity(const int p_id);
	};

	class portal : public entity
	{
	public:
		const utility::string_t name;
		const bool is_default;
		const utility::string_t gmt_time_zone;
		const utility::string_t role;
		struct
		{
			long templates;
			long archived;
			long active;
		} const project_count;
		struct
		{
			utility::string_t company_name;
			utility::string_t website_url;
			utility::string_t time_zone;
			utility::string_t date_format;
		} const settings;
		struct
		{
			utility::string_t code;
			utility::string_t language;
			utility::string_t country;
		} const locale;
		struct
		{
			struct
			{
				utility::string_t url;
			} project;
		} const link;

		portal(const web::json::value&);
	};

	struct project : public entity
	{
	public:
		project() = delete;
	};

	struct milestone : public entity
	{
	public:
		milestone() = delete;
	};

	struct tasklist : public entity
	{
	public:
		tasklist() = delete;
	};

	struct task : public entity
	{
	public:
		task() = delete;
	};

	template<class T, typename = std::enable_if<std::is_base_of<entity, T>::value>>
	struct comment : public entity
	{
	public:
		comment() = delete;
	};

	template<class T, typename = std::enable_if<std::is_base_of<entity, T>::value>>
	struct attachment : public entity
	{
	public:
		attachment() = delete;
	};

	template<class T, typename = std::enable_if<std::is_base_of<entity, T>::value>>
	struct log : public entity
	{
	public:
		log() = delete;
	};

	struct bug : public entity
	{
	public:
		bug() = delete;
	};

	struct event : public entity
	{
	public:
		event() = delete;
	};

	struct forum : public entity
	{
	public:
		forum() = delete;
	};

	struct category : public entity
	{
	public:
		category() = delete;
	};

	struct user : public entity
	{
	public:
		user() = delete;
	};

	struct company : public entity
	{
	public:
		company() = delete;
	};

	namespace api
	{
		class exception : public std::exception
		{
		public:
			const std::exception inner;
			const std::string message;

			exception(const std::exception&, const std::string&);
			const char* what() const throw ();
		};

		class oauth
		{
		private:
			static const std::string scopes[];

			oauth() = delete;
		public:
			static std::string authorization_url();
			static std::string token_url();

			static const std::string client_id;
			static const std::string client_secret;
			static const std::string redirect_url;
			static const std::string landing_url;
		};

		class portals
		{
		private:
			portals() = delete;

		public:
			inline static std::string portals_endpoint();

			static std::vector<portal> all_portals(session&);
		};

		class projects
		{
		private:
			projects() = delete;

		public:
			inline static std::string projects_endpoint(portal&);
			inline static std::string project_endpoint(portal&, project&);
			inline static std::string customfields_endpoint(portal&);
			inline static std::string groups_endpoint(portal&);
		};

		class dashboard
		{
		private:
			dashboard() = delete;

		public:
			inline static std::string activities_endpoint(portal&, project&);
			inline static std::string statuses_endpoint(portal&, project&);
		};

		class milestones
		{
		private:
			milestones() = delete;

		public:
			inline static std::string milestones_endpoint(portal&, project&);
			inline static std::string milestone_endpoint(portal&, project&, milestone&);
			inline static std::string status_endpoint(portal&, project&, milestone&);
		};

		class tasklists
		{
		private:
			tasklists() = delete;

		public:
			inline static std::string tasklists_endpoint(portal&, project&);
			inline static std::string tasklist_endpoint(portal&, project&, tasklist&);
		};

		class tasks
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

		class timesheets
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

		class bugs
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

		class events
		{
		private:
			events() = delete;

		public:
			inline static std::string events_endpoint(portal&, project&);
			inline static std::string event_endpoint(portal&, project&, event&);
		};

		class forums
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

		class users
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
