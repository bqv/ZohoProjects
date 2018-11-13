#pragma once

#include <string>
#include <vector>

#include "cpprest/json.h"

namespace zoho
{
	typedef unsigned long long id;

	class url
	{
	private:
		const utility::string_t m_url;

	public:
		url(const utility::string_t&);

		utility::string_t to_string();
	};

	class entity
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
	struct comment : public entity
	{
	public:
		comment() = delete;
	};

	template<class T, typename = std::enable_if<std::is_base_of<entity, T>::value>>
	struct attachment : public entity
	{
	public:
		attachment() = delete;
	};
}
