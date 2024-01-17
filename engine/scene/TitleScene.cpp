#include "TitleScene.h"
#include "ImGuiManager.h"

void TitleScene::Initialize() {
	input_ = Input::GetInstance();
	viewProjection_.Initialize();

	//// objモデル
	//plane_ = Model::CreateModelFromObj("engine/resources", "plane.obj");
	//axis_ = Model::CreateModelFromObj("engine/resources", "teapot.obj");

	//// Sprite
	//sprite_ = Sprite::Create(UVCHEKER);

	// 線
	for (int i = 0; i < segmentCount - 1; i++) {
		line_[i] = new Line();
		line_[i]->Initialize();
	}

	//// particle
	//particles_ = new Particles();
	//particles_->Initialize();

	t = 0;

	// スプライン曲線制御点（通過点）の初期化
	controlPoints_ = {
		{0,  0,  0},
		{10, 10, 0},
		{10, 15, 0},
		{20, 15, 0},
		{20, 0,  0},
		{30, 0,  0}
	};
}

void TitleScene::Update() {
	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();

	//plane_->worldTransform.translation_ = Lerp(Vector3{ 0,0,0 }, Vector3{ 5,5,5 }, t);

	// パーティクルの発生位置を指定
	//particles_->SetEmitterPos(Vector3{ 5,5,0 });
	// パーティクルの更新処理
	//particles_->Update();

	// 線分の数+1個分の頂点座標の計算
	for (size_t i = 0; i < segmentCount + 1; i++) {
		float t = 1.0f / segmentCount * i;

		Vector3 pos = CatmullRomSpline(controlPoints_, t);
		// 描画用頂点リストに追加
		pointsDrawing_.push_back(pos);
	}

#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::DragFloat3("translation", &viewProjection_.translation_.x, 0.1f, -100, 100);
	ImGui::DragFloat3("rotation", &viewProjection_.rotation_.x, 0.01f, -6.28f, 6.28f);
	ImGui::End();
	ImGui::Begin("Lerp");
	ImGui::DragFloat("t", &t, 0.001f, 0, 1);
	ImGui::End();

#endif
	//sprite_->ImGuiAdjustParameter();
	//line_->ImGuiAdjustParameter();
}

void TitleScene::Draw() {
	//axis_->Draw(viewProjection_, UVCHEKER);
	//plane_->Draw(viewProjection_, UVCHEKER);
	//particles_->Draw(viewProjection_, PARTICLE);
	//sprite_->Draw();
	// 線
	for (int i = 0; i < segmentCount - 1; i++) {
		line_[i]->Draw(pointsDrawing_[i], pointsDrawing_[i + 1], viewProjection_);
	}
}

void TitleScene::Finalize() {
	for (int i = 0; i < segmentCount - 1; i++) {
		delete line_[i];
	}
	//delete axis_;
	//delete plane_;
	//delete sprite_;
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
	//delete particles_;
}

Vector3 TitleScene::CatmullRomSpline(const std::vector<Vector3>& controlPoints, float t) {
	int n = (int)controlPoints.size();
	int segment = static_cast<int>(t * (n - 1));
	float tSegment = t * (n - 1) - segment;

	Vector3 p0 = controlPoints[segment > 0 ? segment - 1 : 0];
	Vector3 p1 = controlPoints[segment];
	Vector3 p2 = controlPoints[segment < n - 1 ? segment + 1 : n - 1];
	Vector3 p3 = controlPoints[segment < n - 2 ? segment + 2 : n - 1];

	Vector3 interpolatedPoint;
	interpolatedPoint.x =
		0.5f * ((2.0f * p1.x) + (-p0.x + p2.x) * tSegment +
			(2.0f * p0.x - 5.0f * p1.x + 4.0f * p2.x - p3.x) * (tSegment * tSegment) +
			(-p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x) * (tSegment * tSegment * tSegment));
	interpolatedPoint.y =
		0.5f * ((2.0f * p1.y) + (-p0.y + p2.y) * tSegment +
			(2.0f * p0.y - 5.0f * p1.y + 4.0f * p2.y - p3.y) * (tSegment * tSegment) +
			(-p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y) * (tSegment * tSegment * tSegment));
	interpolatedPoint.z =
		0.5f * ((2.0f * p1.z) + (-p0.z + p2.z) * tSegment +
			(2.0f * p0.z - 5.0f * p1.z + 4.0f * p2.z - p3.z) * (tSegment * tSegment) +
			(-p0.z + 3.0f * p1.z - 3.0f * p2.z + p3.z) * (tSegment * tSegment * tSegment));

	return interpolatedPoint;
}