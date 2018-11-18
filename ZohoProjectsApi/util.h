#pragma once

#include <string>

#define JSON_GET_STRING(json, key, value) (json.has_field(U(key)) ? json.at(U(key)).as_string() : U(value))
#define JSON_GET_BOOL(json, key, value) (json.has_field(U(key)) ? json.at(U(key)).as_bool() : value)
#define JSON_GET_INTEGER(json, key, value) (json.has_field(U(key)) ? json.at(U(key)).as_integer() : value)
#define JSON_GET_LONG(json, key, value) (json.has_field(U(key)) ? json.at(U(key)).as_number().to_int64() : value)
#define JSON_GET_ARRAY(json, key, value) (json.has_field(U(key)) ? json.at(U(key)).as_array() : value)

#define JSON_GET_GET_STRING(json, key1, key2, value) (json.has_field(U(key1)) ? JSON_GET_STRING(json.at(U(key1)), key2, value) : U(value))
#define JSON_GET_GET_INTEGER(json, key1, key2, value) (json.has_field(U(key1)) ? JSON_GET_INTEGER(json.at(U(key1)), key2, value) : value)

#define JSON_GET_GET_GET_STRING(json, key1, key2, key3, value) (json.has_field(U(key1)) ? JSON_GET_GET_STRING(json.at(U(key1)), key2, key3, value) : U(value))

namespace util
{
	std::wstring string_convert(const std::string&);
	std::string string_convert(const std::wstring&);

#ifdef UNICODE
	std::wstring string(const std::string&);
	std::wstring string(const std::wstring&);
#else
	std::string string(const std::string&);
	std::string string(const std::wstring&);
#endif
}
