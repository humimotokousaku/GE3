#include "GameScene.h"
#include "../Manager/GameManager.h"
#include "../Manager/ImGuiManager.h"
#include "../Manager/TextureManager.h"
#include "SceneTransition/SceneTransition.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

}

void GameScene::Initialize() {
	sceneNum = GAME_SCENE;

	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	LoadEnemyPopData();

	// ファイル名を指定してテクスチャを読み込む
	playerTexture_ = PLAYER;
	// 3Dモデルの生成
	model_ = Model::CreateModelFromObj("resources", "block.obj");

	modelSkydome_ = Model::CreateModelFromObj("resources/Skydome", "skydome.obj");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//viewProjection_.translation_ = { 0,0,-40 };
	worldTransform_.translation_.z = -10;

	// カメラレールの生成
	railCamera_ = new RailCamera();
	// カメラレールの初期化
	railCamera_->Initialize(worldTransform_, worldTransform_.rotation_);

	// 自キャラの生成
	player_ = new Player();
	// 初期位置
	Vector3 playerPosition(0, -2, 10);
	// 自キャラの初期化
	player_->Initialize(model_, playerTexture_, playerPosition);
	player_->SetGameScene(this);
	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	// 天球
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, { 0, 0, 0 });

	// 線
	for (int i = 0; i < segmentCount; i++) {
		line_[i] = new Line();
		line_[i]->Initialize();
	}

	// スプライン曲線制御点（通過点）の初期化
	controlPoints_ = {
		{0,  0,  0},
		{0,  0,  30},
		{10, 10, 10},
		{10, 15, 0},
		{20, 15, 20},
		{20, 0,  0},
		{30, 0,  -10}
	};
	t_ = 0;
	targetT_ = 1.0f / segmentCount;

	isMoveCamera_ = false;

	// 衝突マネージャーの生成
	collisionManager_ = new CollisionManager();
}

void GameScene::Update() {
	// シーンチェンジ
	if (input_->TriggerKey(DIK_SPACE)) {
		SceneTransition::sceneChangeType_ = FADE_IN;
	}
	if (SceneTransition::GetInstance()->GetSceneChangeSignal()) {
		sceneNum = GAMECLEAR_SCENE;
	}

	// 敵の出現するタイミングと座標
	UpdateEnemyPopCommands();
	// 敵の削除
	enemy_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
		});
	// enemyの更新
	for (Enemy* enemy : enemy_) {
		enemy->Update();
	}

	// 弾の更新
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}
	// 終了した弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
		});
	// 自キャラの更新
	player_->Update(viewProjection_);
	// 終了した弾を削除
	playerBullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});
	// 弾の更新
	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Update();
	}

	// 天球
	skydome_->Update();

	// 線分の数+1個分の頂点座標の計算
	for (size_t i = 0; i < segmentCount + 1; i++) {
		float t = 1.0f / segmentCount * i;
		Vector3 pos = CatmullRomSpline(controlPoints_, t);
		// 描画用頂点リストに追加
		pointsDrawing_.push_back(pos);
	}
	if (isMoveCamera_) {
		// カメラの移動
		if (t_ < 0.99f) {
			t_ += 1.0f / segmentCount / 10;
		}
		else {
			t_ = 0.99f;
			SceneTransition::sceneChangeType_ = FADE_IN;
			isMoveCamera_ = false;
		}
		if (targetT_ < 0.99f) {
			targetT_ += 1.0f / segmentCount / 10;
		}
		else {
			targetT_ = 1.0f;
		}
	}
	target_ = CatmullRomSpline(controlPoints_, targetT_);
	UpdatePlayerPosition(t_);
	viewProjection_.UpdateMatrix();
	// デバッグカメラの更新
	railCamera_->Update(target_);
	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
	// ビュープロジェクション行列の転送
	viewProjection_.TransferMatrix();

	// 当たり判定を必要とするObjectをまとめてセットする
	collisionManager_->SetGameObject(player_, enemy_, enemyBullets_, playerBullets_);
	// 衝突マネージャー(当たり判定)
	collisionManager_->CheckAllCollisions(this, player_);

	if (SceneTransition::GetInstance()->GetSceneChangeSignal()) {
		sceneNum = GAMECLEAR_SCENE;
	}

	ImGui::Begin("RailParameter");
	ImGui::DragFloat("target", &targetT_, 0.001f, 0, 1);
	ImGui::DragFloat("eye", &t_, 0.001f, 0, 1);
	ImGui::Checkbox("isStart", &isMoveCamera_);
	ImGui::End();
}

