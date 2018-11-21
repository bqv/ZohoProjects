#pragma once
#include "stdafx.h"

#include <map>
#include <chrono>

#include "lib.h"
#include "types.h"
#include "util.h"
#include "cpprest/json.h"

namespace zoho
{
	class DLLEXPORT project : public entity
	{
	public:
		std::map<utility::string_t, utility::string_t> customfields;
		std::chrono::system_clock::time_point created_date;
		bool is_bug_enabled;
		utility::string_t owner_name;
		struct DLLEXPORT
		{
			long open;
			long closed;
		} const task_count;
		utility::string_t status;
		struct DLLEXPORT
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
		struct DLLEXPORT
		{
			long open;
			long closed;
		} const milestone_count;
		utility::string_t description;
		utility::string_t name;
		utility::string_t owner_id;
		utility::string_t role;
		struct DLLEXPORT
		{
			long open;
			long closed;
		} const bug_count;

		project(const web::json::value&);
	};

	class DLLEXPORT customfield
	{
	public:
		customfield(const web::json::value&);

		const utility::string_t id;
		const utility::string_t field_id;
		const utility::string_t field_type;
		const utility::string_t field_name;
		const bool is_visible;
		const bool is_pii;
		const bool is_encrypted;
		const utility::string_t default_value;
	};

	class DLLEXPORT group
	{
	public:
		group(const web::json::value&);

		const utility::string_t name;
		const utility::string_t id;
	};
}
