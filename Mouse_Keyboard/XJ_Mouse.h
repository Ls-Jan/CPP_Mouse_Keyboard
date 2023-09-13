#pragma once
#include<Windows.h>

class XJ_Mouse {
private:
	//�ڽ��롰����(�Ҽ�)-��ʾ���á��У����Կ����и�ϵͳ�����ǡ������벼�֡�����Ϊ��һ�����ʾ��(Ӳ��)�й����ԱȽ��ٵĻ���100%��һ����������ֵ
	float __rateX, __rateY;
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
	POINT Get_Position();//��ȡ���λ��
	VOID Set_Position(INT x, INT y);//�������λ��
	VOID Opt_Click(ButtonType button, ClickType click = ClickType::Click);//���������
	VOID Opt_Wheel(INT delta, BOOL horizon = FALSE);//���ֹ�������(����Ϊ���¹���)
};
