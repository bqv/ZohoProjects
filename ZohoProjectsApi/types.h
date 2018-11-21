#pragma once
#include "stdafx.h"

#include <string>
#include <vector>

#include "lib.h"
#include "cpprest/json.h"

namespace zoho
{
	typedef unsigned long long id;

	class DLLEXPORT url
	{
	private:
		const utility::string_t m_url;

	public:
		url(const utility::string_t&);

		utility::string_t to_string();
	};

	class DLLEXPORT entity
	{
	public:
		const id id;

		const std::string id_str()
		{
			return std::to_string(id);
		}

	protected:
		entity(const unsigned long long p_id);
	};

	template<class T, typename = std::enable_if<std::is_base_of<entity, T>::value>>
	struct DLLEXPORT comment : public entity
	{
	public:
		comment() = delete;
	};

	template<class T, typename = std::enable_if<std::is_base_of<entity, T>::value>>
	struct DLLEXPORT attachment : public entity
	{
	public:
		attachment() = delete;
	};
}
