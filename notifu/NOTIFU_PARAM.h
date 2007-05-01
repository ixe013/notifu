#pragma once

#include <string>


struct NOTIFU_PARAM
{
	int mDelay;
	DWORD mType;
	std::basic_string<TCHAR> mTitle;
	std::basic_string<TCHAR> mText;
	HICON mIcon;
};

