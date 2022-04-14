#pragma once

#include "common.h"

#include "SA/CColSphere.h"
#include "SA/CTask.h"
#include "SA/CTaskManager.h"
#include "SA/CPedIntelligence.h"
#include "SA/CRegisteredCorona.h"
#include "SA/touchinterface.h"
#include "SA/txdstore.h"
#include "SA/widget.h"

#include "projectsa.h"
#include "quaternion.h"
#include "scripting.h"
#include "util.h"

class CGame
{
public:
	CGame();
	~CGame() {};
	
	void Process();
	
	bool IsGamePaused();
	float GetFPS();

	int IsLineOfSightClear(float fX, float fY, float fZ);
};