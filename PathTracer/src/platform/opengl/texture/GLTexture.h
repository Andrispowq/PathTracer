#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include "engine/common/texture/Texture.h"

#include <glad/glad.h>

namespace Prehistoric
{
	class GLTexture : public Texture
	{
	public:
		GLTexture(uint32_t width, uint32_t height, ImageFormat format = R8G8B8A8_LINEAR, ImageType type = TEXTURE_2D, bool multisample = false);
		GLTexture(uint32_t id, uint32_t width, uint32_t height, ImageFormat format = R8G8B8A8_LINEAR, ImageType type = TEXTURE_2D);
		GLTexture();

		virtual ~GLTexture() override;

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Unbind() const override;

		virtual void UploadTextureData(unsigned char* data, ImageFormat format) override;
		virtual void Generate() override {}

		void UploadHDRTextureData(float* data);

		virtual void SamplerProperties(SamplerFilter filter, TextureWrapMode wrapMode, bool generate_mipmaps = true) override;
		virtual void GenerateMipmaps() override;

		inline GLuint getTextureID() const { return id; }
		inline bool isMultisample() const { return multisample; }
	public:
		//These methods give the caller the responsibility of deleting the generated textures! They must be registered to the AssetManagers list
		static Texture* Storage2D(uint32_t width, uint32_t height, uint32_t levels = 1, ImageFormat format = R8G8B8A8_LINEAR, SamplerFilter filter = Trilinear, TextureWrapMode wrapMode = Repeat, bool generate_mipmaps = true, bool multisample = false);
		static Texture* Storage3D(uint32_t width, uint32_t height, uint32_t levels = 1, ImageFormat format = R8G8B8A8_LINEAR, SamplerFilter filter = Trilinear, TextureWrapMode wrapMode = Repeat, bool generate_mipmaps = true);

		static GLenum getImageType(ImageType imageType, bool multisample);
		static GLenum getInternalFormat(ImageFormat format);
		static GLenum getFormat(ImageFormat format);
		static GLenum getFormatForComputeShader(ImageFormat format);
	private:
		GLuint id;
		bool multisample = false;
	};
};

#endif