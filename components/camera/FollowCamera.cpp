#include "FollowCamera.h"
#include "../Input.h"

void FollowCamera::Initialize() {
	viewProjection_.Initialize();
}

void FollowCamera::Update() {
	if (target_) {
		// 追従対象からカメラまでのオフセット
		Vector3 offset = TargetOffset();

		// 追従座標の補間
		interTarget_ = Lerp(interTarget_, target_->translation_, 0.2f);

		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = Add(interTarget_, offset);
	}
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// デッドゾーンの設定
		SHORT rightThumbX = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbRX);
		const float kRadian = 0.04f;

		destinationAngleY_ += rightThumbX / SHRT_MAX * kRadian;
	}
	viewProjection_.rotation_.y =
		LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, 0.1f);

	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();
}

void FollowCamera::Reset() {
	// 追従対象がいる場合
	if (target_) {
		// 追従座標・角度の初期化
		interTarget_ = target_->translation_;
		viewProjection_.rotation_.y = target_->rotation_.y;
	}
	destinationAngleY_ = viewProjection_.rotation_.y;

	// 追従対象からのオフセット
	Vector3 offset =
		viewProjection_.translation_ = Add(interTarget_, offset);
}

Vector3 FollowCamera::TargetOffset() const {
	// 追従対象からのオフセット
	Vector3 offset = { 0, 8, -40 };
	// 回転行列を合成
	Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_.rotation_);

	// オフセットをカメラの回転に合わせて回転
	offset = TransformNormal(offset, rotateMatrix);

	return offset;
}