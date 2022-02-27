#include	<random>
#include    <vector>
#include    <list>
#include    "Game_Manager.h"
#include	"tank.h"
#include	"drawaxis.h"
#include	"dx11mathutil.h"
#include	"ModelMgr.h"
#include	"CDirectInput.h"
#include    "Bullet_Manager.h"
#include    "Manager_Camera.h"
#include    "BoundingSphere_Manager.h"
#include    "Stage_Manager.h"
#include    "StageHit.h"
#include	"Fire_Billboard.h"
#include	"IScene.h"

// ��Ԃ̐e�q�֌W�y�я����ʒu�E�����p�x���`�����f�[�^
Tank::TankInitData				Tank::ObjectInitData[] = {
	//   �e								���f���ԍ�				�����ʒu					�����p�x		// ���ӁF���Ȃ炸�q�����e����ɂȂ�Ԃ���
	{ Tank::NONE,				Tank::TANK_CATERPILLAR,			{ 0, 0, 0 },				{ 0, 0, 0 } },	// ��� �L���^�s���[
	{ Tank::TANK_CATERPILLAR,	Tank::TANK_BASE,				{ 0, 0, 0 },				{ 0, 0, 0 } },	// ��� �x�[�X
	{ Tank::TANK_BASE,			Tank::TANK_TOP,					{ 0, 3.82394f, -1.15362f }, { 0, 0, 0 } },	// ��� �C��
	{ Tank::TANK_TOP,			Tank::TANK_BARREL,				{ 0, 0.684f, -3.53283f },	{ 0, 0, 0 } },	// ��� �C�g
	{ Tank::END,				Tank::END,						{ 0, 0, 0 },				{ 0, 0, 0 } },	// �I�[�R�[�h
};

