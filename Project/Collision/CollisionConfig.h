#pragma once
#include "Vector3.h"
#include <cstdint>

/// 衝突属性
// プレイヤー陣営
const uint32_t kCollisionAttributePlayer = 0b1;
// 敵陣営
const uint32_t kCollisionAttributeEnemy = 0b1 << 1;

/// 当たり判定の形状
// 球体
const uint32_t kCollisionSphere = 0b10;
// AABB
const uint32_t kCollisionAABB = 0b10 << 1;
// カプセル
const uint32_t kCollisionCapsule = 0b10 << 2;

// 立方体
struct AABB {
	Vector3 max = { 1.0f,1.0f,1.0f };
	Vector3 min = { 1.0f,1.0f,1.0f };
};
// 球
struct cSphere {
	Vector3 center = { 0,0,0 };
	float radius = 1.0f;
};
// カプセル
struct Capsule {
	Vector3 start = { 0,0,0 };
	Vector3 end = { 3.0f,0,0 };
	float radius = 1.0f;
};