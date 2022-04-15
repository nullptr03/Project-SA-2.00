#include <android/log.h>

#include "../main.h"
#include "game.h"

extern CGame *pGame;

extern char* WORLD_PLAYERS;

PED_TYPE* FindPlayerPed(int iID)
{
	return (( PED_TYPE* (*)(int))(g_libGTASA+0x40B288+1))(iID); // FindPlayerPed
}

VEHICLE_TYPE* FindPlayerPedVehicle(int iID)
{
	PED_TYPE *pPed = FindPlayerPed(iID);
	if(pPed)
	{
		VEHICLE_TYPE *pVehicle = (VEHICLE_TYPE*)pPed->pVehicle;
		if(pVehicle) return pVehicle;
	}
	return nullptr;
}

PED_TYPE* GetPoolPed(int iID)
{
	return (( PED_TYPE* (*)(int))(g_libGTASA+0x483D49+1))(iID); // GetPoolPed
}

int GettPoolPedRef(PED_TYPE *pActor)
{
	return (( int (*)(PED_TYPE*))(g_libGTASA+0x483D3A+1))(pActor); // GettPoolPedRef
}

VEHICLE_TYPE *GetPoolVehicle(int iID)
{
	return (( VEHICLE_TYPE* (*)(int))(g_libGTASA+0x483D2E +1))(iID); // GetPoolVehicle
}

int GettPoolVehicleRef(VEHICLE_TYPE *pVehicle)
{
    return (( int (*)(VEHICLE_TYPE*))(g_libGTASA+0x483D20+1))(pVehicle); // GettPoolVehicleRef
}

ENTITY_TYPE *GetPoolObj(int iID)
{
	return (( ENTITY_TYPE* (*)(int))(g_libGTASA+0x483D62+1))(iID); // GetPoolObj
}

uintptr_t GetModelInfoByID(int iModel)
{
	if (iModel < 0 || iModel > 20000)
		return 0;

	uintptr_t *dwModelArray = (uintptr_t*)(g_libGTASA+0x91DCB8);
	return dwModelArray[iModel];
}

uint16_t GetTypeModel(int iModel)
{
	uintptr_t modelInfo = GetModelInfoByID(iModel);
	if(modelInfo)
		return *(uint16_t*)(modelInfo+0x40);
	
	return 0;
}

// 0.3.7
bool IsValidPedModel(int iModel)
{
	uintptr_t modelInfo = GetModelInfoByID(iModel);
	if(modelInfo && *(uintptr_t*)modelInfo == (uintptr_t)(g_libGTASA+0x667668))
		return true;

	return false;
}

bool IsValidVehicleModel(int iModel)
{
	uintptr_t modelInfo = GetModelInfoByID(iModel);
	if(modelInfo && *(uintptr_t*)modelInfo == (uintptr_t)(g_libGTASA+0x6676B8))
		return true;

	return false;
}

// 0.3.7
bool IsValidModel(int iModel)
{
	uintptr_t modelInfo = GetModelInfoByID(iModel);
	if(modelInfo && modelInfo != 0x14)
		return true;

	return false;
}

bool IsExistInfoForModel(int iModel)
{
	uintptr_t modelInfo = GetModelInfoByID(iModel);
	if(modelInfo != 0) return true;
	return false;
}

uint16_t GetModelRefCounts(int iModel)
{
	uintptr_t modelInfo = GetModelInfoByID(iModel);
	return *(uint16_t*)(modelInfo+0x1E);
}

uintptr_t GetModelRWObject(int iModel)
{
	uintptr_t modelInfo = GetModelInfoByID(iModel);
	return *(uintptr_t*)(modelInfo+0x34);
}

uintptr_t GetColModel(int iModel)
{
	uintptr_t modelInfo = GetModelInfoByID(iModel);
	if (modelInfo != 0) {
		uintptr_t colModel = *(uintptr_t*)(modelInfo+0x2C);
		if(colModel) return colModel;
	}
	return 0;
}

uintptr_t ModelInfoCreateInstance(int iModel)
{
	uintptr_t modelInfo = GetModelInfoByID(iModel);
	if (modelInfo != 0)
		return (( uintptr_t (*)(uintptr_t))*(uintptr_t*)(*(uintptr_t*)modelInfo+0x2C))(modelInfo);
	
	return 0;
}

