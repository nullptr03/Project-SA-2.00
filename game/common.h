#pragma once

#include "RW/RenderWare.h"

class CPedIntelligence;

#define PLAYER_PED_SLOTS	210

typedef unsigned short VEHICLEID;
typedef unsigned short PLAYERID;

#define PADDING(x, y) uint8_t x[y]

#define IN_VEHICLE(x) ((x->dwStateFlags & 0x100) >> 8)

#pragma pack(1)
typedef struct _RECT
{
	float fLeft;
	float fBottom;
	float fRight;
	float fTop;
} RECT, *PRECT;

#pragma pack(1)
typedef struct _VECTOR
{
	float X, Y, Z;
} VECTOR, *PVECTOR;

#pragma pack(1)
typedef struct _MATRIX4X4 
{
	VECTOR right;		// 0-12 	; r11 r12 r13
	uint32_t  flags;	// 12-16
	VECTOR up;			// 16-28	; r21 r22 r23
	float  pad_u;		// 28-32
	VECTOR at;			// 32-44	; r31 r32 r33
	float  pad_a;		// 44-48
	VECTOR pos;			// 48-60
	float  pad_p;		// 60-64
} MATRIX4X4, *PMATRIX4X4;

#pragma pack(1)
typedef struct _ENTITY_TYPE
{
	// ENTITY STUFF
	uint32_t vtable; 			// 0-4		;vtable
	VECTOR vecPosBeforeMat;  	// 4-16
	float fRotZBeforeMat;		// 16-20	;fRotZBeforeMat
	MATRIX4X4 *mat; 			// 20-24	;mat
	uintptr_t pdwRenderWare;	// 24-28	;rwObject
	uint32_t dwProcessingFlags; // 28-32	;dwProcessingFlags
	PADDING(_pad1, 6);			// 32-34
	uint16_t nModelIndex; 		// 34-36	;nModelIndex
	PADDING(_pad2, 11); 		// 36-47
	uint8_t byteAreaCode; 		// 47-48
	PADDING(_pad3, 6); 		 	// 48-54
	uint8_t nControlFlags; 		// 54-55 	;nControlFlags
	PADDING(_pad4, 13); 		// 55-68
	VECTOR vecMoveSpeed; 		// 68-80	;vecMoveSpeed
	VECTOR vecTurnSpeed; 		// 80-92	;vecTurnSpeed
	PADDING(_pad5, 88); 		// 92-180
	uintptr_t dwUnkModelRel; 	// 180-184	;dwUnkModelRel
} ENTITY_TYPE;

#pragma pack(1)
typedef struct _WEAPON_SLOT_TYPE
{
	uint32_t dwType;
	uint32_t dwState;
	uint32_t dwAmmoInClip;
	uint32_t dwAmmo;
	PADDING(_pwep1, 12);
} WEAPON_SLOT_TYPE;  // MUST BE EXACTLY ALIGNED TO 28 bytesz

#pragma pack(1)
typedef struct
{
	char unk[0x14];
	int iNodeId;
} AnimBlendFrameData;

