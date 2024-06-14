#include "XJ_Mouse.h"
#pragma comment(lib,"Interception/library/x86/interception.lib")

XJ_Mouse::XJ_Mouse() {
	SIZE Psize{ GetDeviceCaps(GetDC(NULL), DESKTOPHORZRES),GetDeviceCaps(GetDC(NULL), DESKTOPVERTRES) };//获取实际屏幕大小分辨率
	SIZE Lsize{ GetSystemMetrics(0) ,GetSystemMetrics(1) };//获取逻辑屏幕大小分辨率
	this->__rateX = float(Psize.cx) / Lsize.cx;
	this->__rateY = float(Psize.cy) / Lsize.cy;
	this->__context = interception_create_context();
}

XJ_Mouse::~XJ_Mouse(){
	interception_destroy_context(this->__context);
}

POINT XJ_Mouse::Get_Position() {
	POINT Lpos;
	GetCursorPos(&Lpos);//GetCursorPos这个函数往往返回的是逻辑坐标，需自行转换成实际坐标
	POINT Ppos{ Lpos.x * this->__rateX,Lpos.y * this->__rateY };
	return Ppos;
}

VOID XJ_Mouse::Set_Position(INT x, INT y) {
	SetCursorPos(x / this->__rateX, y / this->__rateY);
}

VOID XJ_Mouse::Opt_Click(ButtonType button, ClickType click) {//鼠标点击操作
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


VOID XJ_Mouse::Opt_Wheel(INT delta, BOOL horizon) {//滚轮滚动操作
	INPUT input;
	memset(&input, 0, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.mouseData = -delta;
	input.mi.dwFlags = horizon? MOUSEEVENTF_HWHEEL: MOUSEEVENTF_WHEEL;
	SendInput(1, &input, sizeof(INPUT));
}

VOID XJ_Mouse::Set_Position_Device(INT x, INT y){
	InterceptionMouseStroke mouseStroke[3];
	ZeroMemory(mouseStroke, sizeof(mouseStroke));
	// 鼠标移动到屏幕中间
	//mouseStroke[0].flags = INTERCEPTION_MOUSE_MOVE_RELATIVE;
	mouseStroke[0].flags = INTERCEPTION_MOUSE_MOVE_ABSOLUTE;
	//mouseStroke[0].x = 65535 / 2; // 坐标取值范围是0-65535
	//mouseStroke[0].y = 65535 / 2;
	mouseStroke[0].rolling = 300;
	// 点击鼠标右键
	//mouseStroke[1].state = INTERCEPTION_MOUSE_RIGHT_BUTTON_DOWN;
	//mouseStroke[2].state = INTERCEPTION_MOUSE_RIGHT_BUTTON_UP;
	//mouseStroke[1].state = INTERCEPTION_MOUSE_RIGHT_BUTTON_DOWN;
	//mouseStroke[2].state = INTERCEPTION_MOUSE_RIGHT_BUTTON_UP;
	//mouseStroke[1].state = INTERCEPTION_MOUSE_WHEEL;
	mouseStroke[0].state = INTERCEPTION_MOUSE_WHEEL;

	return VOID();
}

VOID XJ_Mouse::Opt_Click_Device(ButtonType button, ClickType click){
	InterceptionMouseStroke mouseStroke;
	ZeroMemory(&mouseStroke, sizeof(mouseStroke));

	if (click == ClickType::Click || click == ClickType::Press) {
		switch (button) {
		case ButtonType::Left:mouseStroke.state = INTERCEPTION_MOUSE_LEFT_BUTTON_DOWN; break;
		case ButtonType::Middle:mouseStroke.state = INTERCEPTION_MOUSE_MIDDLE_BUTTON_DOWN; break;
		case ButtonType::Right:mouseStroke.state = INTERCEPTION_MOUSE_RIGHT_BUTTON_DOWN; break;
		}
		interception_send(this->__context, INTERCEPTION_MOUSE(0), (InterceptionStroke*)&mouseStroke, 1);
	}
	if (click == ClickType::Click || click == ClickType::Release) {
		switch (button) {
		case ButtonType::Left:mouseStroke.state = INTERCEPTION_MOUSE_LEFT_BUTTON_UP; break;
		case ButtonType::Middle:mouseStroke.state = INTERCEPTION_MOUSE_MIDDLE_BUTTON_UP; break;
		case ButtonType::Right:mouseStroke.state = INTERCEPTION_MOUSE_RIGHT_BUTTON_UP; break;
		}
		interception_send(this->__context, INTERCEPTION_MOUSE(0), (InterceptionStroke*)&mouseStroke, 1);
	}
}

VOID XJ_Mouse::Opt_Wheel_Device(INT delta, BOOL horizon){
	InterceptionMouseStroke mouseStroke;
	ZeroMemory(&mouseStroke, sizeof(mouseStroke));
	mouseStroke.flags = INTERCEPTION_MOUSE_MOVE_ABSOLUTE;
	mouseStroke.rolling = delta;
	mouseStroke.state = horizon? INTERCEPTION_MOUSE_HWHEEL: INTERCEPTION_MOUSE_WHEEL;
	interception_send(this->__context, INTERCEPTION_MOUSE(0), (InterceptionStroke*)&mouseStroke, 1);
}