// ���f���t�@�C�������X�g
Tank::TankModelData				Tank::XFileData[] = {
	{ Tank::TANK_CATERPILLAR,	"assets/tank/tank10_cat" },				// ��Ԃ̃L���^�s���[
	{ Tank::TANK_BASE,			"assets/tank/tank10_base" },			// ��Ԃ̓y��
	{ Tank::TANK_TOP,			"assets/tank/tank10_top" },				// ��Ԃ̖C��
	{ Tank::TANK_BARREL,		"assets/tank/tank10_pipe" },			// ��Ԃ̖C�g
	{ Tank::END,				nullptr }								// �I�[�R�[�h
};

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="_game">�Q�[��</param>
Tank::Tank()
	: speed(0.0f)
{

}

Tank::Tank(TYPE _type)
	: speed(0.0f)
	, type(_type)
{

}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <remarks>�^�C�g�����̂ݎg�p</remarks>
Tank::Tank(IScene* _scene)
	: speed(0.0f)
	, type(TYPE::PLAYER)
{
	tag = "tank";
	render = RENDER::DEFAULTSHADOW;
	isDraw = true;
	scene = _scene;
	scene->AddSceneObject(this);
}


/// <summary>
/// ������
/// </summary>
/// <returns></returns>
bool Tank::Init() {

	std::string x = { ".x" };
	// �s�񏉊���
	DX11MtxIdentity(mtx);					// ���f����ǂݍ���
	// ���f���|�C���^�[���Z�b�g
	if (type == TYPE::PLAYER) {
		for (int i = 0; i < static_cast<int>(Tank::PARTSMAX) - 2; i++) {
			SetModel(ModelMgr::GetInstance().GetModelPtr(XFileData[i].XfileName + x), i);
		}
	}
	else {
		std::string enemy = { "_enemy" };
		for (int i = 0; i < static_cast<int>(Tank::PARTSMAX) - 2; i++) {
			SetModel(ModelMgr::GetInstance().GetModelPtr(XFileData[i].XfileName + enemy + x), i);
		}
	}

	// �v���C���s�񏉊���(�����ʒu)�i�����̂��Ƃ������l�����s����쐬�j
	for (int i = 0; i < static_cast<int>(TANKPARTS::PARTSMAX) - 2; i++) {
		DX11MakeWorldMatrix(mtxlocalpose[i], ObjectInitData[i].FirstAngle, ObjectInitData[i].FirstPosition);
	}

	// �e�q�֌W���l�������s����v�Z����
	CaliculateParentChildMtx();					// �e�̍s����|���Z���ăO���[�o���|�[�Y�����

	obb = new COBB();
	//OBB������
	obb->Init(models[TANKPARTS::TANK_BASE]);

	radius = BoundingSphere_Manager::GetInstance().GetBSRadius(XFileData[TANKPARTS::TANK_BASE].XfileName + x);

	////�������o���Ƃ�
	//GenerateSmoke();

	return true;
}

///<summary>
///�`��
///</summary>
void Tank::Draw() {

	// ���f���`��
//	drawaxis(m_mtx, 200, m_pos);

	if (isDraw) {
		for (int i = 0; i < static_cast<int>(TANKPARTS::PARTSMAX) - 2; i++) {

			models[i]->Draw(mtxParentChild[i]);
		}
	}

	//obb->Draw();


	for (auto& fire : m_fireList)
		fire->Draw();


	/*for (auto& smoke : m_smokeList)
		smoke->Draw();*/
}

///<summary>
///�`��
///</summary>
void Tank::DrawShadow() {

	// ���f���`��
//	drawaxis(m_mtx, 200, m_pos);

	if (isDraw) {
		for (int i = 0; i < static_cast<int>(TANKPARTS::PARTSMAX) - 2; i++) {
			GetLight().SetMtxWorld(mtxParentChild[i]);
			models[i]->DrawShadow(mtxParentChild[i]);
		}
	}

	//obb->Draw();

}

///<summary>
///�`��
///</summary>
void Tank::DrawModel() {

	// ���f���`��
//	drawaxis(m_mtx, 200, m_pos);

	if (isDraw) {
		for (int i = 0; i < static_cast<int>(TANKPARTS::PARTSMAX) - 2; i++) {
			GetLight().SetMtxWorld(mtxParentChild[i]);
			models[i]->DrawModel(mtxParentChild[i]);
		}
	}

	/*for (auto& fire : m_fireList)
		fire->Draw();*/

	for (auto& smoke : m_smokeList)
		smoke->Draw();

	//obb->Draw();

}

/// <summary>
/// ���o����
/// </summary>
void Tank::PrepareDestroy() {
	//��������
	std::uniform_real_distribution<float> rotx(2, 5);
	std::uniform_real_distribution<float> roty(2, 5);
	std::uniform_real_distribution<float> rotz(2, 5);
	std::uniform_real_distribution<float> speed(1, 2);

	//�l�Z�b�g
	destroyRotx = rotx(mt);
	destroyRoty = roty(mt);
	destroyRotz = roty(mt);
	destroySpeed = speed(mt);
}

/// <summary>
/// �������̉��o
/// </summary>
bool Tank::DestroyEffect() {

	if (destroyCnt > 60) {
		return true;
	}

	//���f�����΂�
	for (int i = 0; i < static_cast<int>(TANKPARTS::PARTSMAX) - 2; i++) {

		XMFLOAT4 qt;	// �N�I�[�^�j�I��

		XMFLOAT4 axisX;
		XMFLOAT4 axisY;
		XMFLOAT4 axisZ;
		XMFLOAT3 oldPos;

		GetAxisXfromMatrix(mtxParentChild[i], axisX);
		GetAxisYfromMatrix(mtxParentChild[i], axisY);
		GetAxisZfromMatrix(mtxParentChild[i], axisZ);

		oldPos.x = mtxParentChild[i]._41;
		oldPos.y = mtxParentChild[i]._42;
		oldPos.z = mtxParentChild[i]._43;

		// �s�񂩂�N�I�[�^�j�I���𐶐�
		DX11GetQtfromMatrix(mtxParentChild[i], qt);

		XMFLOAT4 qtx, qty, qtz;		// �N�I�[�^�j�I��

		// �w�莲��]�̃N�I�[�^�j�I���𐶐�
		DX11QtRotationAxis(qtx, axisX, destroyRotx);
		DX11QtRotationAxis(qty, axisY, destroyRoty);
		DX11QtRotationAxis(qtz, axisZ, destroyRotz);

		// �N�I�[�^�j�I��������
		XMFLOAT4 tempqt1;
		DX11QtMul(tempqt1, qt, qtx);

		XMFLOAT4 tempqt2;
		DX11QtMul(tempqt2, qty, qtz);

		XMFLOAT4 tempqt3;
		DX11QtMul(tempqt3, tempqt1, tempqt2);

		// �N�I�[�^�j�I�����m�[�}���C�Y
		DX11QtNormalize(tempqt3, tempqt3);

		// �N�I�[�^�j�I������s����쐬
		DX11MtxFromQt(mtxParentChild[i], tempqt3);

		//�e�����ɔ�΂�
		switch (i)
		{
		case 0:
			mtxParentChild[i]._41 = oldPos.x + destroySpeed;
			mtxParentChild[i]._43 = oldPos.z + destroySpeed;
			break;
		case 1:
			mtxParentChild[i]._41 = oldPos.x - destroySpeed;
			mtxParentChild[i]._43 = oldPos.z + destroySpeed;
			break;
		case 2:
			mtxParentChild[i]._41 = oldPos.x + destroySpeed;
			mtxParentChild[i]._43 = oldPos.z - destroySpeed;
			break;
		case 3:
			mtxParentChild[i]._41 = oldPos.x - destroySpeed;
			mtxParentChild[i]._43 = oldPos.z - destroySpeed;
			break;
		}


		mtxParentChild[i]._42 = oldPos.y + destroySpeed;

	}

	destroyCnt++;

	return false;
}

void Tank::UpdateSelect() {
	XMFLOAT4 axisX;
	XMFLOAT4 axisY;
	XMFLOAT4 axisZ;

	//�@�{�̂̈ړ�����
	//�������o��
	GetAxisXfromMatrix(mtx, axisX);
	GetAxisYfromMatrix(mtx, axisY);
	GetAxisZfromMatrix(mtx, axisZ);

	XMFLOAT4 qt;	// �N�I�[�^�j�I��

	// �s�񂩂�N�I�[�^�j�I���𐶐�
	DX11GetQtfromMatrix(mtx, qt);

	XMFLOAT4 qtx, qty, qtz;		// �N�I�[�^�j�I��

	angle.y = ROTATIONRATE;

	// �w�莲��]�̃N�I�[�^�j�I���𐶐�
	DX11QtRotationAxis(qtx, axisX, angle.x);
	DX11QtRotationAxis(qty, axisY, angle.y);
	DX11QtRotationAxis(qtz, axisZ, angle.z);

	// �N�I�[�^�j�I��������
	XMFLOAT4 tempqt1;
	DX11QtMul(tempqt1, qt, qtx);

	XMFLOAT4 tempqt2;
	DX11QtMul(tempqt2, qty, qtz);

	XMFLOAT4 tempqt3;
	DX11QtMul(tempqt3, tempqt1, tempqt2);

	// �N�I�[�^�j�I�����m�[�}���C�Y
	DX11QtNormalize(tempqt3, tempqt3);

	// �N�I�[�^�j�I������s����쐬
	DX11MtxFromQt(mtx, tempqt3);

	UpdateEneLocalpose();

	// �e�q�֌W���l�������s����v�Z����
	CaliculateParentChildMtx();
}

/// <summary>
/// ���[�J���̈ʒu�X�V
/// </summary>
void Tank::UpdateLocalpose() {

	// �e�p�[�c�̉�]�p�x(�O�t���[������̕ψʗ�)
	XMFLOAT3			partsangle[static_cast<int>(TANKPARTS::PARTSMAX)];
	XMFLOAT3			partstrans[static_cast<int>(TANKPARTS::PARTSMAX)];

	// �ǂ̃p�[�c�𓮂�����
	int idx = 0;

	// �p�x�ƈړ��ʂ�������
	for (int i = 0; i < static_cast<int>(TANKPARTS::PARTSMAX); i++) {
		partsangle[i].x = 0.0f;
		partsangle[i].y = 0.0f;
		partsangle[i].z = 0.0f;
		partstrans[i].x = 0.0f;
		partstrans[i].y = 0.0f;
		partstrans[i].z = 0.0f;
	}

	// �p�[�c�̃��[�J���|�[�Y�����߂�ׂ̃L�[����
	// �C�g�𓮂���
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP)) {
		idx = TANK_BARREL;
		partsangle[idx].x += 1.0f;
	}

	// �C�g�𓮂���
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN)) {
		idx = TANK_BARREL;
		partsangle[idx].x -= 1.0f;
	}

	// �C��𓮂���
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT)) {
		idx = TANK_TOP;
		partsangle[idx].y += 1.0f;
	}

	// �C��𓮂���
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT)) {
		idx = TANK_TOP;
		partsangle[idx].y -= 1.0f;
	}

	//�e����
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE)) {


		XMFLOAT4X4 tempmtx = mtxParentChild[TANKPARTS::TANK_BARREL];

		//��Ԃ̎p�����t�̂��ߔ��΂ɂ���
		tempmtx._31 = -tempmtx._31;
		tempmtx._32 = -tempmtx._32;
		tempmtx._33 = -tempmtx._33;
		tempmtx._34 = -tempmtx._34;

		//����
	/*	Bullet_Manager::GetInstance().Fire(tempmtx, Bullet_Object::TYPEOBJ::PLAYER);
		Manager_Camera::GetInstance().shakeflag = true;*/
	}

	mtxlocalpose[Tank::TANK_CATERPILLAR] = mtx;

	// �p�[�c�̊p�x���[�J���|�[�Y��\���s����v�Z
	XMFLOAT4X4 partsmtx;
	DX11MakeWorldMatrix(partsmtx, partsangle[idx], partstrans[idx]);
	DX11MtxMultiply(mtxlocalpose[idx], partsmtx, mtxlocalpose[idx]);

	mtxlocalpose[Tank::TANK_CATERPILLAR]._41 += mtxlocalpose[Tank::TANK_CATERPILLAR]._31 * speed;
	mtxlocalpose[Tank::TANK_CATERPILLAR]._42 += mtxlocalpose[Tank::TANK_CATERPILLAR]._32 * speed;
	mtxlocalpose[Tank::TANK_CATERPILLAR]._43 += mtxlocalpose[Tank::TANK_CATERPILLAR]._33 * speed;

	//��ԉ��̊K�w�̍s��ƈʒu���Z�b�g
	pos.x = mtxlocalpose[Tank::TANK_CATERPILLAR]._41;
	pos.y = mtxlocalpose[Tank::TANK_CATERPILLAR]._42;
	pos.z = mtxlocalpose[Tank::TANK_CATERPILLAR]._43;
	mtx = mtxlocalpose[Tank::TANK_CATERPILLAR];

	//OBB�X�V
	//obb->Update(mtx);
}

