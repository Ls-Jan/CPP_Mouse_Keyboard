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
	POINT Get_Position();//获取鼠标位置
	VOID Set_Position(INT x, INT y);//设置鼠标位置
	VOID Opt_Click(ButtonType button, ClickType click = ClickType::Click);//鼠标点击操作
	VOID Opt_Wheel(INT delta, BOOL horizon = FALSE);//滚轮滚动操作(正数为向下滚动)
public://以下为驱动版
	VOID Set_Position_Device(INT x, INT y);//设置鼠标位置
	VOID Opt_Click_Device(ButtonType button, ClickType click = ClickType::Click);//鼠标点击操作
	VOID Opt_Wheel_Device(INT delta, BOOL horizon = FALSE);//滚轮滚动操作(正数为向下滚动)

private:
	//在进入“桌面(右键)-显示设置”中，可以看到有个系统设置是“缩放与布局”，该参数和显示屏(硬件)有关，所以比较少的会是100%，一般是其他的值
	InterceptionContext __context;
	float __rateX, __rateY;
};
