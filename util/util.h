#pragma once

void cp1251_to_utf8(char *out, const char *in, unsigned int len = 0);

void FLog(const char *fmt, ...);
void SVLog(const char *fmt, ...);

int FormatChatBubbleText(char* szText, int iWidth, int iMaxWord);