void Tank::UpdateEnemyLocalpose(XMFLOAT4X4 _playermtx) {

	// �e�p�[�c�̉�]�p�x(�O�t���[������̕ψʗ�)
	XMFLOAT3			partsangle[static_cast<int>(TANKPARTS::PARTSMAX)];
	XMFLOAT3			partstrans[static_cast<int>(TANKPARTS::PARTSMAX)];

	XMFLOAT3 Axis;

	//�v���C���[�̏������o��
	XMFLOAT3 playerpos = { _playermtx._41,_playermtx._42,_playermtx._43 };

	//�ڕW�Ƃ̃x�N�g��
	XMFLOAT3 targetvec3 = { playerpos.x - pos.x, playerpos.y - pos.y , playerpos.z - pos.z };
	XMFLOAT2 targetvec2 = { playerpos.x - pos.x, playerpos.z - pos.z };

	//���@�x�N�g��
	/*XMFLOAT3 myvec3 = { mtxParentChild[TANK_TOP]._31 * 2.0f, mtxParentChild[TANK_TOP]._32 * 2.0f, mtxParentChild[TANK_TOP]._33 * 2.0f };
	XMFLOAT2 myvec2 = { mtxParentChild[TANK_TOP]._31 * 2.0f ,mtxParentChild[TANK_TOP]._33 * 2.0f };*/

	XMFLOAT3 myvec3 = { mtxlocalpose[Tank::TANK_CATERPILLAR]._31 * 2.0f, mtxlocalpose[Tank::TANK_CATERPILLAR]._32 * 2.0f, mtxlocalpose[Tank::TANK_CATERPILLAR]._33 * 2.0f };
	XMFLOAT2 myvec2 = { mtxlocalpose[Tank::TANK_CATERPILLAR]._31 * 2.0f ,mtxlocalpose[Tank::TANK_CATERPILLAR]._33 * 2.0f };

	//���K��
	DX11Vec2Normalize(targetvec2, targetvec2);
	DX11Vec2Normalize(myvec2, myvec2);

	//�O�ςŎ������߂�
	DX11Vec3Cross(Axis, targetvec3, myvec3);

	//���ς���cos�Ƃ��v�Z
	float rot = targetvec2.x * myvec2.x + targetvec2.y * myvec2.y;

	//���������Ȃ�̂�␳
	if (fabsf(rot) > 0.99f) {
		rot = 1.0f;
	}

	//���W�A���v�Z
	float rotdiff = acosf(rot);


	//���ɂ���Ċp�x���}�C�i�X�ɂ���
	if (Axis.y < 0.0f) {
		rotdiff = -rotdiff;
	}

	// �ǂ̃p�[�c�𓮂�����
	int idx = 0;

	// �p�x�ƈړ��ʂ�������
	for (int i = 0; i < static_cast<int>(TANKPARTS::PARTSMAX); i++) {
		partsangle[i].x = 0.0f;
		partsangle[i].y = 0.0f;
		partsangle[i].z = 0.0f;
		partstrans[i].x = 0.0f;
		partstrans[i].y = 0.0f;
		partstrans[i].z = 0.0f;
	}

	idx = TANK_CATERPILLAR;
	partsangle[idx].y += rotdiff;

	mtxlocalpose[Tank::TANK_CATERPILLAR] = mtx;

	// �p�[�c�̊p�x���[�J���|�[�Y��\���s����v�Z
	XMFLOAT4X4 partsmtx;
	DX11MakeWorldMatrix(partsmtx, partsangle[idx], partstrans[idx]);
	DX11MtxMultiply(mtxlocalpose[idx], partsmtx, mtxlocalpose[idx]);

	mtxlocalpose[Tank::TANK_CATERPILLAR]._41 += mtxlocalpose[Tank::TANK_CATERPILLAR]._31 * speed;
	mtxlocalpose[Tank::TANK_CATERPILLAR]._42 += mtxlocalpose[Tank::TANK_CATERPILLAR]._32 * speed;
	mtxlocalpose[Tank::TANK_CATERPILLAR]._43 += mtxlocalpose[Tank::TANK_CATERPILLAR]._33 * speed;

	//��ԉ��̊K�w�̍s��ƈʒu���Z�b�g
	pos.x = mtxlocalpose[Tank::TANK_CATERPILLAR]._41;
	pos.y = mtxlocalpose[Tank::TANK_CATERPILLAR]._42;
	pos.z = mtxlocalpose[Tank::TANK_CATERPILLAR]._43;
	mtx = mtxlocalpose[Tank::TANK_CATERPILLAR];

	//OBB�X�V
	//obb->Update(mtx);

}

