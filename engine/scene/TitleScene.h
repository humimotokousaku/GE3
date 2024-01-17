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
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize()override;
private:
	PostEffect* postEffect_;
	const static int kMaxCube = 2;
	Model* plane_;
	Model* axis_;
	Sprite* sprite_;
	Line* line_;

	Particles* particles_;

	int textureNum_;
	Input* input_;
	WorldTransform cubeWorldTransform_[kMaxCube];
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	float t;

	// 線分の数
	const size_t segmentCount = 100;
	// スプライン曲線制御点（通過点）
	std::vector<Vector3> controlPoints_;
};