#pragma once
#include<windows.h>

class XJ_Keyboard {
public:
	VOID Opt_SendKey(WORD key, BOOL vk = TRUE);//���Ͱ���(�������������VK_RETURN�������ʱvk������Ϊ��
	VOID Opt_SendWStr(LPCWSTR wstr);//�����ַ���
	VOID Opt_SendStr(LPCSTR str);//�����ַ���

private:
	LPSTR ConvertToStr(LPCWSTR wstr, PWORD pLen = NULL);//���ַ���(WSTR)ת��Ϊ���ֽ��ַ���(STR)����ʹ��free�ͷŷ��ص�ָ�룬pLenΪ�����ִ�����ĳ���
	LPWSTR ConvertToWStr(LPCSTR str, PWORD pLen = NULL);//���ֽ��ַ���(STR)ת��Ϊ���ַ���(WSTR)����ʹ��free�ͷŷ��ص�ָ�룬pLenΪ�����ִ�����ĳ���
};