#pragma pack(1)
typedef struct _PED_TYPE
{
	ENTITY_TYPE entity; 		// 0000-0184
	PADDING(_pad106, 174);		// 0184-0358
	uint32_t _pad107;			// 0358-0362
	PADDING(_pad101, 722);		// 0362-1084
	CPedIntelligence *pIntelligence; 	// 1084-1088
	uint32_t dwPlayerInfoOffset;	// 1088-1092
	PADDING(_pad102, 4);		// 1092-1096
	uint32_t dwAction;			// 1096-1100
	PADDING(_pad228, 52);		// 1100-1152
	uint32_t dwStateFlags; 		// 1152-1156
	PADDING(_pad201, 12);		// 1156-1168
	AnimBlendFrameData* apBones[19];	// 1168 - 1244
	PADDING(_pad103, 100);		// 1244-1344
	float fHealth;		 		// 1344-1348
	float fMaxHealth;			// 1348-1352
	float fArmour;				// 1352-1356
	PADDING(_pad104, 12);		// 1356-1368
	float fRotation1;			// 1368-1372
	float fRotation2;			// 1372-1376
	PADDING(_pad105, 44);		// 1376-1420
	uint32_t pVehicle;			// 1420-1424
	PADDING(_pad108, 8);		// 1424-1432
	uint32_t dwPedType;			// 1432-1436
	uint32_t dwUnk1;			// 1436-1440
	WEAPON_SLOT_TYPE WeaponSlots[13]; // 1440-1804
	PADDING(_pad270, 12);		// 1804-1816
	uint8_t byteCurWeaponSlot;	// 1816-1817
	PADDING(_pad280, 23);		// 1817-1840
	uintptr_t pFireObject;		// 1840-1844
	PADDING(_pad281, 44);		// 1844-1888
	uint32_t  dwWeaponUsed;		// 1888-1892
	uintptr_t pdwDamageEntity;	// 1892-1896
	PADDING(_pad290, 36);		// 1896-1932
	uintptr_t pEntryExit;		// 1932-1936
} PED_TYPE;

#pragma pack(1)
typedef struct _VEHICLE_TYPE
{
	ENTITY_TYPE entity;			// 0000-0184
	PADDING(_pad201, 880);		// 0184-0312
	uint8_t byteFlags;			// 1064-1065
	PADDING(_pad210, 4);		// 1065-1069
	uint8_t _pad211  : 7;		// 1069-1070 (bits 0..6)
	uint8_t byteSirenOn : 1;	// 1069-1070 (bit 7)
	PADDING(_pad212, 6);		// 1070-1076
	uint8_t byteColor1;			// 1076-1077
	uint8_t byteColor2;			// 1077-1078
	PADDING(_pad182, 4);		// 1078-1082
	unsigned short anUpgrades[15]; // 1082-1112
	PADDING(_pad204, 8);		// 1112-1120
	PED_TYPE *pDriver;			// 1120-1124
	PED_TYPE *pPassengers[7];	// 1124-1152
	PADDING(_pad235, 8);		// 1152-1160
	uint8_t byteNumOfSeats;		// 1160-1161
	PADDING(_pad236, 7);		// 1161-1168
	uintptr_t pFireObject;		// 1168-1172
	PADDING(_pad241, 20);		// 1172-1192
	uint8_t byteOverrideLights : 2;	// 1192-1193
	PADDING(_pad202, 31);		// 1193-1224
	float fHealth;				// 1224-1228
	uint32_t dwTractor;			// 1228-1232
	uint32_t dwTrailer;			// 1232-1236
	PADDING(_pad203, 48);		// 1236-1284
	uint32_t dwDoorsLocked;		// 1284-1288
	PADDING(_pad224, 172);		// 1288-1460
	union {
		struct {
			PADDING(_pad221, 1);	// 1460-1461
			uint8_t bCarWheelPopped[4]; // 1461-1465
		};
		struct {
			float fTrainSpeed;	// 1460-1464
			PADDING(_pad222, 1); // 1464-1465
		};
	};
	uint32_t dwDoorsDamageStatus; // 1465-1469
	PADDING(_pad153, 3);		// 1469-1472
	uint32_t dwLightsDamageStatus; // 1472-1476
	uint32_t dwPanelsDamageStatus; // 1476-1480
	PADDING(_pad243, 28);		// 1480-1508
	uint32_t *dwVehAttachedFront;	// 1508-1512
	uint32_t *dwVehAttachedBottom;	// 1512-1516
	PADDING(_pad164, 128);		// 1516-1644
	uint8_t bBikeWheelPopped[2];// 1644-1646
	PADDING(_pad244, 526);		// 1646-2172
	uint32_t dwHydraThrusters;	// 2172-2176
	PADDING(_pad245, 348);		// 2176-2524
	float fPlaneLandingGear;	// 2524-2528
} VEHICLE_TYPE;

#pragma pack(1)
typedef struct _BULLET_DATA
{
	VECTOR vecOrigin;
	VECTOR vecPos;
	VECTOR vecOffset;
	ENTITY_TYPE* pEntity;
} BULLET_DATA;

