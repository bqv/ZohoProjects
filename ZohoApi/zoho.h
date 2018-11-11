#pragma once

#include <string>
#include <optional>

namespace zoho
{
	namespace api
	{
		const std::string base_url;

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
			static std::string portals_endpoint();
		};

		class projects
		{
		private:
			projects() = delete;

		public:
			static std::string projects_endpoint(portal&);
			static std::string project_endpoint(portal&, project&);
			static std::string customfields_endpoint(portal&);
			static std::string groups_endpoint(portal&);
		};

		class dashboard
		{
		private:
			dashboard() = delete;

		public:
			static std::string activities_endpoint(portal&, project&);
			static std::string statuses_endpoint(portal&, project&);
		};

		class milestones
		{
		private:
			milestones() = delete;

		public:
			static std::string milestones_endpoint(portal&, project&);
			static std::string milestone_endpoint(portal&, project&, milestone&);
			static std::string status_endpoint(portal&, project&, milestone&);
		};

		class tasklists
		{
		private:
			tasklists() = delete;

		public:
			static std::string tasklists_endpoint(portal&, project&);
			static std::string tasklist_endpoint(portal&, project&, tasklist&);
		};

		class tasks
		{
		private:
			tasks() = delete;

		public:
			static std::string tasks_endpoint(portal&, project&);
			static std::string tasklist_tasks_endpoint(portal&, project&, tasklist&);
			static std::string task_endpoint(portal&, project&, task&);
			static std::string reorder_endpoint(portal&, project&, task&);
			static std::string subtasks_endpoint(portal&, project&, task&);
			static std::string my_tasks_endpoint(portal&);
			static std::string comments_endpoint(portal&, project&, task&);
			static std::string comment_endpoint(portal&, project&, task&, comment<task>&);
			static std::string attachments_endpoint(portal&, project&, task&);
			static std::string attachment_endpoint(portal&, project&, task&, attachment<task>&);
			static std::string layouts_endpoint(portal&);
			static std::string layouts_endpoint(portal&, project&);
		};

		class timesheets
		{
		private:
			timesheets() = delete;

		public:
			static std::string project_timelogs_endpoint(portal&, project&);
			static std::string my_timelogs_endpoint(portal&);
			static std::string task_timelogs_endpoint(portal&, project&, task&);
			static std::string approve_task_timelogs_endpoint(portal&, project&, task&, log<task>&);
			static std::string task_timelog_endpoint(portal&, project&, task&, log<task>&);
			static std::string bug_timelogs_endpoint(portal&, project&, bug&);
			static std::string approve_bug_timelogs_endpoint(portal&, project&, bug&, log<bug>&);
			static std::string bug_timelog_endpoint(portal&, project&, bug&, log<bug>&);
			static std::string general_timelogs_endpoint(portal&, project&);
			static std::string approve_general_timelogs_endpoint(portal&, project&, log<project>&);
			static std::string general_timelog_endpoint(portal&, project&, log<project>&);
		};

		class bugs
		{
		private:
			bugs() = delete;

		public:
			static std::string bugs_endpoint(portal&, project&);
			static std::string bug_endpoint(portal&, project&, bug&);
			static std::string comments_endpoint(portal&, project&);
			static std::string comments_endpoint(portal&, project&, bug&);
			static std::string comment_endpoint(portal&, project&, bug&, comment<bug>&);
			static std::string timer_endpoint(portal&, project&, bug&);
			static std::string customviews_endpoint(portal&, project&);
			static std::string attachments_endpoint(portal&, project&, bug&);
			static std::string resolution_endpoint(portal&, project&, bug&);
			static std::string followers_endpoint(portal&, project&, bug&);
			static std::string defaultfields_endpoint(portal&, project&);
			static std::string customfields_endpoint(portal&, project&);
			static std::string activities_endpoint(portal&, project&, bug&);
			static std::string renamedfields_endpoint(portal&, project&);
			static std::string my_bugs_endpoint(portal&);
		};

		class events
		{
		private:
			events() = delete;

		public:
			static std::string events_endpoint(portal&, project&);
			static std::string event_endpoint(portal&, project&, event&);
		};

		class forums
		{
		private:
			forums() = delete;

		public:
			static std::string forums_endpoint(portal&, project&);
			static std::string forum_endpoint(portal&, project&, forum&);
			static std::string follow_endpoint(portal&, project&, forum&);
			static std::string unfollow_endpoint(portal&, project&, forum&);
			static std::string categories_endpoint(portal&, project&);
			static std::string comments_endpoint(portal&, project&, forum&);
			static std::string comment_endpoint(portal&, project&, forum&, comment<forum>&);
			static std::string bestanswer_endpoint(portal&, project&, forum&, comment<forum>&);
			static std::string category_endpoint(portal&, project&, category&);
		};

		class users
		{
		private:
			users() = delete;

		public:
			static std::string users_endpoint(portal&, project&);
			static std::string user_endpoint(portal&, project&, user&);
			static std::string avail_endpoint(portal&);
			static std::string users_endpoint(portal&);
			static std::string activation_endpoint(portal&);
			static std::string company_endpoint(portal&, company&);
		};
	}

	struct entity
	{
	public:
		std::string id;
		
	protected:
		entity() {};
	};

	struct portal : public entity
	{
	public:
		portal() {};
	};

	struct project : public entity
	{
	public:
		project() {};
	};

	struct milestone : public entity
	{
	public:
		milestone() {};
	};

	struct tasklist : public entity
	{
	public:
		tasklist() {};
	};

	struct task : public entity
	{
	public:
		task() {};
	};

	template<class T, typename = std::enable_if<std::is_base_of<entity, T>::value>>
	struct comment : public entity
	{
	public:
		comment() {};
	};

	template<class T, typename = std::enable_if<std::is_base_of<entity, T>::value>>
	struct attachment : public entity
	{
	public:
		attachment() {};
	};

	template<class T, typename = std::enable_if<std::is_base_of<entity, T>::value>>
	struct log : public entity
	{
	public:
		log() {};
	};

	struct bug : public entity
	{
	public:
		bug() {};
	};

	struct event : public entity
	{
	public:
		event() {};
	};

	struct forum : public entity
	{
	public:
		forum() {};
	};

	struct category : public entity
	{
	public:
		category() {};
	};

	struct user : public entity
	{
	public:
		user() {};
	};

	struct company : public entity
	{
	public:
		company() {};
	};
}