void Tank::UpdateEneLocalpose() {
	// �e�p�[�c�̉�]�p�x(�O�t���[������̕ψʗ�)
	XMFLOAT3			partsangle[static_cast<int>(TANKPARTS::PARTSMAX)];
	XMFLOAT3			partstrans[static_cast<int>(TANKPARTS::PARTSMAX)];

	// �ǂ̃p�[�c�𓮂�����
	int idx = 0;

	// �p�x�ƈړ��ʂ�������
	for (int i = 0; i < static_cast<int>(TANKPARTS::PARTSMAX); i++) {
		partsangle[i].x = 0.0f;
		partsangle[i].y = 0.0f;
		partsangle[i].z = 0.0f;
		partstrans[i].x = 0.0f;
		partstrans[i].y = 0.0f;
		partstrans[i].z = 0.0f;
	}


	mtxlocalpose[Tank::TANK_CATERPILLAR] = mtx;

	// �p�[�c�̊p�x���[�J���|�[�Y��\���s����v�Z
	XMFLOAT4X4 partsmtx;
	DX11MakeWorldMatrix(partsmtx, partsangle[idx], partstrans[idx]);
	DX11MtxMultiply(mtxlocalpose[idx], partsmtx, mtxlocalpose[idx]);

	mtxlocalpose[Tank::TANK_CATERPILLAR]._41 += mtxlocalpose[Tank::TANK_CATERPILLAR]._31 * speed;
	mtxlocalpose[Tank::TANK_CATERPILLAR]._42 += mtxlocalpose[Tank::TANK_CATERPILLAR]._32 * speed;
	mtxlocalpose[Tank::TANK_CATERPILLAR]._43 += mtxlocalpose[Tank::TANK_CATERPILLAR]._33 * speed;

	//��ԉ��̊K�w�̍s��ƈʒu���Z�b�g
	pos.x = mtxlocalpose[Tank::TANK_CATERPILLAR]._41;
	pos.y = mtxlocalpose[Tank::TANK_CATERPILLAR]._42;
	pos.z = mtxlocalpose[Tank::TANK_CATERPILLAR]._43;
	mtx = mtxlocalpose[Tank::TANK_CATERPILLAR];

}

