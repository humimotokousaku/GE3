#pragma once
#include "../../base/Model.h"
#include "../../base/WorldTransform.h"
#include "../ICharacter.h"

#include <optional>

class Player : public ICharacter{
public:
	Player();
	~Player();
	/// <summary>
	/// 初期化
	/// <summary>
	void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 更新
	/// <summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// <summary>
	void Draw(const ViewProjection& viewProjection, uint32_t textureHandle) override;

	// 浮遊ギミック初期化
	void InitializeFloatingGimmick();
	// 浮遊ギミック更新
	void UpdateFloatingGimmick();
	// 遷移行動更新
	void BehaviorRootUpdate();
	// 通常行動初期化
	void BehaviorRootInitialize();
	// 攻撃行動初期化
	void BehaviorAttackInitialize();
	// 攻撃行動更新
	void BehaviorAttackUpdate();
	// ダッシュ行動初期化
	void BehaviorDashInitialize();
	// ダッシュ行動更新
	void BehaviorDashUpdate();

	// 自機のワールド座標
	Vector3 GetWorldPosition();

	const WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }
	const WorldTransform& GetWorldTransformBase() { return worldTransform_; }

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	// パーツの親子関係
	void SetParent(const WorldTransform* parent);

	void SetCameraParent(const WorldTransform* parent);

private:
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// ワールド変換データ
	//WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	// 武器
	WorldTransform worldTransformHammer_;

	// 振るまい
	enum class Behavior {
		kRoot,	 // 通常状態
		kAttack, // 攻撃中
		kDash	 // ダッシュ中
	};
	Behavior behavior_ = Behavior::kRoot;
	// 次の振るまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	
	// ダッシュ用ワーク
	struct WorkDash {
		// ダッシュ用の媒介変数
		uint32_t dashParameter_ = 0;
	};
	WorkDash workDash_;
	// ダッシュの移動量
	Vector3 dashMove_;

	// 攻撃フレーム
	int attackAnimationFrame;

	// テクスチャ
	uint32_t playerTexture_;

	const uint16_t kMaxMoveModelParts = 2;
	// 浮遊ギミックの媒介変数
	float floatingParameter_[2];


	// 目標角度
	float goalAngle_;

};
