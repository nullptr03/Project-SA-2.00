#pragma once

#include "game/RW/RenderWare.h"

PED_TYPE* FindPlayerPed(int iID = -1);
VEHICLE_TYPE *FindPlayerPedVehicle(int iID = -1);
PED_TYPE* GetPoolPed(int iID);
int GettPoolPedRef(PED_TYPE *pActor);

VEHICLE_TYPE *GetPoolVehicle(int iID);
int GettPoolVehicleRef(VEHICLE_TYPE *pVehicle);

ENTITY_TYPE *GetPoolObj(int iID);

uintptr_t GetModelInfoByID(int iModel);
uint16_t GetTypeModel(int iModel);
bool IsValidPedModel(int iModel);
bool IsValidVehicleModel(int iModel);
bool IsValidModel(int iModel);
bool IsExistInfoForModel(int iModel);
uint16_t GetModelRefCounts(int iModel);
uintptr_t GetModelRWObject(int iModel);
uintptr_t GetColModel(int iModel);
uintptr_t ModelInfoCreateInstance(int iModel);
float GetModelColSphereRadius(int iModel);
void GetModelColSphereVecCenter(int iModel, VECTOR* vec);
void SetDrawDistanceModel(int iModel, float fDistance);
float GetDrawDistanceModel(int iModel);

uintptr_t LoadTextureFromDB(const char* dbname, const char* texture);
uintptr_t LoadTextureFromTxd(const char* txdname, const char* texture);

void SetRenderWareCamera(RwCamera* camera);
void RwObjectHasFrameSetFrame(RwCamera* camera, uintptr_t rwFrame);
void RpWorldAddCamera(RwCamera* camera);
void RpWorldAddLight(uintptr_t rpLight);
void RpWorldRemoveLight(uintptr_t rpLight);

uintptr_t RpLightCreate(int type);
void RpLightSetColor(uintptr_t rpLight, float* v);

RwFrame * RwFrameCreate();
void RwFrameTranslate(uintptr_t rwFrame, float* v, int type);
void RwFrameRotate(uintptr_t rwFrame, float* v, float rot, int type);
RwFrame * GetFrameFromName(uintptr_t rpClump, const char* name);

uintptr_t RpAnimBlendClumpGetAssociation(uintptr_t rpClump, const char* name);
uintptr_t GetAnimHierarchyFromSkinClump(uintptr_t rpClump);
int RpHAnimIDGetIndex(uintptr_t animHierarchy, int iBoneID);
uintptr_t RpHAnimHierarchyGetMatrixArray(uintptr_t animHierarchy);
void RpAnimBlendClumpUpdateAnimations(uintptr_t rpClump, float step, int flags);

void DefinedState();
void DefinedState2d();
void SetScissorRect(void* pRect);
float DegToRad(float fDegrees);

float FloatOffset(float f1, float f2);
float fixAngle(float angle);
float subAngle(float a1, float a2);

void ProjectMatrix(VECTOR* vecOut, MATRIX4X4* mat, VECTOR *vecPos);
void RwMatrixOrthoNormalize(MATRIX4X4 *matIn, MATRIX4X4 *matOut);
void RwMatrixInvert(MATRIX4X4 *matOut, MATRIX4X4 *matIn);
void RwMatrixRotate(MATRIX4X4 *mat, int axis, float angle);
void RwMatrixScale(MATRIX4X4 *mat, VECTOR *vecScale);

void DrawTextureUV(uintptr_t texture, RECT* rect, uint32_t dwColor, float *uv);

void DestroyAtomicOrClump(uintptr_t rwObject);
void RenderClumpOrAtomic(uintptr_t rwObject);
float GetDistance(VECTOR *vec1, VECTOR *vec2);

void HideEntity(ENTITY_TYPE *pEntity);

const char *getGameDataStorage();
RwReal getNearScreenZ();
RwReal getRecipNearClip();