/// <summary>
/// �e����
/// </summary>
void Tank::Fire() {

	std::unique_ptr<Fire_Billboard> fire;

	XMFLOAT4X4 temp = mtxParentChild[Tank::TANK_BARREL];

	temp._31 = -temp._31;
	temp._32 = -temp._32;
	temp._33 = -temp._33;
	temp._34 = -temp._34;

	XMFLOAT3 pipepos = { temp._41 + temp._31 * PIPELENGTH, temp._42 + temp._32 * PIPELENGTH, temp._43 + temp._33 * PIPELENGTH };

	fire = std::make_unique<Fire_Billboard>(pipepos);
	fire->Init();

	m_fireList.emplace_back(std::move(fire));

	m_fireFlg = true;
	m_fireCnt = 0;

}


/// <summary>
/// �X�V
/// </summary>
void Tank::Update() {

	// �ړ��ʋy�ъp�x���O�ɂ���
	speed = 0.0f;
	angle.x = angle.y = angle.z = 0.0f;

	//���폜
	auto itr = m_smokeList.begin();
	while (itr != m_smokeList.end()) {

		if ((*itr)->isDraw == false) {
			itr = m_smokeList.erase(itr);

		}
		else ++itr;
	}

	//�����o��
	for (auto& smoke : m_smokeList) {
		smoke->Update();
	}

	static uint16_t l_smokecnt = 0;

	/*if (l_smokecnt > 20) {
		GenerateSmoke();
		l_smokecnt = 0;
	}
	l_smokecnt++;*/

	XMFLOAT4 axisX;
	XMFLOAT4 axisY;
	XMFLOAT4 axisZ;
	//�������o��
	GetAxisXfromMatrix(mtx, axisX);
	GetAxisYfromMatrix(mtx, axisY);
	GetAxisZfromMatrix(mtx, axisZ);

	//�����n����
	if (m_fireCnt > m_fireInterval) {
		m_fireFlg = false;
	}

	if (m_fireFlg) {

		float t;

		t = static_cast<float>(m_fireCnt + 1) / static_cast<float>(m_fireInterval / 2);

		XMFLOAT3 pos;

		pos = Lerp(lerpstart, lerpend, t);

		m_fireCnt++;
		//move += ACCEL;
		keyInput = true;
	}
	else {
		m_fireMtx = mtxParentChild[Tank::TANK_BARREL];
	}


	auto it = m_fireList.begin();
	while (it != m_fireList.end()) {

		if ((*it)->isDraw == false) {
			it = m_fireList.erase(it);

		}
		else ++it;
	}


	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A)) {
		angle.y -= ROTATIONRATE;
		keyInput = true;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D)) {
		angle.y += ROTATIONRATE;
		keyInput = true;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W)) {
		//speed = -0.3f;
		move -= ACCEL;
		keyInput = true;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S)) {
		//speed = 0.3f;
		move += ACCEL;
		keyInput = true;
	}



	static float lengthx = obb->GetOBB().fLengthX / 3;
	static float lengthz = obb->GetOBB().fLengthZ / 3;

	//�\�[�g�p���X�g
	std::vector<XMFLOAT3> temp;

	//�S�̎x�_�̈ʒu���v�Z
	for (int i = 0; i < 4; i++) {

		switch (i) {
		case RIGHTUP:
			ikpos[i].x = pos.x + axisZ.x * lengthx;
			ikpos[i].z = pos.z + axisZ.z * lengthz;
			break;
		case LEFTUP:
			ikpos[i].x = pos.x + axisZ.x * lengthx;
			ikpos[i].z = pos.z + axisZ.z * -lengthz;
			break;
		case RIGHTDOWN:
			ikpos[i].x = pos.x + axisZ.x * -lengthx;
			ikpos[i].z = pos.z + axisZ.z * lengthz;
			break;
		case LEFTDOWN:
			ikpos[i].x = pos.x + axisZ.x * -lengthx;
			ikpos[i].z = pos.z + axisZ.z * -lengthz;
			break;
		}

		bool sts = SearchAllSurface(ikpos[i].x, ikpos[i].z, ikpos[i]);
		if (sts) {
			temp.emplace_back(ikpos[i]);
		}
	}

	//�\�[�g���ē���ւ���
	for (int i = 0; i < temp.size(); i++) {
		for (int j = i + 1; j < temp.size(); j++) {
			if (temp.at(i).y < temp.at(j).y) {
				XMFLOAT3 tmp = temp.at(i);
				temp.at(i) = temp.at(j);
				temp.at(j) = tmp;
			}
		}
	}

	//�R�_����@������
	XMFLOAT3 normal = CreateNormal(temp.at(0), temp.at(1), temp.at(2));

	//������̃x�N�g��
	XMFLOAT3 up = { axisY.x,axisY.y,axisY.z };

	XMFLOAT3 dir;
	DX11Vec3Cross(dir, up, normal);
	DX11Vec3Normalize(dir, dir);

	float dot;

	DX11Vec3Dot(dot, up, dir);

	float rad = acosf(dot);

	XMFLOAT4 axisup = { up.x, up.y, up.z, 0.0f };

	XMFLOAT4 qtnor = { 0,0,0,1 };

	DX11QtRotationAxis(qtnor, axisup, rad);

	//angle.x = 45.0f;
	//float a = 0;
	// �L�[���͂��������ꍇ
	if (keyInput) {
		XMFLOAT4 qt;	// �N�I�[�^�j�I��

		// �s�񂩂�N�I�[�^�j�I���𐶐�
		DX11GetQtfromMatrix(mtx, qt);

		XMFLOAT4 qtx, qty, qtz;		// �N�I�[�^�j�I��

		// �w�莲��]�̃N�I�[�^�j�I���𐶐�
		DX11QtRotationAxis(qtx, axisX, angle.x);
		DX11QtRotationAxis(qty, axisY, angle.y);
		DX11QtRotationAxis(qtz, axisZ, angle.z);

		// �N�I�[�^�j�I��������
		XMFLOAT4 tempqt1;
		DX11QtMul(tempqt1, qt, qtx);

		XMFLOAT4 tempqt2;
		DX11QtMul(tempqt2, qty, qtz);

		XMFLOAT4 tempqt3;
		DX11QtMul(tempqt3, tempqt1, tempqt2);

		XMFLOAT4 tempqt4;
		DX11QtMul(tempqt4, tempqt3, qtnor);

		// �N�I�[�^�j�I�����m�[�}���C�Y
		DX11QtNormalize(tempqt3, tempqt3);

		// �N�I�[�^�j�I������s����쐬
		DX11MtxFromQt(mtx, tempqt3);

		/*a = 1 - tempqt3.x*tempqt3.x * 2 - tempqt3.z*tempqt3.z * 2;*/
	}
	else {
		//�L�[��������ĂȂ��Ƃ��̓X�s�[�h�������Ă���
		if (move > 0) {
			move -= ACCEL;
		}
		else if (move < 0) {
			move += ACCEL;
		}

		if (fabsf(move) < 0.02f) {
			move = 0.0f;
		}
	}

	//�ō����x�ɒB������
	if (move > MAXSPEED) {
		move = MAXSPEED;
	}
	else if (move < -MAXSPEED) {
		move = -MAXSPEED;
	}

	speed = move;

	//�X�s�[�h�ɂ���ăJ�����̈ʒu��ς���
	/*float crate = move / MAXSPEED;
	crate = fabsf(crate);
	XMFLOAT3 camerapos = { Manager_Camera::DEFPOSX, Manager_Camera::DEFPOSY, Manager_Camera::DEFPOSZ };
	camerapos.z = 10 * crate + Manager_Camera::DEFPOSZ;

	Manager_Camera::GetInstance().SetLocalPos(camerapos);*/

	// Z�������o��
	axisZ.x = mtx._31;
	axisZ.y = mtx._32;
	axisZ.z = mtx._33;
	axisZ.w = 0.0f;

	// ���S�ʒu���X�V
	pos.x += axisZ.x*speed;
	pos.y += axisZ.y*speed;
	pos.z += axisZ.z*speed;

	XMFLOAT3 ans;

	bool sts = SearchAllSurface(pos.x, pos.z, ans);
	if (sts) {
		// ��_�����������ꍇ
		mtx._41 = pos.x;
		mtx._42 = ans.y;
		//mtx._42 = pos.y;
		mtx._43 = pos.z;
	}
	else {
		// ��_��������Ȃ������ꍇ
		mtx._41 = pos.x;
		mtx._42 = pos.y;					// �����O
		mtx._43 = pos.z;
	}

	// ���[�J���|�[�Y���X�V����
	UpdateLocalpose();

	// �e�q�֌W���l�������s����v�Z����
	CaliculateParentChildMtx();



	keyInput = false;
}

