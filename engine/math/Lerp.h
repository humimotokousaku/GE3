#pragma once
#include "MathStructs.h"
#include <vector>

namespace Lerps {
	// 線形補間
	Vector3 Lerp(Vector3 start, Vector3 end, float t);
	// 球面線形補間
	Vector3 Slerp(Vector3 start, Vector3 end, float t);
	// Cat-mullRom曲線
	Vector3 CatmullRomSpline(const std::vector<Vector3>& controlPoints, float t);
};

namespace Easings {
#pragma region Quint

	// だんだん減速(ぎりぎりまで速度が速い)
	float easeOutQuint(float x);

	// だんだん早くなる(一気に早くなる)
	float easeInQuint(float x);

	float easeInOutQuint(float x);

#pragma endregion

#pragma region Circ

	float easeInCirc(float x);

	float easeOutCirc(float x);

	float easeInOutCirc(float x);

#pragma endregion

#pragma region Cubic

	float easeInCubic(float x);

	float easeOutCubic(float x);

	float easeInOutCubic(float x);

#pragma endregion

#pragma region Back

	float easeInBack(float x);

	float easeOutBack(float x);

	float easeInOutBack(float x);

#pragma endregion

#pragma region Bounce

	float easeOutBounce(float x);

	float easeInBounce(float x);

	float easeInOutBounce(float x);

#pragma endregion

#pragma region Elastic

	float easeInElastic(float x);

	float easeOutElastic(float x);

	float easeInOutElastic(float x);

#pragma endregion

#pragma region Quart

	float easeInQuart(float x);

	float easeOutQuart(float x);

	float easeInOutQuart(float x);

#pragma endregion

#pragma region Expo

	float easeInExpo(float x);

	float easeOutExpo(float x);

	float easeInOutExpo(float x);

#pragma endregion
}