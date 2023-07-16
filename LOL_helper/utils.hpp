#pragma once
#include	<windows.h>
#include    <string>

static	void CopyTextToClipboard(const std::wstring& text) {
	if (OpenClipboard(nullptr)) {
		EmptyClipboard();

		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (text.length() + 1) * sizeof(wchar_t));
		if (hMem) {
			wchar_t* pMem = (wchar_t*)GlobalLock(hMem);
			if (pMem) {
				wcscpy_s(pMem, text.length() + 1, text.c_str());
				GlobalUnlock(hMem);
				SetClipboardData(CF_UNICODETEXT, hMem);
			}
		}
		CloseClipboard();
	}
}
