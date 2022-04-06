#pragma once

class CWidget
{
public:
	char _pad0[2];				// 0000-0002
	RwTexture *m_pTexture;		// 0002-0006
	char _pad1[67];				// 0006-0073
	uint8_t m_byteColor;		// 0073-0074
	char _pad2[3];				// 0074-0077
	bool m_bEnabled;			// 0077-0078
	char _pad3[50];				// 0078-0128
	unsigned int m_nFlags;		// 0128-0129

public:
	void ToggleWindowPosition(bool bToggle);
	bool IsWindowPosition();

	void SetEnabled(bool bEnabled);
	bool GetEnabled();

	uint8_t GetColor();
};