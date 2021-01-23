#ifndef VERTEX_BUFFER_LOADER_H
#define VERTEX_BUFFER_LOADER_H

#include "engine/common/framework/Window.h"
#include "engine/common/buffer/MeshVertexBuffer.h"

#include "platform/opengl/buffer/GLMeshVertexBuffer.h"
#include "platform/opengl/buffer/GLPatchVertexBuffer.h"
//#include "platform/vulkan/buffer/VKPatchVertexBuffer.h"

#include "engine/core/resources/Loader.h"
#include "engine/core/model/obj/OBJLoader.h"
#include "engine/core/model/Mesh.h"

namespace Prehistoric
{
	enum class Type
	{
		Mesh, Patch
	};

	struct VertexBufferLoaderExtra : public Extra
	{
		Type type = Type::Mesh;
		std::optional<Mesh> mesh;
	};

	class VertexBufferLoader : public Loader
	{
	public:
		VertexBufferLoader(Window* window) : Loader(window) {}

		virtual void* LoadResourceInternal(const std::string& path, Extra* extra) override;
	};
};

#endif