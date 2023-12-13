#pragma once
#include "IScene.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
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
	const static int kMaxCube = 2;
	Model* block_;
	Model* axis_;
	Sprite* sprite_;

	int textureNum_;
	Input* input_;
	Vector3 pos_;
	WorldTransform cubeWorldTransform_[kMaxCube];
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};