#pragma once
#include "IScene.h"
#include "../components/WorldTransform.h"
#include "../components/camera/ViewProjection.h"
#include "../components/input/Input.h"
#include "../components/model/Model.h"
#include "../object/Cube.h"
#include "../object/Sprite.h"
#include "../object/Sphere.h"

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