void GameScene::Draw() {
	// 自機
	player_->Draw(viewProjection_);
	// 自弾
	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Draw(viewProjection_);
	}

	// 敵
	for (Enemy* enemy : enemy_) {
		enemy->Draw(viewProjection_);
	}
	// 敵弾
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}

	// 天球
	skydome_->Draw(viewProjection_);

	// 2Dレティクル
	player_->DrawUI();
	// 線
	for (int i = 0; i < segmentCount - 1; i++) {
		line_[i]->Draw(pointsDrawing_[i], pointsDrawing_[i + 1], viewProjection_);
	}
}

void GameScene::Finalize() {
	controlPoints_.clear();
	pointsDrawing_.clear();
	enemyPopCommands_.clear();
	for (int i = 0; i < segmentCount; i++) {
		delete line_[i];
	}
	// 3Dモデル
	delete model_;
	// 自キャラの解放
	delete player_;
	playerBullets_.clear();
	for (PlayerBullet* bullet : playerBullets_) {
		delete bullet;
	}
	// enemyの解放
	enemy_.clear();
	for (Enemy* enemy : enemy_) {
		delete enemy;
	}
	enemyBullets_.clear();
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	// 衝突マネージャーの解放
	delete collisionManager_;
	// 3Dモデル
	delete modelSkydome_;
	// 天球
	delete skydome_;
	// カメラレール
	delete railCamera_;
	// ビュー(カメラ)の解放
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();

	worldTransform_.constBuff_.ReleaseAndGetAddressOf();
}

Vector3 GameScene::CatmullRomSpline(const std::vector<Vector3>& controlPoints, float t) {
	int n = (int)controlPoints.size();
	int segment = static_cast<int>(t * (n - 1));
	float tSegment = t * (n - 1) - segment;

	Vector3 p0 = controlPoints[segment > 0 ? segment - 1 : 0];
	Vector3 p1 = controlPoints[segment];
	Vector3 p2 = controlPoints[segment < n - 1 ? segment + 1 : n - 1];
	Vector3 p3 = controlPoints[segment < n - 2 ? segment + 2 : n - 1];

	Vector3 interpolatedPoint;
	interpolatedPoint.x =
		0.5f * ((2.0f * p1.x) + (-p0.x + p2.x) * tSegment +
			(2.0f * p0.x - 5.0f * p1.x + 4.0f * p2.x - p3.x) * (tSegment * tSegment) +
			(-p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x) * (tSegment * tSegment * tSegment));
	interpolatedPoint.y =
		0.5f * ((2.0f * p1.y) + (-p0.y + p2.y) * tSegment +
			(2.0f * p0.y - 5.0f * p1.y + 4.0f * p2.y - p3.y) * (tSegment * tSegment) +
			(-p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y) * (tSegment * tSegment * tSegment));
	interpolatedPoint.z =
		0.5f * ((2.0f * p1.z) + (-p0.z + p2.z) * tSegment +
			(2.0f * p0.z - 5.0f * p1.z + 4.0f * p2.z - p3.z) * (tSegment * tSegment) +
			(-p0.z + 3.0f * p1.z - 3.0f * p2.z + p3.z) * (tSegment * tSegment * tSegment));

	return interpolatedPoint;
}

void GameScene::UpdatePlayerPosition(float t) {
	Vector3 cameraPosition{};
	// Catmull-Romスプライン関数で補間された位置を取得
	cameraPosition = CatmullRomSpline(controlPoints_, t);
	cameraPosition.y += 0.01f;
	cameraPosition.z += 0.01f;
	railCamera_->SetTranslation(cameraPosition);
}

void GameScene::SpawnEnemy(Vector3 pos) {
	Enemy* enemy = new Enemy();
	// 自機の位置をもらう
	enemy->SetPlayer(player_);
	// 初期化
	enemy->SetGameScene(this);
	enemy->Initialize(model_, pos);

	// リストに登録
	enemy_.push_back(enemy);
}

void GameScene::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("resources/csv/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands_ << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (isWait_) {
		waitTime_--;
		if (waitTime_ <= 0) {
			// 待機完了
			isWait_ = false;
		}
		return;
	}

	// 1桁分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands_, line)) {
		// 1桁の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			SpawnEnemy(Vector3(x, y, z));
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待ち時間
			isWait_ = true;
			waitTime_ = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::AddPlayerBullet(PlayerBullet* playerBullet) {
	// リストに登録する
	playerBullets_.push_back(playerBullet);
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}