#include "main.h"
#include "game/game.h"

void CWidget::ToggleWindowPosition(bool bToggle)
{
	unsigned int flags = this->m_nFlags;
	if(bToggle)
	{
		this->m_nFlags |= 0x1000;
	}
	else if(this->m_nFlags & 0x1000)
	{
		this->m_nFlags = flags - 4096;
	}
}

bool CWidget::IsWindowPosition()
{
	return (bool)(this->m_nFlags & 0x1000) ? true : false;
}

void CWidget::SetEnabled(bool bEnabled)
{
	this->m_bEnabled = bEnabled;
}

bool CWidget::GetEnabled()
{
	return this->m_bEnabled;
}

uint8_t CWidget::GetColor()
{
	return this->m_byteColor;
}