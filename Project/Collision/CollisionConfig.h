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

// 立方体
struct AABB {
	Vector3 max = { 1.0f,1.0f,1.0f };
	Vector3 min = { 1.0f,1.0f,1.0f };
};