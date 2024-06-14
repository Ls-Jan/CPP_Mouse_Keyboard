#include "XJ_Keyboard.h"
#pragma comment(lib,"Interception/library/x86/interception.lib")

XJ_Keyboard::XJ_Keyboard(){
	InterceptionKeyStroke* keyStroke=this->__keyStroke;
	ZeroMemory(keyStroke, 2*sizeof(InterceptionKeyStroke));
	keyStroke[0].state = INTERCEPTION_KEY_DOWN;
	keyStroke[1].state = INTERCEPTION_KEY_UP;
	this->__context = interception_create_context();
}

XJ_Keyboard::~XJ_Keyboard(){
	interception_destroy_context(this->__context);
}

VOID XJ_Keyboard::Opt_SendKey(WORD key, BOOL vk) {//发送按键(如果按键是形如VK_RETURN的虚拟键时vk需设置为真
	INPUT input[2];
	memset(input, 0, sizeof(INPUT));
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

VOID XJ_Keyboard::Opt_PressKey(CHAR key) {
	INPUT input;
	memset(&input, 0, sizeof(INPUT));
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = key;
	SendInput(1, &input, sizeof(INPUT));
	return VOID();
}

VOID XJ_Keyboard::Opt_ReleaseKey(CHAR key) {
	INPUT input;
	memset(&input, 0, sizeof(INPUT));
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = key;
	input.ki.dwFlags |= KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}

VOID XJ_Keyboard::Opt_SendKey_Device(CHAR key){
	auto keyStroke = this->__keyStroke;
	keyStroke[0].code = MapVirtualKey(key, MAPVK_VK_TO_VSC);
	keyStroke[1].code = keyStroke[0].code;
	interception_send(this->__context, INTERCEPTION_KEYBOARD(0), (InterceptionStroke*)keyStroke, 2);
}

VOID XJ_Keyboard::Opt_PressKey_Device(CHAR key){
	auto keyStroke = this->__keyStroke;
	keyStroke[0].code = MapVirtualKey(key, MAPVK_VK_TO_VSC);
	interception_send(this->__context, INTERCEPTION_KEYBOARD(0), (InterceptionStroke*)keyStroke, 1);
}

VOID XJ_Keyboard::Opt_ReleaseKey_Device(CHAR key){
	auto keyStroke = this->__keyStroke;
	keyStroke[1].code = MapVirtualKey(key, MAPVK_VK_TO_VSC);
	interception_send(this->__context, INTERCEPTION_KEYBOARD(0), (InterceptionStroke*)keyStroke, 1);
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

