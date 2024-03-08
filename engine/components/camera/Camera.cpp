#include "Camera.h"

Camera::~Camera() {

}

void Camera::Initialize() {
	// トランスフォーム
	transform_ = {
		{1,1,1},
		{0,0,0},
		{0,0,0}
	};
	CreateViewProjection();
}

void Camera::Update() {
	UpdateMatrix();
	if (Input::GetInstance()->PressKey(DIK_RIGHT)) {
		transform_.translate.x += 0.01f;
	}
	if (Input::GetInstance()->PressKey(DIK_LEFT)) {
		transform_.translate.x -= 0.01f;
	}
	if (Input::GetInstance()->PressKey(DIK_UP)) {
		transform_.translate.y += 0.01f;
	}
	if (Input::GetInstance()->PressKey(DIK_DOWN)) {
		transform_.translate.y -= 0.01f;
	}
}

void Camera::CreateViewProjection() {
	CreateConstBuffer();
	Map();
	UpdateMatrix();
	TransferMatrix();
}

void Camera::CreateConstBuffer() {
	viewProjection_.constBuff_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(ConstBufferDataViewProjection));
}

void Camera::Map() {
	viewProjection_.constBuff_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&viewProjection_.constMap));
}

void Camera::UpdateMatrix() {
	UpdateViewMatrix();
	UpdateProjectionMatrix();
	TransferMatrix();
}

void Camera::TransferMatrix() {
	viewProjection_.constMap->view = viewProjection_.matView;
	viewProjection_.constMap->projection = viewProjection_.matProjection;
}

void Camera::UpdateViewMatrix() {
	Matrix4x4 cameraMatrix = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewProjection_.matView = Inverse(cameraMatrix);
}

void Camera::UpdateProjectionMatrix() {
	viewProjection_.matProjection = MakePerspectiveFovMatrix(viewProjection_.fovAngleY, viewProjection_.aspectRatio, viewProjection_.nearZ, viewProjection_.farZ);
}