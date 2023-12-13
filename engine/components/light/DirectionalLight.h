#pragma once
#include "MathStructs.h"

struct DirectionalLight {
	Vector4 color;
	Vector3 direction;
	float intensity;
};