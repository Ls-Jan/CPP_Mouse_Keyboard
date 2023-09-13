#include "XJ_Keyboard.h"

VOID XJ_Keyboard::Opt_SendKey(WORD key, BOOL vk) {//发送按键(如果按键是形如VK_RETURN的虚拟键时vk需设置为真
	INPUT input[2];
	memset(input, 0, sizeof(input));
	input[0].type = INPUT_KEYBOARD;
	if (vk) {
		input[0].ki.wVk = key;
	}
	else {
		input[0].ki.wScan = key;
		input[0].ki.dwFlags = KEYEVENTF_UNICODE;
	}
	input[1] = input[0];
	input[1].ki.dwFlags |= KEYEVENTF_KEYUP;
	SendInput(2, input, sizeof(INPUT));
}

VOID XJ_Keyboard::Opt_SendWStr(LPCWSTR wstr) {//发送字符串
	for (auto pst = 0; wstr[pst] != 0; ++pst) {
		WORD ch = wstr[pst];
		switch (ch) {
		case '\n':
			this->Opt_SendKey(VK_RETURN, TRUE);
			break;
		case '\t':
			this->Opt_SendKey(VK_TAB, TRUE);
			break;
		default:
			this->Opt_SendKey(ch, false);
		}
	}
}

VOID XJ_Keyboard::Opt_SendStr(LPCSTR str) {//发送字符串
	LPWSTR wstr = this->ConvertToWStr(str);
	this->Opt_SendWStr(wstr);
	free(wstr);
}

LPSTR XJ_Keyboard::ConvertToStr(LPCWSTR wstr, PWORD pLen) {//宽字符串(WSTR)转换为多字节字符串(STR)。请使用free释放返回的指针，pLen为返回字串数组的长度
	//WideCharToMultiByte用法：https://www.cnblogs.com/vranger/p/3793123.html
	LPSTR str;
	DWORD len;

	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, NULL, NULL, NULL);
	str = (LPSTR)malloc(len * sizeof(CHAR));
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (pLen)
		*pLen = len;
	return str;
}

LPWSTR XJ_Keyboard::ConvertToWStr(LPCSTR str, PWORD pLen) {//多字节字符串(STR)转换为宽字符串(WSTR)。请使用free释放返回的指针，pLen为返回字串数组的长度
	//MultiByteToWideChar用法：https://www.cnblogs.com/vranger/p/3793205.html
	LPWSTR wstr;
	DWORD len;

	len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	wstr = (LPWSTR)malloc(len * sizeof(WCHAR));
	MultiByteToWideChar(CP_ACP, 0, str, -1, wstr, len);
	if (pLen)
		*pLen = len;
	return wstr;
}
