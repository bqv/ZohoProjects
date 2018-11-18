#include "stdafx.h"
#include "util.h"

namespace util
{
	std::wstring string_convert(const std::string& in)
	{
		std::wstring out(in.size(), 0);
		out.resize(std::mbstowcs(&out[0], in.c_str(), in.size()));
		return out;
	}

	std::string string_convert(const std::wstring& in)
	{
		std::string out(in.size(), 0);
		out.resize(std::wcstombs(&out[0], in.c_str(), in.size()));
		return out;
	}

#ifdef UNICODE
	std::wstring string(const std::string& s)
	{
		return string_convert(s);
	}

	std::wstring string(const std::wstring& s)
	{
		return s.substr();
	}
#else
	std::string string(const std::string& s)
	{
		return s.substr();
	}

	std::string string(const std::wstring& s)
	{
		return string_convert(s);
	}
#endif
}
