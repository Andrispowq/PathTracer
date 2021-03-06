#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "engine/common/framework/Window.h"
#include "engine/common/texture/Texture.h"

#include "engine/core/util/math/Math.h"

namespace Prehistoric
{
	/*
		This implementation is currently only applicable for OpenGL. It will be rewritten for compatibility with Vulkan
	*/
	class Framebuffer
	{
	public:
		Framebuffer(Window* window) : window(window) {}
		virtual ~Framebuffer() {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Clear(const Vector4f& colour) = 0;
		virtual void SetDrawAttachments(uint32_t n, uint32_t* attachments) = 0; //TODO: this is OpenGL only!

		virtual void addDepthAttachment(uint32_t width, uint32_t height, bool multisample = false) = 0;

		virtual void addColourAttachment2D(Texture* texture, uint32_t attachment = 0, uint32_t mipLevel = 0) = 0;
		virtual void addColourAttachmentMultisample2D(uint32_t attachment = 0) = 0;

		virtual void addColourAttachment3D(Texture* texture, uint32_t face, uint32_t attachment = 0, uint32_t mipLevel = 0) = 0;

		virtual void Blit(Framebuffer* destination, uint32_t width, uint32_t height, uint32_t source_attachment = 0, uint32_t dest_attachment = 0) = 0;

	protected:
		Window* window;
		uint32_t width, height;
	};
};

#endif