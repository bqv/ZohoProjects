#pragma once
#include "stdafx.h"

#include <chrono>

#include "lib.h"
#include "types.h"
#include "util.h"
#include "cpprest/json.h"

namespace zoho
{
	class DLLEXPORT activity : entity
	{
	public:
		activity(const web::json::value&);

		const utility::string_t state;
		const utility::string_t activity_for;
		const utility::string_t name;
		const utility::string_t activity_by;
		const std::chrono::system_clock::time_point time;
	};

	class DLLEXPORT status : entity
	{
	public:
		status(const web::json::value&);

		const utility::string_t content;
		const utility::string_t posted_by;
		const utility::string_t posted_person;
		const std::chrono::system_clock::time_point posted_time;
	};
}
