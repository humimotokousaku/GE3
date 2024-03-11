#pragma once
#include "Vector3.h"
#include "CollisionConfig.h"
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

#pragma region 球
	// 球の座標,半径を取得
	cSphere GetSphere() { return sphere_; }
	// 半径の取得
	float GetRadius() { return sphere_.radius; }
	// 中心座標を取得
	Vector3 GetSphereCenter() { return sphere_.center; }
#pragma endregion

#pragma region 立方体
	// 立方体を取得
	cAABB GetAABB() { return aabb_; }
	// minの取得
	Vector3 GetAABB_Min() { return aabb_.min; }
	// maxの取得
	Vector3 GetAABB_Max() { return aabb_.max; }
#pragma endregion

#pragma region カプセル
	// カプセルの始点,終点,半径を取得
	Capsule GetCapsule() { return capsule_; }
	// カプセルの半径の取得
	float GetCapsuleRadius() { return capsule_.radius; }
	// カプセルの始点を取得
	Vector3 GetStartPos() { return capsule_.start; }
	// カプセルの終点を取得
	Vector3 GetEndPos() { return capsule_.end; }
#pragma endregion

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

#pragma region 球
	// 球の座標,半径を設定
	void SetSphere(cSphere sphere) { sphere_ = sphere; }
	// 半径の設定
	void SetRadius(float radius) { sphere_.radius = radius; }
	// 中心座標を設定
	void SetSphereCenter(Vector3 center) { sphere_.center = center; }
#pragma endregion

#pragma region 立方体
	// 立方体を設定
	void SetAABB(cAABB aabb) { aabb_ = aabb; }
	// minの設定
	void SetAABB_Min(Vector3 min) { aabb_.min = min; }
	// maxの設定
	void SetAABB_Max(Vector3 max) { aabb_.max = max; }
#pragma endregion

#pragma region カプセル
	// カプセルの始点,終点,半径を取得
	void SetCapsule(Capsule capsule) {  capsule_ = capsule; }
	// カプセルの半径の取得
	void SetCapsuleRadius(float radius) {  capsule_.radius = radius; }
	// カプセルの始点を取得
	void SetStartPos(Vector3 start) {  capsule_.start = start; }
	// カプセルの終点を取得
	void SetEndPos(Vector3 end) {  capsule_.end = end; }
#pragma endregion

	//// OBBの設定
	//void SetOBB(OBB obb) { obb_ = obb; }
	//void SetOBBCenterPos(Vector3 centerPos) { obb_.m_Pos = centerPos; }
	//void SetOBBDirect(int index) { 
	//	Vector3 rotateResult = TransformNormal(obb_.m_NormaDirect[index], MakeRotateMatrix(GetRotation()));
	//	obb_.m_NormaDirect[index] = Normalize(rotateResult);
	//}

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
	// 球
	cSphere sphere_ = {
		{ 0,0,0 },
		1.0f
	};
	// 立方体
	cAABB aabb_ = {
		{-1.0f,-1.0f,-1.0f},
		{1.0f,1.0f,1.0f}
	};
	// カプセル
	Capsule capsule_ = {
		{0,0,0},
		{3.0f,0,0},
		1.0f
	};

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