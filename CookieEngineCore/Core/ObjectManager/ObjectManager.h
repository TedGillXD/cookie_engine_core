#pragma once
#include "Common/Common.h"
#include "ContentLoader/ContentLoader.h"

namespace Cookie::Core {

	/**
	 * .cobject file: the csutomized file taht stores object infos
	 * Format of .object file:
	 * 1 byte: object type - object 2D or object 3D
	 * 4 integer numbers: 4 lengths of strings that indicates 4 paths for 4 textures(.asset file)
	 * 4 strings: 4 path indicates 4 textures(.asset file)
	 */


	class ObjectManager {
	private:
		// The map that stores object infos, model infos, texture infos, material infos
		// key is the file path
		// value is the data

	public:
		static Content::Object2D LoadObject2DFromFile(std::string filePath);

		static Content::Object3D LoadObject3DFromFile(std::string filePath);
	};
}