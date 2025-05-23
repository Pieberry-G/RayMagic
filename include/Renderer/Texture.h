#pragma once

#include <glm/glm.hpp>

namespace RayMagic {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetRendererID() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void SetData(void* data, uint32_t size) = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;

		static std::shared_ptr<Texture2D> Create(uint32_t width, uint32_t height);
		static std::shared_ptr<Texture2D> Create(const std::string& path);
	};

	class TextureBuffer : public Texture
	{
	public:
		virtual void SetData(const std::vector<glm::vec3>& data) = 0;

		static std::shared_ptr<TextureBuffer> Create();
	};

}