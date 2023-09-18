#include "XJ_Mouse.h"

XJ_Mouse::XJ_Mouse() {
	SIZE Psize{ GetDeviceCaps(GetDC(NULL), DESKTOPHORZRES),GetDeviceCaps(GetDC(NULL), DESKTOPVERTRES) };//��ȡʵ����Ļ��С�ֱ���
	SIZE Lsize{ GetSystemMetrics(0) ,GetSystemMetrics(1) };//��ȡ�߼���Ļ��С�ֱ���
	this->__rateX = float(Psize.cx) / Lsize.cx;
	this->__rateY = float(Psize.cy) / Lsize.cy;
}

POINT XJ_Mouse::Get_Position() {
	POINT Lpos;
	GetCursorPos(&Lpos);//GetCursorPos��������������ص����߼����꣬������ת����ʵ������
	POINT Ppos{ Lpos.x * this->__rateX,Lpos.y * this->__rateY };
	return Ppos;
}

VOID XJ_Mouse::Set_Position(INT x, INT y) {
	SetCursorPos(x / this->__rateX, y / this->__rateY);
}

VOID XJ_Mouse::Opt_Click(ButtonType button, ClickType click) {//���������
	INPUT input;
	memset(&input, 0, sizeof(INPUT));
	input.type = INPUT_MOUSE;

	if (click == ClickType::Click || click == ClickType::Press) {
		switch (button) {
		case ButtonType::Left:input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN; break;
		case ButtonType::Middle:input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN; break;
		case ButtonType::Right:input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN; break;
		}
		SendInput(1, &input, sizeof(INPUT));
	}
	if (click == ClickType::Click || click == ClickType::Release) {
		switch (button) {
		case ButtonType::Left:input.mi.dwFlags = MOUSEEVENTF_LEFTUP; break;
		case ButtonType::Middle:input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP; break;
		case ButtonType::Right:input.mi.dwFlags = MOUSEEVENTF_RIGHTUP; break;
		}
		SendInput(1, &input, sizeof(INPUT));
	}
}


VOID XJ_Mouse::Opt_Wheel(INT delta, BOOL horizon) {//���ֹ�������
	INPUT input;
	memset(&input, 0, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.mouseData = -delta;
	input.mi.dwFlags = horizon? MOUSEEVENTF_HWHEEL: MOUSEEVENTF_WHEEL;
	SendInput(1, &input, sizeof(INPUT));
}
