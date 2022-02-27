#include "Manager_Camera.h"
#include "CCamera.h"
#include "CDirectInput.h"
#include "dx11mathutil.h"

int Manager_Camera::CameraFlg = 0;

/// <summary>
/// TPSカメラ
/// </summary>
/// <param name="_playermtx">対象の行列</param>
void Manager_Camera::TPSCamera(DirectX::XMFLOAT4X4 _playermtx) {

	XMFLOAT3 zaxis = { _playermtx._31,_playermtx._32 ,_playermtx._33 };
	XMFLOAT3 xaxis = { _playermtx._11,_playermtx._12 ,_playermtx._13 };
	XMFLOAT3 yaxis = { _playermtx._21,_playermtx._22 ,_playermtx._23 };

	XMFLOAT3 shakePos = { 0,0,0 };

	
	XMFLOAT3 eye;
	eye.x = _playermtx._41 + localPos.z * zaxis.x + localPos.y * yaxis.x + localPos.x * xaxis.x;
	eye.y = _playermtx._42 + localPos.z * zaxis.y + localPos.y * yaxis.y + localPos.x * xaxis.y;
	eye.z = _playermtx._43 + localPos.z * zaxis.z + localPos.y * yaxis.z + localPos.x * xaxis.z;

	/*if (shakeflag) {
		
		if (frame % 2 == 0) {
			shakePos.x = 0.4f;
		}
		else {
			shakePos.y = -0.4f;
		}

		eye.y = _playermtx._42 + (localPos.z + shakePos.z) * zaxis.y + (localPos.y + shakePos.y) * yaxis.y + (localPos.x + shakePos.x) * xaxis.y;
		eye.z = _playermtx._43 + (localPos.z + shakePos.z) * zaxis.z + (localPos.y + shakePos.y) * yaxis.z + (localPos.x + shakePos.x) * xaxis.z;
		eye.x = _playermtx._41 + (localPos.z + shakePos.z) * zaxis.x + (localPos.y + shakePos.y) * yaxis.x + (localPos.x + shakePos.x) * xaxis.x;

		if (frame > MAXFRAME) {
			shakeflag = false;
			frame = 0;
		}

		frame++;
	}
	else {
		
		eye.x = _playermtx._41 + localPos.z * zaxis.x + localPos.y * yaxis.x + localPos.x * xaxis.x;
		eye.y = _playermtx._42 + localPos.z * zaxis.y + localPos.y * yaxis.y + localPos.x * xaxis.y;
		eye.z = _playermtx._43 + localPos.z * zaxis.z + localPos.y * yaxis.z + localPos.x * xaxis.z;
	}*/

	XMFLOAT3 lookat;
	lookat.x = _playermtx._41 - localPos.z * zaxis.x;
	lookat.y = _playermtx._42 - localPos.z * zaxis.y;
	lookat.z = _playermtx._43 - localPos.z * zaxis.z;

	XMFLOAT3 up;
	up.x = _playermtx._21;
	up.y = _playermtx._22;
	up.z = _playermtx._23;

	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);

	CCamera::GetInstance()->CreateCameraMatrix();
}

/// <summary>
/// FPSカメラ
/// </summary>
/// <param name="_playermtx">対象の行列</param>
void Manager_Camera::FPSCamera(DirectX::XMFLOAT4X4 _playermtx) {

	XMFLOAT3 zaxis = { _playermtx._31,_playermtx._32 ,_playermtx._33 };

	XMFLOAT3 eye;
	eye.x = _playermtx._41;
	eye.y = _playermtx._42;
	eye.z = _playermtx._43;

	XMFLOAT3 lookat;
	lookat.x = _playermtx._41 + zaxis.x;
	lookat.y = _playermtx._42 + zaxis.y;
	lookat.z = _playermtx._43 + zaxis.z;

	XMFLOAT3 up;
	up.x = _playermtx._21;
	up.y = _playermtx._22;
	up.z = _playermtx._23;

	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);

	CCamera::GetInstance()->CreateCameraMatrix();
}

