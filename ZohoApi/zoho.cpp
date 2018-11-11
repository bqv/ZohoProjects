#include "stdafx.h"
#include "zoho.h"

#include <vector>
#include <sstream>
#include <numeric>

namespace zoho
{
	namespace api
	{
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

		std::string portals::portals_endpoint()
		{
			return base_url + "/portals/";
		}

		std::string projects::projects_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/";
		}

		std::string projects::project_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + '/';
		}

		std::string projects::customfields_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/customfields/";
		}

		std::string projects::groups_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/groups";
		}

		std::string dashboard::activities_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/activities/";
		}

		std::string dashboard::statuses_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/statuses/";
		}

		std::string milestones::milestones_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/milestones/";
		}

		std::string milestones::milestone_endpoint(portal& p_portal, project& p_project, milestone& p_milestone)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/milestones/" + p_milestone.id + '/';
		}

		std::string milestones::status_endpoint(portal& p_portal, project& p_project, milestone& p_milestone)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/milestones/" + p_milestone.id + "/status/";
		}

		std::string tasklists::tasklists_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/tasklists/";
		}

		std::string tasklists::tasklist_endpoint(portal& p_portal, project& p_project, tasklist& p_tasklist)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/tasklists/" + p_tasklist.id + '/';
		}

		std::string tasks::tasks_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/tasks/";
		}

		std::string tasks::tasklist_tasks_endpoint(portal& p_portal, project& p_project, tasklist& p_tasklist)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/tasklists/" + p_tasklist.id + "/tasks/";
		}

		std::string tasks::task_endpoint(portal& p_portal, project& p_project, task& p_task)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/tasks/" + p_task.id + '/';
		}

		std::string tasks::reorder_endpoint(portal& p_portal, project& p_project, task& p_task)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/tasks/" + p_task.id + "/reorder";
		}

		std::string tasks::subtasks_endpoint(portal& p_portal, project& p_project, task& p_task)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/tasks/" + p_task.id + "/subtasks/";
		}

		std::string tasks::my_tasks_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id + "/mytasks/";
		}

		std::string tasks::comments_endpoint(portal& p_portal, project& p_project, task& p_task)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/tasks/" + p_task.id + "/comments/";
		}

		std::string tasks::comment_endpoint(portal& p_portal, project& p_project, task& p_task, comment<task>& p_comment)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/tasks/" + p_task.id + "/comments/" + p_comment.id + '/';
		}

		std::string tasks::attachments_endpoint(portal& p_portal, project& p_project, task& p_task)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/tasks/" + p_task.id + "/attachments/";
		}

		std::string tasks::attachment_endpoint(portal& p_portal, project& p_project, task& p_task, attachment<task>& p_attachment)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/tasks/" + p_task.id + "/attachments/" + p_attachment.id + '/';
		}

		std::string tasks::layouts_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id + "/tasklayouts/";
		}

		std::string tasks::layouts_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/tasklayouts/";
		}

		std::string timesheets::project_timelogs_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/logs/";
		}

		std::string timesheets::my_timelogs_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id + "/logs/";
		}

		std::string timesheets::task_timelogs_endpoint(portal& p_portal, project& p_project, task& p_task)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/tasks/" + p_task.id + "/logs/";
		}

		std::string timesheets::approve_task_timelogs_endpoint(portal& p_portal, project& p_project, task& p_task, log<task>& p_log)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/tasks/" + p_task.id + "/logs/" + p_log.id + "/approval";
		}

		std::string timesheets::task_timelog_endpoint(portal& p_portal, project& p_project, task& p_task, log<task>& p_log)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/tasks/" + p_task.id + "/logs/" + p_log.id + '/';
		}

		std::string timesheets::bug_timelogs_endpoint(portal& p_portal, project& p_project, bug& p_bug)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/bugs/" + p_bug.id + "/logs/";
		}

		std::string timesheets::approve_bug_timelogs_endpoint(portal& p_portal, project& p_project, bug& p_bug, log<bug>& p_log)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/bugs/" + p_bug.id + "/logs/" + p_log.id + "/approval";
		}

		std::string timesheets::bug_timelog_endpoint(portal& p_portal, project& p_project, bug& p_bug, log<bug>& p_log)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/bugs/" + p_bug.id + "/logs/" + p_log.id + '/';
		}

		std::string timesheets::general_timelogs_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/logs/";
		}

		std::string timesheets::approve_general_timelogs_endpoint(portal& p_portal, project& p_project, log<project>& p_log)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/logs/" + p_log.id + "/approval";
		}

		std::string timesheets::general_timelog_endpoint(portal& p_portal, project& p_project, log<project>& p_log)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/logs/" + p_log.id + '/';
		}

		std::string bugs::bugs_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/bugs/";
		}

		std::string bugs::bug_endpoint(portal& p_portal, project& p_project, bug& p_bug)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/bugs/" + p_bug.id + '/';
		}

		std::string bugs::comments_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/bugs/comments/";
		}

		std::string bugs::comments_endpoint(portal& p_portal, project& p_project, bug& p_bug)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/bugs/" + p_bug.id + "/comments/";
		}

		std::string bugs::comment_endpoint(portal& p_portal, project& p_project, bug& p_bug, comment<bug>& p_comment)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/bugs/" + p_bug.id + "/comments/" + p_comment.id;
		}

		std::string bugs::timer_endpoint(portal& p_portal, project& p_project, bug& p_bug)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/bugs/" + p_bug.id + "/timer";
		}

		std::string bugs::customviews_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/bugs/customviews/";
		}

		std::string bugs::attachments_endpoint(portal& p_portal, project& p_project, bug& p_bug)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/bugs/" + p_bug.id + "/attachments/";
		}

		std::string bugs::resolution_endpoint(portal& p_portal, project& p_project, bug& p_bug)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/bugs/" + p_bug.id + "/resolution/";
		}

		std::string bugs::followers_endpoint(portal& p_portal, project& p_project, bug& p_bug)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/bugs/" + p_bug.id + "/bugfollowers/";
		}

		std::string bugs::defaultfields_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/bugs/defaultfields/";
		}

		std::string bugs::customfields_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/bugs/customfields/";
		}

		std::string bugs::activities_endpoint(portal& p_portal, project& p_project, bug& p_bug)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/bugs/" + p_bug.id + "/activities/";
		}

		std::string bugs::renamedfields_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/bugs/renamedfields/";
		}

		std::string bugs::my_bugs_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id + "/mybugs/";
		}

		std::string events::events_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/events/";
		}

		std::string events::event_endpoint(portal& p_portal, project& p_project, event& p_event)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/events/" + p_event.id + '/';
		}

		std::string forums::forums_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/forums/";
		}

		std::string forums::forum_endpoint(portal& p_portal, project& p_project, forum& p_forum)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/forums/" + p_forum.id + '/';
		}

		std::string forums::follow_endpoint(portal& p_portal, project& p_project, forum& p_forum)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/forums/" + p_forum.id + "/follow";
		}

		std::string forums::unfollow_endpoint(portal& p_portal, project& p_project, forum& p_forum)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/forums/" + p_forum.id + "/unfollow";
		}

		std::string forums::categories_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/categories/";
		}

		std::string forums::comments_endpoint(portal& p_portal, project& p_project, forum& p_forum)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/forums/" + p_forum.id + "/comments/";
		}

		std::string forums::comment_endpoint(portal& p_portal, project& p_project, forum& p_forum, comment<forum>& p_comment)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/forums/" + p_forum.id + "/comments/" + p_comment.id + '/';
		}

		std::string forums::bestanswer_endpoint(portal& p_portal, project& p_project, forum& p_forum, comment<forum>& p_comment)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/forums/" + p_forum.id + "/comments/" + p_comment.id + "/markbestanswer";
		}

		std::string forums::category_endpoint(portal& p_portal, project& p_project, category& p_category)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/categories/" + p_category.id + '/';
		}

		std::string users::users_endpoint(portal& p_portal, project& p_project)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/users/";
		}

		std::string users::user_endpoint(portal& p_portal, project& p_project, user& p_user)
		{
			return base_url + "/portal/" + p_portal.id + "/projects/" + p_project.id + "/users/" + p_user.id + '/';
		}

		std::string users::avail_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id + "/users/availcount/";
		}

		std::string users::users_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id + "/users/";
		}

		std::string users::activation_endpoint(portal& p_portal)
		{
			return base_url + "/portal/" + p_portal.id + "/users/activate/";
		}

		std::string users::company_endpoint(portal& p_portal, company& p_company)
		{
			return base_url + "/portal/" + p_portal.id + "/clients/" + p_company.id + "/users";
		}
	}
}
