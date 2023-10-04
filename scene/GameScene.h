#pragma once
#include "IScene.h"
// base
#include "../base/Model.h"
#include "../base/ViewProjection.h"
// components
#include "../components/Input.h"
// GameObject
#include "../Player.h"
#include "../Ground.h"
#include "../Skydome.h"
// c
#include <memory>

class GameManager;
class GameScene : public IScene
{
public:
	// 初期化
	void Initialize()override;

	// 更新処理
	void Update()override;

	// 描画
	void Draw()override;

	void Finalize()override;
private:
	// 入力
	Input* input_;

	// 3Dモデルデータ
	std::unique_ptr<Model> playerModel_;
	std::unique_ptr<Model> groundModel_;
	std::unique_ptr<Model> skydomeModel_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 自キャラ
	std::unique_ptr<Player> player_;
	// 地面
	std::unique_ptr<Ground> ground_;
	// 天球
	std::unique_ptr<Skydome> skydome_;
};

