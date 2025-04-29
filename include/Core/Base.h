#pragma once

#include <memory>

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

namespace RayMagic {

	template <typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}