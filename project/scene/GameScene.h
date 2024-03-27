#pragma once
#include "IScene.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "Line.h"
#include "Plane.h"
#include "Particles.h"
#include "PostEffect.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Object3D.h"
#include "Animation.h"
#include "GameObject/Player.h"
#include "GameObject/Enemy.h"
#include "Collision/CollisionManager.h"

class GameManager;
class GameScene : public IScene
{
public:
	~GameScene()override = default;
	GameScene();

	// 初期化
	void Initialize()override;

	// 更新処理
	void Update()override;

	// 描画
	void Draw()override;

	void Finalize()override;
private:
	std::unique_ptr<Object3D> plane_;
	std::unique_ptr<Object3D> axis_;
	std::array<std::unique_ptr<Sprite>, 1u> sprite_;
	std::unique_ptr<Particles> particles_;
	std::unique_ptr<Particles> particles_1;

	// テクスチャ
	uint32_t uvcheckerTexture_;
	uint32_t monsterBallTexture_;
	uint32_t particleTexture_;
	uint32_t gamePlayTexture_;

	Input* input_;
	std::unique_ptr<Camera> camera_;
};

