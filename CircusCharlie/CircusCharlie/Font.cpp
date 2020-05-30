#include "Font.h"



Font::Font()
{
}

void Font::Init()
{
	m_Fonthdc = BitMapManager::GetSingleton()->GetBufferDC();
	hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, OEM_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("Terminal"));
	OldFont = (HFONT)SelectObject(m_Fonthdc, hFont);
}

void Font::Text(int x, int y, LPCWSTR text, COLORREF color)
{

	TCHAR str[128];

	wsprintf(str, text);

	SetBkColor(m_Fonthdc, 0x00000000);
	SetTextColor(m_Fonthdc, color);


	TextOut(m_Fonthdc, x, y, str, lstrlen(str));
}


Font::~Font()
{
}
