#include "Stage.h"
#include "ModelMgr.h"

/// <summary>
/// ������
/// </summary>
/// <param name="modelname"> �t�@�C����</param>
/// <returns>�@�������ł�����</returns>
bool Stage_Object::Init(std::string modelname) {

	SetModel(ModelMgr::GetInstance().GetModelPtr(modelname));
	if (model == nullptr) {
		char str[128];
		sprintf_s<128>(str, "%s", modelname);
		MessageBox(nullptr, str, "load error", MB_OK);
		assert(false);
		return false;
	}

	return true;

}

/// <summary>
/// �`��
/// </summary>
void Stage_Object::Draw() {

	model->Draw(this->mtx);

}

/// <summary>
/// �`��
/// </summary>
void Stage_Object::DrawShadow() {

	model->DrawShadow(this->mtx);

}

/// <summary>
/// �`��
/// </summary>
void Stage_Object::DrawModel() {

	model->DrawModel(this->mtx);

}

/// <summary>
/// �I��
/// </summary>
void Stage_Object::Finalize() {

}