#include		<DirectXMath.h>
#include		"CCamera.h"

// Šp“x‚ðƒ‰ƒWƒAƒ“‚É•ÏX
inline float ToRad(float angle) {
	return (angle *  DirectX::XM_PI) / 180.0f;
}

//  
// ‹…–Ê‘©”›ƒJƒƒ‰(‚yŽ²‰ñ“]‚È‚µƒo[ƒWƒ‡ƒ“)
//
void UpdateSphereCamera(
	float radius,						// ”¼Œa(“ü—Í)
	float elevation,					// ‹ÂŠpi“ü—Íj
	float azimuth,						// •ûˆÊŠpi“ü—Íj
	const DirectX::XMFLOAT3& lookat)	// ’Ž‹“_i“ü—Íj
{		

	DirectX::XMFLOAT3 eye;				// Ž‹“_

	// ‹ÂŠpA•ûˆÊŠp‚ðƒ‰ƒWƒAƒ“‚É•ÏŠ·
	elevation = ToRad(elevation);
	azimuth = ToRad(azimuth);

	// ‹ÂŠp‚É‰ž‚¶‚½”¼Œa‚ðŒvŽZ
	float r = radius*sinf(elevation);

	// Ž‹“_À•W‚ðŒvŽZ
	eye.x = r*cosf(azimuth);
	eye.y = radius*cosf(elevation);
	eye.z = r*sinf(azimuth);

	// ãŒü‚«ƒxƒNƒgƒ‹‚ðŒvŽZ(‹ÂŠp‚ð‚ð‚X‚O“x‰ñ“]‚³‚¹‚é‚Æ‚Å‚«‚é)
	DirectX::XMFLOAT3 up;
	float elevationplus90 = elevation + ToRad(90.0f);			// ‹ÂŠp‚ð‚X‚O“x‘«‚·

	r = radius * sinf(elevationplus90);	

	up.x = r * cosf(azimuth);				
	up.y = radius * cosf(elevationplus90);
	up.z = r * sinf(azimuth);

	// Ž‹“_ƒxƒNƒgƒ‹‚ð’Ž‹“_•ª•½sˆÚ“®
	eye.x += lookat.x;
	eye.y += lookat.y;
	eye.z += lookat.z;

	// ƒrƒ…[•ÏŠ·s—ñ‚ðì¬‚·‚é
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetUp(up);
	CCamera::GetInstance()->CreateCameraMatrix();
}