#pragma pack(1)
typedef struct _NEW_ATTACHED_OBJECT
{
	int			iModel;
	int			iBone;
	VECTOR		vecOffset;
	VECTOR		vecRotation;
	VECTOR		vecScale;
	uint32_t 	dwMaterialColor1;
	uint32_t	dwMaterialColor2;
} NEW_ATTACHED_OBJECT;

#pragma pack(1)
typedef struct
{
	uint16_t wModel;
	VECTOR vecPos;
	float fRadius;
} stRemoveBuilding;

#pragma pack(1)
typedef struct _PED_MODEL
{
	uintptr_t 	vtable;
	uint8_t		data[88];
} PED_MODEL;

#pragma pack(1)
typedef struct _ATOMIC_MODEL
{
	uintptr_t	vtable;
	uint8_t		data[56];
} ATOMIC_MODEL;

#pragma pack(1)
typedef struct stOccluders {
	unsigned short fMidX;
	unsigned short fMidY;
	unsigned short fMidZ;
	unsigned short fWidthX;
	unsigned short fWidthY;
	unsigned short fHeight;
	char cRotX;
	char cRotY;
	char cRotZ;
	char cPad;
	unsigned short nFlags;
};

#pragma pack(1)
typedef struct _VEH_MODELINFO
{
	PADDING(_pad0, 15);				// 00-15
	RpMaterial* unkMat;				// 15-19
	PADDING(_pad15, 41);			// 19-60
	RpMaterial* rpMaterialPlate; 	// 60-64
	char szTextPlate[8];			// 64-72
	char unkchar;					// 73-74
	PADDING(_pad74, 100);
} VEH_MODELINFO;

#pragma pack(1)
typedef struct _PED_DAMAGE
{
	ENTITY_TYPE*	pDamager;
	float			fDamageFactor;
	uint32_t		dwbodyPart;
	uint32_t		dwweaponType;
	bool			bSpeak;
} PED_DAMAGE;

#define	VEHICLE_SUBTYPE_CAR				1
#define	VEHICLE_SUBTYPE_BIKE			2
#define	VEHICLE_SUBTYPE_HELI			3
#define	VEHICLE_SUBTYPE_BOAT			4
#define	VEHICLE_SUBTYPE_PLANE			5
#define	VEHICLE_SUBTYPE_PUSHBIKE		6
#define	VEHICLE_SUBTYPE_TRAIN			7

#define TRAIN_PASSENGER_LOCO			538
#define TRAIN_FREIGHT_LOCO				537
#define TRAIN_PASSENGER					570
#define TRAIN_FREIGHT					569
#define TRAIN_TRAM						449
#define HYDRA							520

#define ACTION_WASTED					55
#define ACTION_DEATH					54
#define ACTION_INCAR					50
#define ACTION_NORMAL					1
#define ACTION_SCOPE					12
#define ACTION_NONE						0 

