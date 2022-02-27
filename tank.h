#pragma once
#include    <vector>
#include    <random>
#include	"CModel.h"
#include    "Object.h"
#include    "obb.h"
#include    "Smoke_Billboard.h"

using namespace DirectX;

class BoundingSphere;
class Fire_Billboard;
class IScene;

class Tank : public Object
{
public:
	// �I�u�W�F�N�g��
	enum TANKPARTS {
		TANK_CATERPILLAR,			// �L���^�s��
		TANK_BASE,					// �{��
		TANK_TOP,					// �C��
		TANK_BARREL,				// �C�g
		NONE,						// �Ȃ�
		END,						// �I��
		PARTSMAX					// �p�[�c�̍ő吔
	};

	// �\���̌^�^�O
	struct	TankInitData {
		Tank::TANKPARTS		ParentObjectNo;		// �e�I�u�W�F�N�g�m��
		Tank::TANKPARTS		ModelNo;			// ���f���ԍ�
		XMFLOAT3			FirstPosition;		// �ŏ��̈ʒu
		XMFLOAT3			FirstAngle;			// �ŏ��̊p�x
	};

	// �^���N�̃��f�������X�g
	struct	TankModelData {
		Tank::TANKPARTS		ModelNo;					// ���f���ԍ�
		const char*			XfileName;					// �w�t�@�C����
	};

	// �����z�u�f�[�^
	static TankInitData	ObjectInitData[];

	// ���f���t�@�C�������X�g
	static TankModelData			XFileData[];

	enum TYPE {
		PLAYER = 0,
		ENEMY
	};

	float				radius;		//���a
	XMFLOAT3			angle;							// ��]�p
	bool				keyInput;						//�L�[���͂�����
	TYPE				type;							//�^�C�v
	float				speed;							// �X�s�[�h
	static constexpr float ROTATIONRATE = 0.5f;				//��]�p�x
	std::mt19937	mt;										//�j�󎞗p����

public:

	static constexpr float RANGE_STAGE = 450.0f;		//�s���͈�

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Tank();
	Tank(TYPE);
	Tank(IScene*);

	/// <summary>
	/// ���f�����Z�b�g
	/// </summary>
	/// <param name="p">���f�� </param>
	/// <param name="idx">�p�[�c�̔ԍ��@</param>
	void SetModel(CModel* p, int idx) {
		models[idx] = p;
	}

	/// <summary>
	/// �o�����̍s��擾
	/// </summary>
	/// <returns>�s��</returns>
	const XMFLOAT4X4& GetTankBarrelMtx() const { return mtxParentChild[TANKPARTS::TANK_BARREL]; }

	/// <summary>
	/// ��Ԃ�OBB�擾
	/// </summary>
	/// <returns></returns>
	const COBB::OBBinfo& GetOBB() const { return obb->GetOBB(); }

	void SetPos(const XMFLOAT3& _pos) {
		pos = _pos;
		mtxlocalpose[TANKPARTS::TANK_CATERPILLAR]._41 = pos.x;
		mtxlocalpose[TANKPARTS::TANK_CATERPILLAR]._42 = pos.y;
		mtxlocalpose[TANKPARTS::TANK_CATERPILLAR]._43 = pos.z;
	}

	/// <summary>
	/// �����_���l�Z�b�g
	/// </summary>
	/// <param name="_rnd"></param>
	void SetRand(const std::mt19937& _rnd) {
		mt = _rnd;
	}

	bool Init();
	void Draw();
	void DrawModel();
	void DrawShadow();
	void Update();

	void Finalize();

	//�G�p
	void UpdateChase(XMFLOAT4X4 _playermtx);
	void UpdateFree();
	void UpdateCycle();

	//�������̒l�Z�b�g
	void PrepareDestroy();

	//�������o
	bool DestroyEffect();

	/// <summary>
	/// �e����
	/// </summary>
	void Fire();

	//�C�g�̍s��擾
	const XMFLOAT4X4 GetPipeMtx() const;

	void UpdateSelect();

private:

	enum IKPOS {
		RIGHTUP,
		RIGHTDOWN,
		LEFTUP,
		LEFTDOWN,
	};

	/// <summary>
	/// ���[�J���|�[�Y���X�V����
	/// </summary>
	void UpdateLocalpose();	

	void UpdateEnemyLocalpose(XMFLOAT4X4 _playermtx);
	void UpdateEneLocalpose();

	/// <summary>
	/// �e�q�֌W���l�������s����쐬
	/// </summary>
	void CaliculateParentChildMtx();					

	/// <summary>
	/// ��
	/// </summary>
	void GenerateSmoke();

	CModel*				models[TANKPARTS::PARTSMAX];
	XMFLOAT4X4			mtxlocalpose[PARTSMAX];				// �����̂��Ƃ������l�����s��
	XMFLOAT4X4			mtxParentChild[PARTSMAX];			// �e�q�֌W���l�������s��
	
	XMFLOAT3            ikpos[4];							//IK�p�x�_�ʒu

	COBB*               obb;								//OBB
	
	static constexpr float ACCEL = 0.01f;					//�����x							
	static constexpr float FIRSTSP = 0.1f;					//�����x
	static constexpr float MAXSPEED = 0.3f;					//�ō����x
	static constexpr float PIPELENGTH = 20.0f;				//�p�C�v�̒���
	
	float move = 0;											//�ړ���

	float destroyRotx = 0;									//�j�󎞉�]�p
	float destroyRoty = 0;
	float destroyRotz = 0;
	float destroySpeed = 0;
	uint16_t destroyCnt = 0;

	std::vector<std::unique_ptr<Fire_Billboard>> m_fireList;
	std::vector<std::unique_ptr<Smoke_Billboard>> m_smokeList;

	uint16_t m_fireInterval = 6;
	uint16_t m_fireCnt;
	bool m_fireFlg;
	float m_fireReactionAngle = 1.0f;

	XMFLOAT4X4 m_fireMtx;

	//Lerp�p
	XMFLOAT3 lerpstart;
	XMFLOAT3 lerpend;
};