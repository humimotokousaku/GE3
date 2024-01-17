#pragma once
#include "IScene.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "Line.h"
#include "Particles.h"
#include "PostEffect.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class GameManager;

class TitleScene : public IScene
{
public:
	/// 
	/// Default Method
	/// 

	// 初期化
	void Initialize() override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Draw() override;
	// 解放処理
	void Finalize()override;

	///
	/// User Method
	/// 

	// Cat-mullRom曲線
	Vector3 CatmullRomSpline(const std::vector<Vector3>& controlPoints, float t);

private:
	PostEffect* postEffect_;
	const static int kMaxCube = 2;
	Model* plane_;
	Model* axis_;
	Sprite* sprite_;


	Particles* particles_;

	int textureNum_;
	Input* input_;
	WorldTransform cubeWorldTransform_[kMaxCube];
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	float t;

	// 線分の数
	static const size_t segmentCount = 100;
	Line* line_[segmentCount - 1];
	// スプライン曲線制御点（通過点）
	std::vector<Vector3> controlPoints_;
	// 線分で描画する用の頂点リスト
	std::vector<Vector3> pointsDrawing_;
};