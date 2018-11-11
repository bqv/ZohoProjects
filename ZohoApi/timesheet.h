#pragma once

#include "types.h"

namespace zoho
{
	template<class T, typename = std::enable_if<std::is_base_of<entity, T>::value>>
	class log : public entity
	{
	public:
		log() = delete;
	};
}
