#include "main.h"
#include "game/game.h"

CWidget *CTouchInterface::getWidgets(int widgetId)
{
    CWidget **pWidgets = (CWidget**)(g_libGTASA+0x6F3794);
    return pWidgets[widgetId];
}