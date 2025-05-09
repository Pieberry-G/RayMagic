#pragma once

#include <random>
#include <glm/glm.hpp>

namespace RayMagic {

	class Random
	{
	public:
		static void Init()
		{
			s_RandomEngine.seed(std::random_device()());
		}

		static uint32_t UInt()
		{
			return s_Distribution(s_RandomEngine);
		}

		static uint32_t UInt(uint32_t min, uint32_t max)
		{
			return min + (s_Distribution(s_RandomEngine) % (max - min + 1));
		}

		static float Float()
		{
			return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
		}

		static glm::vec3 Vec3()
		{
			return glm::vec3(Float(), Float(), Float());
		}

		static glm::vec3 Vec3(float min, float max)
		{
			return glm::vec3(Float() * (max - min) + min, Float() * (max - min) + min, Float() * (max - min) + min);
		}

		// 单位球内的随机向量
	    static glm::vec3 InUnitSphere()
	    {
	        glm::vec3 d;
	        do
	        {
	            d = 2.0f * Vec3() - glm::vec3(1, 1, 1);
	        } while (dot(d, d) > 1.0);
	        return normalize(d);
	    }
	
	    // 法向半球随机向量
	    static glm::vec3 InNormalHemisphere(const glm::vec3& n)
	    {
	        return normalize(InUnitSphere() + n);
	    }

	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};

}