float GetModelColSphereRadius(int iModel)
{
	uintptr_t colModel = GetColModel(iModel);
	return *(float*)(colModel+0x24);
}

void GetModelColSphereVecCenter(int iModel, VECTOR* vec)
{
	uintptr_t colModel = GetColModel(iModel);
	VECTOR *v = (VECTOR*)(colModel+0x18);
	if(v) {
		vec->X = v->X;
		vec->X = v->Y;
		vec->Z = v->Z;
	}
}

void SetDrawDistanceModel(int iModel, float fDistance)
{
	uintptr_t modelInfo = GetModelInfoByID(iModel);
	if(fDistance > 0.0f)
		*(float*)(modelInfo+0x30) = fDistance;
}

float GetDrawDistanceModel(int iModel)
{
	uintptr_t modelInfo = GetModelInfoByID(iModel);
	return *(float*)(modelInfo+0x30);
}

uintptr_t LoadTextureFromDB(const char* dbname, const char* texture)
{
	// TextureDatabaseRuntime::GetDatabase(dbname)
	uintptr_t db_handle = (( uintptr_t (*)(const char*))(g_libGTASA+0x1EAC8C+1))(dbname);
	if(!db_handle) return 0;
	
	// TextureDatabaseRuntime::Register(db)
	(( void (*)(uintptr_t))(g_libGTASA+0x1E9BC8+1))(db_handle);
	
	// TextureDatabaseRuntime::GetTexture
	uintptr_t tex = (( uintptr_t (*)(const char*))(g_libGTASA+0x1E9CE4+1))(texture);
	if(tex)
		++*(uint32_t *)(tex + 0x54);

	// TextureDatabaseRuntime::Unregister(db)
	(( void (*)(uintptr_t))(g_libGTASA+0x1E9C80+1))(db_handle);

	return tex;
}

uintptr_t LoadTextureFromTxd(const char* txdname, const char* texture)
{
	uintptr_t tex = 0;
	if(txdname && texture)
	{
		int txdslot = CTxdStore::FindTxdSlot(txdname);
		if(txdslot != -1)
		{
			CTxdStore::PushCurrentTxd();
			CTxdStore::SetCurrentTxd(txdslot);
			tex = (uintptr_t)RwTextureRead(texture, 0);
			CTxdStore::PopCurrentTxd();
		}
	}
	return tex;
}

void SetRenderWareCamera(RwCamera* camera)
{
	// CVisibilityPlugins::SetRenderWareCamera
	(( void (*)(RwCamera*))(g_libGTASA+0x5D61F8+1))(camera);
}

void RwObjectHasFrameSetFrame(RwCamera* camera, uintptr_t rwFrame)
{
	// RwObjectHasFrameSetFrame
	(( void (*)(RwCamera*, uintptr_t))(g_libGTASA+0x1DCFE4+1))(camera, rwFrame);
}

void RpWorldAddCamera(RwCamera* camera)
{
	uintptr_t rpWorld = *(uintptr_t*)(g_libGTASA+0x9FC938);
	if(rpWorld)
		(( void (*)(uintptr_t, RwCamera*))(g_libGTASA+0x21E004+1))(rpWorld, camera); // RpWorldAddCamera
}

void RpWorldAddLight(uintptr_t rpLight)
{
	uintptr_t rpWorld = *(uintptr_t*)(g_libGTASA+0x9FC938);
	if(rpWorld)
		(( void (*)(uintptr_t, uintptr_t))(g_libGTASA+0x21E830+1))(rpWorld, rpLight); // RpWorldAddLight
}

void RpWorldRemoveLight(uintptr_t rpLight)
{
	uintptr_t rpWorld = *(uintptr_t*)(g_libGTASA+0x9FC938);
	if(rpWorld)
		(( void (*)(uintptr_t, uintptr_t))(g_libGTASA+0x21E874+1))(rpWorld, rpLight); // RpWorldRemoveLight
}

uintptr_t RpLightCreate(int type)
{
	// RpLightCreate
	return (( uintptr_t (*)(int))(g_libGTASA+0x216E30+1))(type);
}