// ---- weapon id defines ----
#define WEAPON_BRASSKNUCKLE				1
#define WEAPON_GOLFCLUB					2
#define WEAPON_NITESTICK				3
#define WEAPON_KNIFE					4
#define WEAPON_BAT						5
#define WEAPON_SHOVEL					6
#define WEAPON_POOLSTICK				7
#define WEAPON_KATANA					8
#define WEAPON_CHAINSAW					9
#define WEAPON_DILDO					10
#define WEAPON_DILDO2					11
#define WEAPON_VIBRATOR					12
#define WEAPON_VIBRATOR2				13
#define WEAPON_FLOWER					14
#define WEAPON_CANE						15
#define WEAPON_GRENADE					16
#define WEAPON_TEARGAS					17
#define WEAPON_MOLTOV					18
#define WEAPON_ROCKET					19
#define WEAPON_ROCKET_HS				20
#define WEAPON_FREEFALLBOMB				21
#define WEAPON_COLT45					22
#define WEAPON_SILENCED					23
#define WEAPON_DEAGLE					24
#define WEAPON_SHOTGUN					25
#define WEAPON_SAWEDOFF					26
#define WEAPON_SHOTGSPA					27
#define WEAPON_UZI						28
#define WEAPON_MP5						29
#define WEAPON_AK47						30
#define WEAPON_M4						31
#define WEAPON_TEC9						32
#define WEAPON_RIFLE					33
#define WEAPON_SNIPER					34
#define WEAPON_ROCKETLAUNCHER			35
#define WEAPON_HEATSEEKER				36
#define WEAPON_FLAMETHROWER				37
#define WEAPON_MINIGUN					38
#define WEAPON_SATCHEL					39
#define WEAPON_BOMB						40
#define WEAPON_SPRAYCAN					41
#define WEAPON_FIREEXTINGUISHER			42
#define WEAPON_CAMERA					43
#define WEAPON_NIGHTVISION				44
#define WEAPON_INFRARED					45
#define WEAPON_PARACHUTE				46
#define WEAPON_ARMOUR					47
#define WEAPON_VEHICLE					49
#define WEAPON_HELIBLADES				50
#define WEAPON_EXPLOSION				51
#define WEAPON_DROWN					53
#define WEAPON_COLLISION				54
#define WEAPON_UNKNOWN					255

//---- weapon model defines ----
#define WEAPON_MODEL_BRASSKNUCKLE		331 // was 332
#define WEAPON_MODEL_GOLFCLUB			333
#define WEAPON_MODEL_NITESTICK			334
#define WEAPON_MODEL_KNIFE				335
#define WEAPON_MODEL_BAT				336
#define WEAPON_MODEL_SHOVEL				337
#define WEAPON_MODEL_POOLSTICK			338
#define WEAPON_MODEL_KATANA				339
#define WEAPON_MODEL_CHAINSAW			341
#define WEAPON_MODEL_DILDO				321
#define WEAPON_MODEL_DILDO2				322
#define WEAPON_MODEL_VIBRATOR			323
#define WEAPON_MODEL_VIBRATOR2			324
#define WEAPON_MODEL_FLOWER				325
#define WEAPON_MODEL_CANE				326
#define WEAPON_MODEL_GRENADE			342 // was 327
#define WEAPON_MODEL_TEARGAS			343 // was 328
#define WEAPON_MODEL_MOLTOV				344 // was 329
#define WEAPON_MODEL_COLT45				346
#define WEAPON_MODEL_SILENCED			347
#define WEAPON_MODEL_DEAGLE				348
#define WEAPON_MODEL_SHOTGUN			349
#define WEAPON_MODEL_SAWEDOFF			350
#define WEAPON_MODEL_SHOTGSPA			351
#define WEAPON_MODEL_UZI				352
#define WEAPON_MODEL_MP5				353
#define WEAPON_MODEL_AK47				355
#define WEAPON_MODEL_M4					356
#define WEAPON_MODEL_TEC9				372
#define WEAPON_MODEL_RIFLE				357
#define WEAPON_MODEL_SNIPER				358
#define WEAPON_MODEL_ROCKETLAUNCHER		359
#define WEAPON_MODEL_HEATSEEKER			360
#define WEAPON_MODEL_FLAMETHROWER		361
#define WEAPON_MODEL_MINIGUN			362
#define WEAPON_MODEL_SATCHEL			363
#define WEAPON_MODEL_BOMB				364
#define WEAPON_MODEL_SPRAYCAN			365
#define WEAPON_MODEL_FIREEXTINGUISHER	366
#define WEAPON_MODEL_CAMERA				367
#define WEAPON_MODEL_NIGHTVISION		368	// newly added
#define WEAPON_MODEL_INFRARED			369	// newly added
#define WEAPON_MODEL_JETPACK			370	// newly added
#define WEAPON_MODEL_PARACHUTE			371

#define OBJECT_PARACHUTE				3131
#define OBJECT_CJ_CIGGY					1485
#define OBJECT_DYN_BEER_1				1486
#define OBJECT_CJ_BEER_B_2				1543
#define OBJECT_CJ_PINT_GLASS			1546