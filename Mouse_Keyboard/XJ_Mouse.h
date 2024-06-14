#pragma once
#include<Windows.h>
#include"Interception/library/interception.h"

class XJ_Mouse {
public:
	enum ClickType {
		Click,
		Press,
		Release,
	};
	enum ButtonType {
		Left,
		Middle,
		Right
	};
public:
	XJ_Mouse();
	~XJ_Mouse();
	POINT Get_Position();//��ȡ���λ��
	VOID Set_Position(INT x, INT y);//�������λ��
	VOID Opt_Click(ButtonType button, ClickType click = ClickType::Click);//���������
	VOID Opt_Wheel(INT delta, BOOL horizon = FALSE);//���ֹ�������(����Ϊ���¹���)
public://����Ϊ������
	VOID Set_Position_Device(INT x, INT y);//�������λ��
	VOID Opt_Click_Device(ButtonType button, ClickType click = ClickType::Click);//���������
	VOID Opt_Wheel_Device(INT delta, BOOL horizon = FALSE);//���ֹ�������(����Ϊ���¹���)

private:
	//�ڽ��롰����(�Ҽ�)-��ʾ���á��У����Կ����и�ϵͳ�����ǡ������벼�֡����ò�������ʾ��(Ӳ��)�йأ����ԱȽ��ٵĻ���100%��һ����������ֵ
	InterceptionContext __context;
	float __rateX, __rateY;
};
