#pragma once
#include "stdafx.h"

#include "lib.h"
#include "types.h"

namespace zoho
{
	template<class T, typename = std::enable_if<std::is_base_of<entity, T>::value>>
	class DLLEXPORT log : public entity
	{
	public:
		log() = delete;
	};
}
