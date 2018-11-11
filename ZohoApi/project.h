#pragma once

#include <map>
#include <chrono>

#include "types.h"
#include "util.h"
#include "cpprest/json.h"

namespace zoho
{
	class project : public entity
	{
	public:
		std::map<utility::string_t, utility::string_t> customfields;
		std::chrono::system_clock::time_point created_date;
		bool is_bug_enabled;
		utility::string_t owner_name;
		struct
		{
			long open;
			long closed;
		} const task_count;
		utility::string_t status;
		struct
		{
			url folder;
			url milestone;
			url forum;
			url document;
			url status;
			url event;
			url task;
			url bug;
			url self;
			url timesheet;
			url user;
			url tasklist;
			url activity;
		} const link;
		utility::string_t workspace_id;
		struct
		{
			long open;
			long closed;
		} const milestone_count;
		utility::string_t description;
		utility::string_t name;
		utility::string_t owner_id;
		utility::string_t role;

		project(const web::json::value&);
	};
}
