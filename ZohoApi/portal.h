#pragma once

#include "types.h"
#include "util.h"
#include "cpprest/json.h"

namespace zoho
{
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
			url project;
		} const link;

		portal(const web::json::value&);
	};

}
