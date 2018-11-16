#include "stdafx.h"
#include "CppUnitTest.h"

#include "../ZohoProjectsApi/zoho.h"
#include "../ZohoProjectsApi/api.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZohoProjects
{
	namespace Test
	{
		TEST_CLASS(Zoho)
		{
		public:
			TEST_METHOD(Portals_API_Calls) {
				zoho::session session;
				std::vector<zoho::portal> portals = zoho::api::portals::all_portals(session);
				Assert::IsTrue(portals.size() > 0);
			}

			TEST_METHOD(Projects_API_Calls) {
				zoho::session session;
				std::vector<zoho::portal> portals = zoho::api::portals::all_portals(session);
				Assert::IsTrue(portals.size() > 0);
				std::vector<zoho::project> projects = zoho::api::projects::all_projects(session, portals[0]);
				Assert::IsTrue(projects.size() > 0);
			}
		};
	};
};
