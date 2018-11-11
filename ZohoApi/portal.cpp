#include "stdafx.h"
#include "portal.h"

namespace zoho
{
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
		, link({
			url(JSON_GET_GET_GET_STRING(p_json, "link", "project", "url", "")),
		  })
	{
	}
}