void RpLightSetColor(uintptr_t rpLight, float* v)
{
	// RpLightSetColor
	(( void (*)(uintptr_t, float*))(g_libGTASA+0x2167C6+1))(rpLight, v);
}

RwFrame * RwFrameCreate()
{
	// RwFrameCreate
	return (( RwFrame * (*)())(g_libGTASA+0x1D822C+1))();
}

void RwFrameTranslate(uintptr_t rwFrame, float* v, int type)
{
	// RwFrameTranslate
	(( void (*)(uintptr_t, float*, int))(g_libGTASA+0x1D8694+1))(rwFrame, v, type);
}

void RwFrameRotate(uintptr_t rwFrame, float* v, float rot, int type)
{
	// RwFrameRotate
	(( void (*)(uintptr_t, float*, float, int))(g_libGTASA+0x1D87A8+1))(rwFrame, v, rot, type);
}

RwFrame * GetFrameFromName(uintptr_t rpClump, const char* name)
{
	return (( RwFrame * (*)(uintptr_t, const char*))(g_libGTASA+0x3856A4+1))(rpClump, name);
}

uintptr_t RpAnimBlendClumpGetAssociation(uintptr_t rpClump, const char* name)
{
	return (( uintptr_t (*)(uintptr_t, const char*))(g_libGTASA+0x3909D4+1))(rpClump, name);
}

uintptr_t GetAnimHierarchyFromSkinClump(uintptr_t rpClump)
{
	return (( uintptr_t (*)(uintptr_t))(g_libGTASA+0x5D1020+1))(rpClump);
}

int RpHAnimIDGetIndex(uintptr_t animHierarchy, int iBoneID)
{
	return (( int (*)(uintptr_t, int))(g_libGTASA+0x1C2C90+1))(animHierarchy, iBoneID);
}

uintptr_t RpHAnimHierarchyGetMatrixArray(uintptr_t animHierarchy)
{
	return *(uintptr_t*)(animHierarchy+8);
}

void RpAnimBlendClumpUpdateAnimations(uintptr_t rpClump, float step, int flags)
{
	(( void (*)(uintptr_t, float, int))(g_libGTASA+0x38BF00+1))(rpClump, step, flags);
}

void DefinedState()
{
	// DefinedState
	(( void (*)())(g_libGTASA+0x5D0BC0+1))();
}

void DefinedState2d()
{
	// DefinedState2d
	(( void (*)())(g_libGTASA+0x5D0C64+1))();
}

void SetScissorRect(void* pRect)
{
	// CWidget::SetScissor
	(( void(*)(void*))(g_libGTASA+0x2B3E54+1))(pRect);
}

float DegToRad(float fDegrees)
{
	if (fDegrees > 360.0f || fDegrees < 0.0f) return 0.0f;
	if (fDegrees > 180.0f) return (float)(-(rwPI - (((fDegrees - 180.0f) * rwPI) / 180.0f)));
	else return (float)((fDegrees * rwPI) / 180.0f);
}

// 0.3.7
float FloatOffset(float f1, float f2)
{
	if(f1 >= f2) return f1 - f2;
	else return (f2 - f1);
}

// 0.3.7
float fixAngle(float angle)
{
	if (angle > 180.0f) angle -= 360.0f;
	if (angle < -180.0f) angle += 360.0f;
	return angle;
}

// 0.3.7
float subAngle(float a1, float a2)
{
	return fixAngle(fixAngle(a2) - a1);
}

uint32_t fixColor(uint32_t dwColor)
{
	return (uint32_t)__builtin_bswap32(dwColor | 0x000000FF);
}

void ProjectMatrix(VECTOR* vecOut, MATRIX4X4* mat, VECTOR *vecPos)
{
	vecOut->X = mat->at.X * vecPos->Z + mat->up.X * vecPos->Y + mat->right.X * vecPos->X + mat->pos.X;
	vecOut->Y = mat->at.Y * vecPos->Z + mat->up.Y * vecPos->Y + mat->right.Y * vecPos->X + mat->pos.Y;
	vecOut->Z = mat->at.Z * vecPos->Z + mat->up.Z * vecPos->Y + mat->right.Z * vecPos->X + mat->pos.Z;
}