/// <summary>
/// �ǐՍX�V
/// </summary>
/// <param name="_playermtx">���@�p��</param>
/// <remarks>�G�̂�</remarks>
void Tank::UpdateChase(XMFLOAT4X4 _playermtx) {

	XMFLOAT4 axisX;
	XMFLOAT4 axisY;
	XMFLOAT4 axisZ;

	/*static bool keyinput = true;*/

	//�@�{�̂̈ړ�����
	//�������o��
	GetAxisXfromMatrix(mtx, axisX);
	GetAxisYfromMatrix(mtx, axisY);
	GetAxisZfromMatrix(mtx, axisZ);

	speed = -0.5f;

	XMFLOAT4 qt;	// �N�I�[�^�j�I��

	// �s�񂩂�N�I�[�^�j�I���𐶐�
	DX11GetQtfromMatrix(mtx, qt);

	XMFLOAT4 qtx, qty, qtz;		// �N�I�[�^�j�I��

	// �w�莲��]�̃N�I�[�^�j�I���𐶐�
	DX11QtRotationAxis(qtx, axisX, angle.x);
	DX11QtRotationAxis(qty, axisY, angle.y);
	DX11QtRotationAxis(qtz, axisZ, angle.z);

	// �N�I�[�^�j�I��������
	XMFLOAT4 tempqt1;
	DX11QtMul(tempqt1, qt, qtx);

	XMFLOAT4 tempqt2;
	DX11QtMul(tempqt2, qty, qtz);

	XMFLOAT4 tempqt3;
	DX11QtMul(tempqt3, tempqt1, tempqt2);

	// �N�I�[�^�j�I�����m�[�}���C�Y
	DX11QtNormalize(tempqt3, tempqt3);

	// �N�I�[�^�j�I������s����쐬
	DX11MtxFromQt(mtx, tempqt3);


	// Z�������o��
	axisZ.x = mtx._31;
	axisZ.y = mtx._32;
	axisZ.z = mtx._33;
	axisZ.w = 0.0f;

	//�ō����x�ɒB������
	if (move > MAXSPEED) {
		move = MAXSPEED;
	}
	else if (move < -MAXSPEED) {
		move = -MAXSPEED;
	}
	move -= ACCEL;
	speed = move;

	// �ʒu���X�V
	pos.x += axisZ.x*speed;
	pos.y += axisZ.y*speed;
	pos.z += axisZ.z*speed;

	XMFLOAT3 ans;

	bool sts = SearchAllSurface(pos.x, pos.z, ans);
	if (sts) {
		// ��_�����������ꍇ
		mtx._41 = pos.x;
		mtx._42 = ans.y;
		mtx._43 = pos.z;
	}
	else {
		// ��_��������Ȃ������ꍇ
		mtx._41 = pos.x;
		mtx._42 = pos.y;					// �����O
		mtx._43 = pos.z;
	}

	// ���[�J���|�[�Y���X�V����
	UpdateEnemyLocalpose(_playermtx);

	// �e�q�֌W���l�������s����v�Z����
	CaliculateParentChildMtx();

	keyInput = false;
}

