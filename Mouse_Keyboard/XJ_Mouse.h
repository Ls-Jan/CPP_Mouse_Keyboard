#pragma once
#include<Windows.h>

class XJ_Mouse {
private:
	//在进入“桌面(右键)-显示设置”中，可以看到有个系统设置是“缩放与布局”，因为这一般和显示屏(硬件)有关所以比较少的会是100%，一般是其他的值
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
	POINT Get_Position();//获取鼠标位置
	VOID Set_Position(INT x, INT y);//设置鼠标位置
	VOID Opt_Click(ButtonType button, ClickType click = ClickType::Click);//鼠标点击操作
	VOID Opt_Wheel(INT delta, BOOL horizon = FALSE);//滚轮滚动操作(正数为向下滚动)
};
