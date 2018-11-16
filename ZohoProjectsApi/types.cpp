#include "stdafx.h"
#include "types.h"

#include "util.h"

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

	entity::entity(const unsigned long long p_id)
		: id(p_id)
	{
	}
}
