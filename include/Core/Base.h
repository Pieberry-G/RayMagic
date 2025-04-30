#pragma once

#ifdef RM_PLATFORM_WINDOWS
	#ifdef RM_BUILD_DLL
		#ifdef RM_BUILD_DLL
			#define RayMagic_API __declspec(dllexport)
		#else
			#define RayMagic_API __declspec(dllimport)
		#endif
	#else
		#define RayMagic_API
	#endif
#else
	#error RayMagic only support Windows!
#endif

#ifdef RM_ENABLE_ASSERTS
	#define RM_ASSERT(x, ...) { if(!(x)) { RM_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define RM_CORE_ASSERT(x, ...) { if(!(x)) { RM_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define RM_ASSERT(x, ...)
	#define RM_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define RM_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)