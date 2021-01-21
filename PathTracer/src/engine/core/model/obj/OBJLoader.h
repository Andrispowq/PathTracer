#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "engine/core/model/Mesh.h"

#include "engine/core/util/Constants.h"
#include "engine/core/config/FrameworkConfig.h"

namespace Prehistoric
{
	namespace OBJLoader
	{
		Mesh LoadMesh(const std::string& path, const std::string& objectFile, const std::string& materialFile);

		void CalculateTangents(Vertex& v0, Vertex& v1, Vertex& v2);
	};
};

#endif