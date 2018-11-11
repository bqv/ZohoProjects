#include "stdafx.h"
#include "project.h"

namespace zoho
{
	project::project(const web::json::value& p_json)
		: entity::entity(JSON_GET_INTEGER(p_json, "id", -1))
		, created_date(std::chrono::duration(std::chrono::milliseconds(
			JSON_GET_LONG(p_json, "created_date_long", -1))))
		, is_bug_enabled(JSON_GET_BOOL(p_json, "IS_BUG_ENABLED", false))
		, owner_name(JSON_GET_STRING(p_json, "owner_name", ""))
		, task_count({
			JSON_GET_GET_INTEGER(p_json, "task_count", "open", 0),
			JSON_GET_GET_INTEGER(p_json, "task_count", "closed", 0),
		  })
		, status(JSON_GET_STRING(p_json, "status", ""))
		, link({
			url(JSON_GET_GET_GET_STRING(p_json, "link", "folder", "url", "")),
			url(JSON_GET_GET_GET_STRING(p_json, "link", "milestone", "url", "")),
			url(JSON_GET_GET_GET_STRING(p_json, "link", "forum", "url", "")),
			url(JSON_GET_GET_GET_STRING(p_json, "link", "document", "url", "")),
			url(JSON_GET_GET_GET_STRING(p_json, "link", "status", "url", "")),
			url(JSON_GET_GET_GET_STRING(p_json, "link", "event", "url", "")),
			url(JSON_GET_GET_GET_STRING(p_json, "link", "task", "url", "")),
			url(JSON_GET_GET_GET_STRING(p_json, "link", "bug", "url", "")),
			url(JSON_GET_GET_GET_STRING(p_json, "link", "self", "url", "")),
			url(JSON_GET_GET_GET_STRING(p_json, "link", "timesheet", "url", "")),
			url(JSON_GET_GET_GET_STRING(p_json, "link", "user", "url", "")),
			url(JSON_GET_GET_GET_STRING(p_json, "link", "tasklist", "url", "")),
			url(JSON_GET_GET_GET_STRING(p_json, "link", "activity", "url", "")),
		  })
		, workspace_id(JSON_GET_STRING(p_json, "workspace_id", ""))
		, milestone_count({
			JSON_GET_GET_INTEGER(p_json, "milestone_count", "open", 0),
			JSON_GET_GET_INTEGER(p_json, "milestone_count", "closed", 0),
		  })
		, description(JSON_GET_STRING(p_json, "description", ""))
		, name(JSON_GET_STRING(p_json, "name", ""))
		, owner_id(JSON_GET_STRING(p_json, "owner_id", ""))
		, role(JSON_GET_STRING(p_json, "role", ""))
	{
		web::json::array array = JSON_GET_ARRAY(p_json, "custom_fields", web::json::value::array().as_array());
		for (web::json::value item : array)
		{
			web::json::object pair = item.as_object();
			customfields.insert_or_assign(pair.begin()->first, pair.begin()->second.as_string());
		}
	}
}
