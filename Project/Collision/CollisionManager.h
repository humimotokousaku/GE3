#pragma once
#include "CollisionConfig.h"
#include "Collider.h"
#include <list>

class CollisionManager {
public:
	/// <summary>
	/// 当たっているなら衝突応答関数を呼ぶ
	/// </summary>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	/// <summary>
	/// 当たっているかの確認
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// コライダーリストをすべてクリア
	/// </summary>
	void ClearColliderList() {
		colliders_.clear();
	}
	/// <summary>
	/// 特定のコライダーをクリア
	/// </summary>
	/// <param name="collider"></param>
	void ClearColliderList(Collider* collider) {
		colliders_.remove(collider);
	}

	/// Setter
	
	/// <summary>
	/// コライダーリストに登録
	/// </summary>
	/// <param name="collider">当たり判定をつけたいオブジェクト</param>
	void SetColliderList(Collider* collider) { 
		colliders_.push_back(collider); 
	}

private:
	// コライダーリスト
	std::list<Collider*> colliders_;
};