/// <summary>
/// ���G�X�V
/// </summary>
/// <remarks>�G�̂�</remarks>
void Tank::UpdateFree() {
	XMFLOAT4 axisX;
	XMFLOAT4 axisY;
	XMFLOAT4 axisZ;

	//�@�{�̂̈ړ�����
	//�������o��
	GetAxisXfromMatrix(mtx, axisX);
	GetAxisYfromMatrix(mtx, axisY);
	GetAxisZfromMatrix(mtx, axisZ);

	XMFLOAT4 qt;	// �N�I�[�^�j�I��

	// �s�񂩂�N�I�[�^�j�I���𐶐�
	DX11GetQtfromMatrix(mtx, qt);

	XMFLOAT4 qtx, qty, qtz;		// �N�I�[�^�j�I��

	// �w�莲��]�̃N�I�[�^�j�I���𐶐�
	DX11QtRotationAxis(qtx, axisX, angle.x);
	DX11QtRotationAxis(qty, axisY, angle.y);
	DX11QtRotationAxis(qtz, axisZ, angle.z);

	// �N�I�[�^�j�I��������
	XMFLOAT4 tempqt1;
	DX11QtMul(tempqt1, qt, qtx);

	XMFLOAT4 tempqt2;
	DX11QtMul(tempqt2, qty, qtz);

	XMFLOAT4 tempqt3;
	DX11QtMul(tempqt3, tempqt1, tempqt2);

	// �N�I�[�^�j�I�����m�[�}���C�Y
	DX11QtNormalize(tempqt3, tempqt3);

	// �N�I�[�^�j�I������s����쐬
	DX11MtxFromQt(mtx, tempqt3);


	// Z�������o��
	axisZ.x = mtx._31;
	axisZ.y = mtx._32;
	axisZ.z = mtx._33;
	axisZ.w = 0.0f;

	pos.x += axisZ.x*speed;
	pos.y += axisZ.y*speed;
	pos.z += axisZ.z*speed;

	XMFLOAT3 ans;

	bool sts = SearchAllSurface(pos.x, pos.z, ans);
	if (sts) {
		// ��_�����������ꍇ
		mtx._41 = pos.x;
		mtx._42 = ans.y;
		mtx._43 = pos.z;
	}
	else {
		// ��_��������Ȃ������ꍇ
		mtx._41 = pos.x;
		mtx._42 = pos.y;					// �����O
		mtx._43 = pos.z;
	}


	UpdateEneLocalpose();

	// �e�q�֌W���l�������s����v�Z����
	CaliculateParentChildMtx();

}

