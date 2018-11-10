#pragma once

class zoho
{
private:
	static const std::string scopes[];

	zoho();
public:
	static std::string authorization_url();
	static std::string token_url();

	static const std::string base_url;
	static const std::string client_id;
	static const std::string client_secret;
	static const std::string redirect_url;
	static const std::string accounts_url;
};

