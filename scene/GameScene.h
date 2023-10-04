#pragma once
#include "IScene.h"
// base
#include "../base/Model.h"
#include "../base/ViewProjection.h"
// GameObject
#include "../Player.h"
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
	//Input* input_;

	// 3Dモデルデータ
	std::unique_ptr<Model> model_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 自キャラ
	std::unique_ptr<Player> player_;
};