/// <summary>
///�@����X�V
/// </summary>
void Tank::UpdateCycle() {
	// �e�p�[�c�̉�]�p�x(�O�t���[������̕ψʗ�)
	XMFLOAT3			partsangle[static_cast<int>(TANKPARTS::PARTSMAX)];
	XMFLOAT3			partstrans[static_cast<int>(TANKPARTS::PARTSMAX)];

	XMFLOAT3 Axis;

	//�ڕW�Ƃ̃x�N�g��
	//�t����
	XMFLOAT3 targetvec3 = { mtxlocalpose[Tank::TANK_CATERPILLAR]._31 * -2.0f, mtxlocalpose[Tank::TANK_CATERPILLAR]._32 * -2.0f, mtxlocalpose[Tank::TANK_CATERPILLAR]._33 * -2.0f };
	XMFLOAT2 targetvec2 = { mtxlocalpose[Tank::TANK_CATERPILLAR]._31 * -2.0f ,mtxlocalpose[Tank::TANK_CATERPILLAR]._33 * -2.0f };


	XMFLOAT3 myvec3 = { mtxlocalpose[Tank::TANK_CATERPILLAR]._31 * 2.0f, mtxlocalpose[Tank::TANK_CATERPILLAR]._32 * 2.0f, mtxlocalpose[Tank::TANK_CATERPILLAR]._33 * 2.0f };
	XMFLOAT2 myvec2 = { mtxlocalpose[Tank::TANK_CATERPILLAR]._31 * 2.0f ,mtxlocalpose[Tank::TANK_CATERPILLAR]._33 * 2.0f };

	//���K��
	DX11Vec2Normalize(targetvec2, targetvec2);
	DX11Vec2Normalize(myvec2, myvec2);

	//�O�ςŎ������߂�
	DX11Vec3Cross(Axis, targetvec3, myvec3);

	//���ς���cos�Ƃ��v�Z
	float rot = targetvec2.x * myvec2.x + targetvec2.y * myvec2.y;

	//���������Ȃ�̂�␳
	if (fabsf(rot) > 0.99f) {
		rot = -1.0f;
	}

	//���W�A���v�Z
	float rotdiff = acosf(rot);

	//���ɂ���Ċp�x���}�C�i�X�ɂ���
	if (Axis.y < 0.0f) {
		rotdiff = -rotdiff;
	}

	// �ǂ̃p�[�c�𓮂�����
	int idx = 0;

	// �p�x�ƈړ��ʂ�������
	for (int i = 0; i < static_cast<int>(TANKPARTS::PARTSMAX); i++) {
		partsangle[i].x = 0.0f;
		partsangle[i].y = 0.0f;
		partsangle[i].z = 0.0f;
		partstrans[i].x = 0.0f;
		partstrans[i].y = 0.0f;
		partstrans[i].z = 0.0f;
	}

	idx = TANK_CATERPILLAR;
	partsangle[idx].y += rotdiff;

	mtxlocalpose[Tank::TANK_CATERPILLAR] = mtx;

	// �p�[�c�̊p�x���[�J���|�[�Y��\���s����v�Z
	XMFLOAT4X4 partsmtx;
	DX11MakeWorldMatrix(partsmtx, partsangle[idx], partstrans[idx]);
	DX11MtxMultiply(mtxlocalpose[idx], partsmtx, mtxlocalpose[idx]);

	mtxlocalpose[Tank::TANK_CATERPILLAR]._41 += mtxlocalpose[Tank::TANK_CATERPILLAR]._31 * speed;
	mtxlocalpose[Tank::TANK_CATERPILLAR]._42 += mtxlocalpose[Tank::TANK_CATERPILLAR]._32 * speed;
	mtxlocalpose[Tank::TANK_CATERPILLAR]._43 += mtxlocalpose[Tank::TANK_CATERPILLAR]._33 * speed;

	XMFLOAT3 ans;

	//��ԉ��̊K�w�̍s��ƈʒu���Z�b�g
	pos.x = mtxlocalpose[Tank::TANK_CATERPILLAR]._41;
	pos.y = mtxlocalpose[Tank::TANK_CATERPILLAR]._42;
	pos.z = mtxlocalpose[Tank::TANK_CATERPILLAR]._43;
	mtx = mtxlocalpose[Tank::TANK_CATERPILLAR];

	bool sts = SearchAllSurface(pos.x, pos.z, ans);
	if (sts) {
		// ��_�����������ꍇ
		mtx._41 = pos.x;
		mtx._42 = ans.y;
		mtx._43 = pos.z;
	}
	else {
		// ��_��������Ȃ������ꍇ
		mtx._41 = pos.x;
		mtx._42 = pos.y;					// �����O
		mtx._43 = pos.z;
	}

	CaliculateParentChildMtx();
}

/// <summary>
/// �㏈��
/// </summary>
void Tank::Finalize() {

	delete obb;
}

/// <summary>
/// �e�̍s�񂩂�q�̍s��𐶐�
/// </summary>
void Tank::CaliculateParentChildMtx() {

	// ���[�J���|�[�Y��e�����Ɋ|���Ă���
	// �L���^�s��
	mtxParentChild[Tank::TANK_CATERPILLAR] = mtxlocalpose[Tank::TANK_CATERPILLAR];

	// �{��
	DX11MtxMultiply(mtxParentChild[Tank::TANK_BASE], mtxlocalpose[Tank::TANK_BASE], mtxlocalpose[Tank::TANK_CATERPILLAR]);

	// �C��						   
	DX11MtxMultiply(mtxParentChild[Tank::TANK_TOP], mtxlocalpose[Tank::TANK_TOP], mtxlocalpose[Tank::TANK_BASE]);
	DX11MtxMultiply(mtxParentChild[Tank::TANK_TOP], mtxParentChild[Tank::TANK_TOP], mtxlocalpose[Tank::TANK_CATERPILLAR]);

	// �C�g						   
	DX11MtxMultiply(mtxParentChild[Tank::TANK_BARREL], mtxlocalpose[Tank::TANK_BARREL], mtxlocalpose[Tank::TANK_TOP]);
	DX11MtxMultiply(mtxParentChild[Tank::TANK_BARREL], mtxParentChild[Tank::TANK_BARREL], mtxlocalpose[Tank::TANK_BASE]);
	DX11MtxMultiply(mtxParentChild[Tank::TANK_BARREL], mtxParentChild[Tank::TANK_BARREL], mtxlocalpose[Tank::TANK_CATERPILLAR]);

}

const XMFLOAT4X4 Tank::GetPipeMtx() const {

	if (m_fireFlg) {
		return mtxParentChild[Tank::TANK_BARREL];
		//return m_fireMtx;

	}
	else {
		return mtxParentChild[Tank::TANK_BARREL];
	}
}

void Tank::GenerateSmoke() {

	std::unique_ptr<Smoke_Billboard> smoke;

	XMFLOAT4 axisZ;
	XMFLOAT3 temppos[2];

	GetAxisZfromMatrix(mtx, axisZ);

	static float lengthx = obb->GetOBB().fLengthX / 2;
	static float lengthz = obb->GetOBB().fLengthZ / 2;

	temppos[0].x = pos.x + axisZ.x * lengthx;
	temppos[0].y = 1.0f;
	temppos[0].z = pos.z + axisZ.z * lengthz;

	temppos[1].x = pos.x + axisZ.x * lengthx;
	temppos[1].z = pos.z - axisZ.z * lengthz;

	smoke = std::make_unique<Smoke_Billboard>(temppos[0]);
	smoke->Init();
	m_smokeList.emplace_back(std::move(smoke));

	//smoke = std::make_unique<Smoke_Billboard>(temppos[1]);
	//smoke->Init();
	//m_smokeList.emplace_back(std::move(smoke));
}