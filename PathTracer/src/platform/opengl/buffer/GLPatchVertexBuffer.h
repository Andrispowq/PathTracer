#ifndef GL_PATCH_VERTEX_BUFFER_H
#define GL_PATCH_VERTEX_BUFFER_H

#include "engine/common/buffer/PatchVertexBuffer.h"

#include <glad/glad.h>

namespace Prehistoric
{
	class GLPatchVertexBuffer : public PatchVertexBuffer
	{
	public:
		GLPatchVertexBuffer(Window* window, const std::vector<Vector2f>& vertices);
		virtual ~GLPatchVertexBuffer() override;

		void Bind(CommandBuffer* commandBuffer) const override;
		void Draw(CommandBuffer* commandBuffer) const override;
		void Unbind() const override;
	private:
		GLuint vao;
		GLuint vbo;
	};
};

#endif