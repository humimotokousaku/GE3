#pragma once
#include "IScene.h"

#include "../base/Model.h"
#include "../base/ViewProjection.h"
#include "../base/WorldTransform.h"

#include "../components/Audio.h"
#include "../components/Input.h"

#include "../Collision/Collider.h"
#include "../Collision/CollisionManager.h"

#include "../object/Sprite.h"

#include "../Enemy/Enemy.h"
#include "../Player/Player.h"
#include "../skydome/Skydome.h"

class GameManager;
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public IScene
{
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	void Finalize()override;

private: // メンバ変数
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// テクスチャハンドル
	uint32_t playerTexture_ = 0;
	// 3Dモデルデータ
	Model* model_ = nullptr;

	// 天球の3Dモデル
	Model* modelSkydome_ = nullptr;
	Skydome* skydome_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 自キャラ
	Player* player_ = nullptr;

	// enemy
	Enemy* enemy_ = nullptr;

	// 衝突マネージャー
	CollisionManager* collisionManager_ = nullptr;
};

