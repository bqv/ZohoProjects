#include "stdafx.h"
#include "dashboard.h"

namespace zoho
{
	activity::activity(const web::json::value& p_json)
		: entity::entity(JSON_GET_LONG(p_json, "id", -1))
		, state(JSON_GET_STRING(p_json, "state", ""))
		, activity_for(JSON_GET_STRING(p_json, "activity_for", ""))
		, name(JSON_GET_STRING(p_json, "name", ""))
		, activity_by(JSON_GET_STRING(p_json, "activity_by", ""))
		, time(std::chrono::duration(std::chrono::milliseconds(
			JSON_GET_LONG(p_json, "time_long", 0))))
	{
	}

	status::status(const web::json::value& p_json)
		: entity::entity(JSON_GET_LONG(p_json, "id", -1))
		, content(JSON_GET_STRING(p_json, "content", ""))
		, posted_by(JSON_GET_STRING(p_json, "posted_by", ""))
		, posted_person(JSON_GET_STRING(p_json, "posted_person", ""))
		, posted_time(std::chrono::duration(std::chrono::milliseconds(
			JSON_GET_LONG(p_json, "posted_time_long", 0))))
	{
	}
}
