#include "CollisionManager.h"
#include <assert.h>

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	// 衝突フィルタリング
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
		(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0) {
		return;
	}

	//// コライダーのワールド座標を取得
	//colliderA->GetWorldPosition();
	//colliderB->GetWorldPosition();

	/// 球体の判定
	if (colliderA->GetCollisionPrimitive() == kCollisionSphere && colliderB->GetCollisionPrimitive() == kCollisionSphere) {
		// 座標AとBの距離を求める
		Vector3 a2b = {
			colliderA->GetWorldPosition().x - colliderB->GetWorldPosition().x,
			colliderA->GetWorldPosition().y - colliderB->GetWorldPosition().y,
			colliderA->GetWorldPosition().z - colliderB->GetWorldPosition().z };
		float a2bR = colliderA->GetRadius() + colliderB->GetRadius();
		// 球と球の交差判定
		if ((a2b.x * a2b.x) + (a2b.y * a2b.y) + (a2b.z * a2b.z) <= (a2bR * a2bR)) {
			// コライダーAの衝突時コールバックを呼び出す
			colliderA->OnCollision();
			// コライダーBの衝突時コールバックを呼び出す
			colliderB->OnCollision();

			// 今当たっている
			colliderA->SetIsOnCollision(true);
			colliderB->SetIsOnCollision(true);
		}
		else {
			// 今は当たっていない
			colliderA->SetIsOnCollision(false);
			colliderB->SetIsOnCollision(false);
		}
	}
	if (colliderA->GetCollisionPrimitive() == kCollisionSphere && colliderB->GetCollisionPrimitive() == kCollisionAABB) {
		// 最近接点
		Vector3 closestPoint = {
			std::min(std::max(colliderA->GetWorldPosition().x, colliderB->GetWorldPosition().x + colliderB->GetAABB_Min().x), colliderB->GetWorldPosition().x + colliderB->GetAABB_Max().x),
			std::min(std::max(colliderA->GetWorldPosition().y, colliderB->GetWorldPosition().y + colliderB->GetAABB_Min().y), colliderB->GetWorldPosition().y + colliderB->GetAABB_Max().y),
			std::min(std::max(colliderA->GetWorldPosition().z, colliderB->GetWorldPosition().z + colliderB->GetAABB_Min().z), colliderB->GetWorldPosition().z + colliderB->GetAABB_Max().z)
		};

		float dist = Length(Subtract(closestPoint, colliderA->GetWorldPosition()));
		if (dist <= colliderA->GetRadius()) {
			// コライダーAの衝突時コールバックを呼び出す
			colliderA->OnCollision();
			// コライダーBの衝突時コールバックを呼び出す
			colliderB->OnCollision();
			// 今当たっている
			colliderA->SetIsOnCollision(true);
			colliderB->SetIsOnCollision(true);
		}
		else {
			// 今は当たっていない
			colliderA->SetIsOnCollision(false);
			colliderB->SetIsOnCollision(false);
		}

	}
	if ((colliderA->GetCollisionPrimitive() == kCollisionCapsule && colliderB->GetCollisionPrimitive() == kCollisionSphere)) {
		Vector3 d = Subtract(colliderB->GetWorldPosition(), colliderA->GetStartPos());
		Vector3 ba = Subtract(colliderA->GetEndPos(), colliderA->GetStartPos());
		// カプセルのベクトルの長さ
		float length = Length(ba);
		// 正規化
		Vector3 e = Normalize(ba);
		// 内積
		float dot = Dot(d, e);

		float t = dot / length;
		if (t > 1) {
			t = 1;
		}
		else if (t < 0) {
			t = 0;
		}
		// 線形補間
		Vector3 f;
		f.x = (1.0f - t) * colliderA->GetStartPos().x + t * colliderA->GetEndPos().x;
		f.y = (1.0f - t) * colliderA->GetStartPos().y + t * colliderA->GetEndPos().y;
		f.z = (1.0f - t) * colliderA->GetStartPos().z + t * colliderA->GetEndPos().z;

		// 距離
		float distance = Length(Subtract(colliderB->GetWorldPosition(), f));

		// 当たっていたらplayerが赤になる
		if (distance < colliderB->GetRadius() + colliderA->GetCapsuleRadius()) {
			// コライダーAの衝突時コールバックを呼び出す
			colliderA->OnCollision();
			// コライダーBの衝突時コールバックを呼び出す
			colliderB->OnCollision();

			// 今当たっている
			colliderA->SetIsOnCollision(true);
			colliderB->SetIsOnCollision(true);
		}
		else {
			// 今は当たっていない
			colliderA->SetIsOnCollision(false);
			colliderB->SetIsOnCollision(false);
		}
	}
	if ((colliderA->GetCollisionPrimitive() == kCollisionCapsule && colliderB->GetCollisionPrimitive() == kCollisionAABB)) {
		Vector3 d = Subtract(colliderB->GetWorldPosition(), colliderA->GetStartPos());
		Vector3 ba = Subtract(colliderA->GetEndPos(), colliderA->GetStartPos());
		// カプセルのベクトルの長さ
		float length = Length(ba);
		// 正規化
		Vector3 e = Normalize(ba);
		// 内積
		float dot = Dot(d, e);

		float t = dot / length;
		if (t > 1) {
			t = 1;
		}
		else if (t < 0) {
			t = 0;
		}
		// カプセルの線上での最短点
		Vector3 f;
		f.x = (1.0f - t) * colliderA->GetStartPos().x + t * colliderA->GetEndPos().x;
		f.y = (1.0f - t) * colliderA->GetStartPos().y + t * colliderA->GetEndPos().y;
		f.z = (1.0f - t) * colliderA->GetStartPos().z + t * colliderA->GetEndPos().z;

		// 最近接点
		Vector3 closestPoint = {
			std::min(std::max(f.x, colliderB->GetWorldPosition().x + colliderB->GetAABB_Min().x), colliderB->GetWorldPosition().x + colliderB->GetAABB_Max().x),
			std::min(std::max(f.y, colliderB->GetWorldPosition().y + colliderB->GetAABB_Min().y), colliderB->GetWorldPosition().y + colliderB->GetAABB_Max().y),
			std::min(std::max(f.z, colliderB->GetWorldPosition().z + colliderB->GetAABB_Min().z), colliderB->GetWorldPosition().z + colliderB->GetAABB_Max().z)
		};

		float dist = Length(Subtract(closestPoint, f));
		if (dist <= colliderA->GetCapsuleRadius()) {
			// コライダーAの衝突時コールバックを呼び出す
			colliderA->OnCollision();
			// コライダーBの衝突時コールバックを呼び出す
			colliderB->OnCollision();

			// 今当たっている
			colliderA->SetIsOnCollision(true);
			colliderB->SetIsOnCollision(true);
		}
		else {
			// 今は当たっていない
			colliderA->SetIsOnCollision(false);
			colliderB->SetIsOnCollision(false);
		}
	}

	// 前のフレームで当たっていたかを更新
	colliderA->SetIsPreOnCollision(colliderA->GetIsOnCollision());
	colliderB->SetIsPreOnCollision(colliderB->GetIsOnCollision());
}

void CollisionManager::CheckAllCollisions() {
	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;
			// 当たり判定と応答(フレンドリーファイアしないように設定)
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}