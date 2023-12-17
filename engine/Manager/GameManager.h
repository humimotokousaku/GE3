#pragma once

#pragma region include
 
// Scene
#include "IScene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "GameClear.h"
#include "GameOver.h"

// Manager
#include "PipelineManager.h"

// Base
#include "WinApp.h"
#include "DirectXCommon.h"

// components
#include "Light.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "Input.h"
#include "Audio.h"

#pragma endregion

class GameManager
{	
public:
	///
	/// Default Method
	/// 
	// コンストラクタ
	GameManager();

	// デストラクタ
	//~GameManager();

	// 初期化
	void Initialize();

	// 更新処理
	//void Update();

	// 解放処理
	void Finalize();

	///
	/// user method
	///
	// ループ処理
	void Run();

	// 描画前の処理
	void BeginFrame();

	// 描画後の処理
	void EndFrame();

	// ImGuiのパラメータを入れる
	void ImGuiAdjustParameter();

private:
	// base
	WinApp* winApp_;
	DirectXCommon* directXCommon_;
	PipelineManager* pipelineManager_;

	// components
	Camera* camera_;
	DebugCamera* debugCamera_;
	Light* light_;
	PointLight* pointLight_;
	SpotLight* spotLight_;

	TextureManager* textureManager_;
	ImGuiManager* imGuiManager_;
	Input* input_;
	Audio* audio_;
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_;
	SoundData soundData1_;

	// scene
	IScene* sceneArr_[4];
	int sceneNum_;
	int preSceneNum_;
};

