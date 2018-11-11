#include "stdafx.h"
#include "types.h"

namespace zoho
{
	url::url(const utility::string_t& p_url)
		: m_url(p_url)
	{
	}

	utility::string_t url::to_string()
	{
		return m_url;
	}

	customfield::customfield(const utility::string_t& p_key, const utility::string_t& p_value)
		: key(p_key), value(p_value)
	{
	}


	entity::entity(const int p_id)
		: id(p_id)
	{
	}
}
