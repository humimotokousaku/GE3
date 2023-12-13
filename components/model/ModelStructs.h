#pragma once
#include "../../math/Vector2.h"
#include "../../math/Vector3.h"
#include "../../math/Vector4.h"
#include "../../math/Matrix4x4.h"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <stdint.h>

struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};
struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};
struct MaterialData {
	std::string textureFilePath;
};

MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
};