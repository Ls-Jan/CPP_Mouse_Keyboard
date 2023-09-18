#pragma once
#include<windows.h>

class XJ_Keyboard {
public:
	VOID Opt_SendKey(WORD key, BOOL vk = FALSE);//发送按键(如果按键是形如VK_RETURN的虚拟键时vk需设置为真)。(key为宽字符
	VOID Opt_SendWStr(LPCWSTR wstr);//发送字符串
	VOID Opt_SendStr(LPCSTR str);//发送字符串

	VOID Opt_PressKey(CHAR key);//按下某键不放。key为VK_虚拟键，或者是ASCII字符
	VOID Opt_ReleaseKey(CHAR key);//松开某键。key为VK_虚拟键，或者是ASCII字符
private:
	LPSTR ConvertToStr(LPCWSTR wstr, PWORD pLen = NULL);//宽字符串(WSTR)转换为多字节字符串(STR)。请使用free释放返回的指针，pLen为返回字串数组的长度
	LPWSTR ConvertToWStr(LPCSTR str, PWORD pLen = NULL);//多字节字符串(STR)转换为宽字符串(WSTR)。请使用free释放返回的指针，pLen为返回字串数组的长度
};

