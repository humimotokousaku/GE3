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
#include "Object3D.h"
#include "Animation.h"
#include "Collision/CollisionManager.h"
#include "GameObject/Enemy.h"
#include "GameObject/Player.h"

class GameManager;

class TitleScene : public IScene
{
public:
	/// 
	/// Default Method
	/// 

	~TitleScene()override = default;

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

private:
	std::unique_ptr<Object3D> plane_;
	std::unique_ptr<Object3D> axis_;
	std::unique_ptr<Sprite> sprite_;
	std::unique_ptr<Particles> particles_;
	std::unique_ptr<Particles> particles_1;
	std::unique_ptr<Animation> anim_;
	std::unique_ptr<Enemy> enemy_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<CollisionManager> collisionManager_;

	int textureNum_;
	Input* input_;
	ViewProjection viewProjection_;
	std::unique_ptr<Camera> camera_;
};