void RwMatrixOrthoNormalize(MATRIX4X4 *matIn, MATRIX4X4 *matOut)
{
	(( void (*)(MATRIX4X4*, MATRIX4X4*))(g_libGTASA+0x1E34A0+1))(matIn, matOut);
}

void RwMatrixInvert(MATRIX4X4 *matOut, MATRIX4X4 *matIn)
{
	(( void (*)(MATRIX4X4*, MATRIX4X4*))(g_libGTASA+0x1E3A28+1))(matOut, matIn);
}

void RwMatrixRotate(MATRIX4X4 *mat, int axis, float angle)
{
	static float rMat[3][3] = 
	{
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f }
	};

	// RwMatrixRotate
	(( void (*)(MATRIX4X4*, float*, float, int))(g_libGTASA+0x1E3974+1))(mat, rMat[axis], angle, 1);
}

void RwMatrixScale(MATRIX4X4 *matrix, VECTOR *vecScale)
{
	matrix->right.X *= vecScale->X;
	matrix->right.Y *= vecScale->X;
	matrix->right.Z *= vecScale->X;

	matrix->up.X *= vecScale->Y;
	matrix->up.Y *= vecScale->Y;
	matrix->up.Z *= vecScale->Y;

	matrix->at.X *= vecScale->Z;
	matrix->at.Y *= vecScale->Z;
	matrix->at.Z *= vecScale->Z;

	matrix->flags &= 0xFFFDFFFC;
}

void RenderClumpOrAtomic(uintptr_t rwObject)
{
	if (rwObject)
	{
		uint8_t type = *(uint8_t*)(rwObject);

		if (type == 1)
		{
			// Atomic
			(( void (*)(uintptr_t))(*(uintptr_t*)(rwObject+0x48)))(rwObject);
		} 
		else if (type == 2)
		{
			// rpClumpRender
			(( void (*)(uintptr_t))(g_libGTASA+0x2142DC+1))(rwObject);
		}
	}
}

void DestroyAtomicOrClump(uintptr_t rwObject)
{
	if (rwObject)
	{
		uint8_t type = *(uint8_t*)(rwObject);

		if (type == 1)
		{
			// RpAtomicDestroy
			(( void(*)(uintptr_t))(g_libGTASA+0x2141EC+1))(rwObject);

			uintptr_t parent = *(uintptr_t*)(rwObject + 4);
			if (parent)
			{
				// RwFrameDestroy
				(( void(*)(uintptr_t))(g_libGTASA+0x1D846C+1))(parent);
			}

		}
		else if (type == 2)
		{
			// RpClumpDestroy
			(( void(*)(uintptr_t))(g_libGTASA+0x21460C+1))(rwObject);
		}
	}
}

void DrawTextureUV(uintptr_t texture, RECT* rect, uint32_t dwColor, float *uv)
{
	if (texture)
	{
		RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
		
		// CSprite2d::Draw
		(( void (*)(uintptr_t, RECT*, uint32_t*, float, float, float, float, float, float, float, float))(g_libGTASA+0x5C95C0+1))(texture, rect, &dwColor, uv[0], uv[1], uv[2], uv[3], uv[4], uv[5], uv[6], uv[7]);
	}
}

float GetDistance(VECTOR *vec1, VECTOR *vec2)
{
	float fX = (vec1->X - vec2->X) * (vec1->X - vec2->X);
	float fY = (vec1->Y - vec2->Y) * (vec1->Y - vec2->Y);
	float fZ = (vec1->Z - vec2->Z) * (vec1->Z - vec2->Z);

	return (float)sqrt(fX + fY + fZ);
}

void HideEntity(ENTITY_TYPE *pEntity)
{
	if(pEntity)
	{
		pEntity->vecPosBeforeMat.Z -= 2000.0f;
		if (pEntity->mat)
			pEntity->mat->pos.Z -= 2000.0f;
	}
}

const char *getGameDataStorage()
{
	return (const char*)(g_libGTASA+0x6D687C);
}

RwReal getNearScreenZ()
{
	// CSprite2d::NearScreenZ
	return *(RwReal*)(g_libGTASA+0xA7C348);
}

RwReal getRecipNearClip()
{
	// CSprite2d::RecipNearClip
	return *(RwReal*)(g_libGTASA+0xA7C344);
}