#include "GameManager.h"

GameManager::~GameManager() {
	Framework::~Framework();
}

void GameManager::Initialize() {
	Framework::Initialize();
}

void GameManager::Update() {
	Framework::Update();

#ifdef _DEBUG
	// FPSカウンターの表示
	ImGui::Begin("Control panel");
	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	ImGui::End();
#endif // _DEBUG
}

void GameManager::Draw() {
	sceneArr_[sceneNum_]->Draw();
}

void GameManager::BeginFrame() {
	Framework::BeginFrame();
}

void GameManager::EndFrame() {
	Framework::EndFrame();
}

void GameManager::Finalize() {
	Framework::Finalize();
}