#include "Camera.h"

Camera::~Camera() {
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
}

void Camera::Initialize() {
	viewProjection_.Initialize();
}

void Camera::Update() {
	viewProjection_.UpdateMatrix();
}