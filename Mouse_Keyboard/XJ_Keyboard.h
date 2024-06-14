#pragma once
#include<windows.h>

#include"Interception/library/interception.h"
class XJ_Keyboard {
public:
	XJ_Keyboard();
	~XJ_Keyboard();

public:
	VOID Opt_SendKey(WORD key, BOOL vk = FALSE);//���Ͱ���(�������������VK_RETURN�������ʱvk������Ϊ��)��(keyΪ���ַ�
	VOID Opt_SendWStr(LPCWSTR wstr);//�����ַ���
	VOID Opt_SendStr(LPCSTR str);//�����ַ���
	VOID Opt_PressKey(CHAR key);//����ĳ�����š�keyΪVK_�������������ASCII�ַ�
	VOID Opt_ReleaseKey(CHAR key);//�ɿ�ĳ����keyΪVK_�������������ASCII�ַ�
public://����Ϊ�����棬�޷����ͳ�Ӣ���ַ�����������ַ�����֧�ִ�д
	VOID Opt_SendKey_Device(CHAR key);//���Ͱ���
	VOID Opt_PressKey_Device(CHAR key);//����ĳ�����š�keyΪVK_�������������ASCII�ַ�
	VOID Opt_ReleaseKey_Device(CHAR key);//�ɿ�ĳ����keyΪVK_�������������ASCII�ַ�

private:
	LPSTR ConvertToStr(LPCWSTR wstr, PWORD pLen = NULL);//���ַ���(WSTR)ת��Ϊ���ֽ��ַ���(STR)����ʹ��free�ͷŷ��ص�ָ�룬pLenΪ�����ִ�����ĳ���
	LPWSTR ConvertToWStr(LPCSTR str, PWORD pLen = NULL);//���ֽ��ַ���(STR)ת��Ϊ���ַ���(WSTR)����ʹ��free�ͷŷ��ص�ָ�룬pLenΪ�����ִ�����ĳ���
private:
	InterceptionContext __context;
	InterceptionKeyStroke __keyStroke[2];
};

