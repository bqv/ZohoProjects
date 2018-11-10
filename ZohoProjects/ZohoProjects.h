#pragma once

class ZohoProjects
{
private:
	static const std::string scopes[];

	ZohoProjects();
public:
	static std::string authorizationUrl();
	static std::string tokenUrl();

	static const std::string baseUrl;
	static const std::string clientId;
	static const std::string clientSecret;
	static const std::string redirectUrl;
	static const std::string zohoUrl;
};

