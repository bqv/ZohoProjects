#pragma once

#ifdef ZOHOPROJECTS
#pragma warning(disable:4374)
#pragma warning(disable:4373)
#pragma warning(disable:4251)
#pragma warning(disable:4275)
#ifdef ZOHOAPI
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif
#endif
