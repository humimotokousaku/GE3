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
#include "../Player/PlayerBullet.h"
#include "../skydome/Skydome.h"
#include "../RailCamera/RailCamera.h"

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

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetPlayerBullets() const { return playerBullets_; }

	Player* GetPlayer() { return player_; }

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetEnemyBullets() const { return enemyBullets_; }

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵の発生処理
	/// </summary>
	/// <param name="pos"></param>
	void SpawnEnemy(Vector3 pos);

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	/// <summary>
	/// 自機を追加する
	/// </summary>
	/// <param name="playerBullet">自弾</param>
	void AddPlayerBullet(PlayerBullet* playerBullet);

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

private: // メンバ変数
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// テクスチャハンドル
	uint32_t playerTexture_ = 0;
	// 3Dレティクル
	uint32_t reticleTexture_ = 0;

	// 3Dモデルデータ
	Model* model_ = nullptr;
	// 天球の3Dモデル
	Model* modelSkydome_ = nullptr;

	// 敵の発生時間や座標などのCSVファイル
	std::stringstream enemyPopCommands_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 自キャラ
	Player* player_ = nullptr;
	// 自弾
	std::list<PlayerBullet*> playerBullets_;
	// 敵
	std::list<Enemy*> enemy_;
	// 敵弾
	std::list<EnemyBullet*> enemyBullets_;
	// 衝突マネージャー
	CollisionManager* collisionManager_ = nullptr;
	// 天球
	Skydome* skydome_ = nullptr;
	// カメラレール
	RailCamera* railCamera_ = nullptr;

	// 敵が発生待機中か
	bool isWait_ = false;
	// 敵が発生するまでの時間
	int32_t waitTime_ = 0;
};

