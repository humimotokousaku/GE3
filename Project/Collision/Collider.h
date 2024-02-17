#pragma once
#include "Vector3.h"
#include <cstdint>

class Collider {
public:
	// 当たった瞬間を取得
	bool TriggerOnCollision();
	// 離れた瞬間を取得
	bool ReleaseOnCollision();
	// 当たっているときを取得
	bool PressOnCollision();

	///
	/// Getter
	/// 
	
	// 半径の取得
	float GetRadius() { return radius_; }

	// 衝突属性(自分)を取得
	uint32_t GetCollisionAttribute() { return collisionAttribute_; }
	// 衝突マスク(相手)を取得
	uint32_t GetCollisionMask() { return collisionMask_; }
	// 当たり判定の形状を取得
	uint32_t GetCollisionPrimitive() { return collisionPrimitive_; }

	// 今当たっているかを設定
	bool GetIsOnCollision() { return isOnCollision_; }
	// 今当たっているかを設定
	bool GetIsPreOnCollision() { return isPreOnCollision_; }

	///
	/// Setter
	///
	
	// 半径の設定
	void SetRadius(float radius) { radius_ = radius; }

	// 衝突属性(自分)を設定
	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }
	// 衝突マスク(相手)を設定
	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }
	// 当たり判定の形状を設定
	void SetCollisionPrimitive(uint32_t collisionPrimitive) { collisionPrimitive_ = collisionPrimitive; }

	// 今当たっているかを設定
	void SetIsOnCollision(bool isOnCollision) { isOnCollision_ = isOnCollision; }
	// 今当たっているかを設定
	void SetIsPreOnCollision(bool isPreOnCollision) { isPreOnCollision_ = isPreOnCollision; }

	///
	/// 純粋仮想関数
	/// 

	// 衝突時に呼ばれる関数
	virtual void OnCollision() = 0;
	// ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;

private:
	// 衝突半径
	float radius_ = 1.0f;

	// 衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;
	// 当たり判定の形状
	uint32_t collisionPrimitive_;

	// コライダーリストから削除
	bool isClear_ = false;
	// 今当たっている
	bool isOnCollision_ = false;
	// 前に当たっている
	bool isPreOnCollision_ = false;
};