/// <summary>
/// 少し遅れるカメラ
/// </summary>
/// <param name="_playermtx">対象の行列</param>
void Manager_Camera::DelayCamera(DirectX::XMFLOAT4X4 _playermtx) {

	axisz[svidx].x = _playermtx._31;
	axisz[svidx].y = _playermtx._32;
	axisz[svidx].z = _playermtx._33;


	axisy[svidx].x = _playermtx._21;
	axisy[svidx].y = _playermtx._22;
	axisy[svidx].z = _playermtx._23;


	pos[svidx].x = _playermtx._41;
	pos[svidx].y = _playermtx._42;
	pos[svidx].z = _playermtx._43;
	svidx++;

	if (svidx > axisz.size() - 1) {
		svidx = 0;
		firstflag = true;
	}

	//30フレーム目の情報を使用してカメラを作成
	int useidx;
	useidx = svidx - 10;
	if (useidx < 0) {
		if (!firstflag) {
			useidx = 0;
		}
		else {
			useidx = svidx + axisz.size() - 10;
		}
	}

	XMFLOAT3 eye;
	XMFLOAT3 lookat;
	XMFLOAT3 up;

	lookat.x = _playermtx._41 + 50.0f*axisz[useidx].x;
	lookat.y = _playermtx._42 + 50.0f*axisz[useidx].y;
	lookat.z = _playermtx._43 + 50.0f*axisz[useidx].z;

	eye.x = pos[useidx].x - 50 * axisz[useidx].x;
	eye.y = pos[useidx].y - 50 * axisz[useidx].y;
	eye.z = pos[useidx].z - 50 * axisz[useidx].z;

	up = axisy[useidx];

	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);

	CCamera::GetInstance()->CreateCameraMatrix();


}

/// <summary>
/// 更新
/// </summary>
/// <param name="_playermtx">対象の行列</param>
void Manager_Camera::UpdateCamera(DirectX::XMFLOAT4X4 _playermtx) {

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_1)) {
		CameraFlg++;
		if (CameraFlg > 2) {
			CameraFlg = 0;
		}
	}

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_K)) {
		CameraFlg++;
		if (CameraFlg > 1) {
			CameraFlg = 0;
		}
	}

	switch (CameraFlg)
	{
	case 0:
		TPSCamera(_playermtx);

		break;
	case 1:

		/*_playermtx._42 += 5.0f;*/
		FPSCamera(_playermtx);
		//TPSCamera(_playermtx);
		break;
	}

}

/// <summary>
/// カメラをゆらす
/// </summary>
void Manager_Camera::ShakeCamera(XMFLOAT4X4 _playermtx) {

	XMFLOAT3 zaxis = { _playermtx._31,_playermtx._32 ,_playermtx._33 };
	XMFLOAT3 xaxis = { _playermtx._11,_playermtx._12 ,_playermtx._13 };
	XMFLOAT3 yaxis = { _playermtx._21,_playermtx._22 ,_playermtx._23 };

	XMFLOAT3 eye;

	XMFLOAT3 shakePos = { 0,0,0 };

	if (frame % 2 == 0) {
		shakePos.x = 1.0f;
	}
	else {
		shakePos.y = -1.0f;
	}

	eye.x = _playermtx._41 + shakePos.z * zaxis.x + shakePos.y * yaxis.x + shakePos.x * xaxis.x;
	eye.y = _playermtx._42 + shakePos.z * zaxis.y + shakePos.y * yaxis.y + shakePos.x * xaxis.y;
	eye.z = _playermtx._43 + shakePos.z * zaxis.z + shakePos.y * yaxis.z + shakePos.x * xaxis.z;

	frame++;
}

void Manager_Camera::UpdateSphereCamera(
	float _radius, 
	float _elevation, 
	float _azimuth, 
	const XMFLOAT3& _lookat) 
{
	XMFLOAT3 eye;

	_elevation = ToRad(_elevation);
	_azimuth = ToRad(_azimuth);

	float r = _radius * sinf(_elevation);

	eye.x = r * cosf(_azimuth);
	eye.y = _radius * cosf(_elevation);
	eye.z = r * sinf(_azimuth);

	XMFLOAT3 up;

	float elevationplus90 = _elevation + ToRad(90.0f);

	r = _radius * sinf(elevationplus90);

	up.x = r * cosf(_azimuth);
	up.y = _radius * cosf(elevationplus90);
	up.z = r * sinf(_azimuth);

	eye.x += _lookat.x;
	eye.y += _lookat.y;
	eye.z += _lookat.z;

	CCamera::GetInstance()->SetLookat(_lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);

	CCamera::GetInstance()->CreateCameraMatrix();
}