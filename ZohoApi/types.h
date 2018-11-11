#pragma once

#include <string>
#include <vector>

#include "cpprest/details/basic_types.h"

namespace zoho
{
	class url
	{
	private:
		const utility::string_t m_url;

	public:
		url(const utility::string_t&);

		utility::string_t to_string();
	};

	class customfield
	{
	public:
		customfield(const utility::string_t&, const utility::string_t&);

		const utility::string_t key;
		const utility::string_t value;
	};

	class entity
	{
	public:
		const int id;

		const std::string id_str()
		{
			return std::to_string(id);
		}

	protected:
		entity(const int p_id);
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
