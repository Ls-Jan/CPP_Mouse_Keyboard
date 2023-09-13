

#include"XJ_Mouse.h"
#include"XJ_Keyboard.h"

#include<stdio.h>
#include <locale.h>

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "");//设置代码页，防中文乱码：https://learn.microsoft.com/zh-cn/cpp/c-runtime-library/code-pages?view=msvc-170
	if (argc > 1) {//argv第一个往往是程序路径，忽视		
		if (strcmp(argv[1], "/M") == 0 || strcmp(argv[1], "/m") == 0) {
			XJ_Mouse mouse;
			if (argc == 2) {//返回鼠标位置
				POINT pos = mouse.Get_Position();
				printf_s("%d,%d", pos.x, pos.y);
			}
			else {//设置鼠标位置
				try {
					char* data = argv[2];
					char* back = strchr(data, ',');
					if (back && (strlen(back) > 1)) {
						back += 1;//舍弃逗号
						char front[128];
						int len = back - data - 1;
						memset(front, 0, 128);
						strncpy_s(front, data, min(len, 127));//截取
						int posX = atoi(front);
						int posY = atoi(back);
						mouse.Set_Position(posX, posY);
					}
					else {
						throw - 1;
					}
				}
				catch (...) {
					printf_s("【输入的参数有误(格式：xxx,yyy)，请重新核对！】\n");
				}
			}
			return 0;
		}
		else if (strcmp(argv[1], "/K") == 0 || strcmp(argv[1], "/k") == 0) {
			XJ_Keyboard keyboard;
			if (argc > 2) {//发送文本
				keyboard.Opt_SendStr(argv[2]);
			}
			else {
				printf_s("【输入参数不足，缺少“发送”内容】\n");
			}
			return 0;
		}
		else if (strcmp(argv[1], "/C") == 0 || strcmp(argv[1], "/c") == 0) {
			XJ_Mouse mouse;
			if (argc > 2) {//鼠标点击
				if (strcmp(argv[2], "L") == 0 || strcmp(argv[2], "l") == 0)
					mouse.Opt_Click(XJ_Mouse::ButtonType::Left);
				else if (strcmp(argv[2], "M") == 0 || strcmp(argv[2], "m") == 0)
					mouse.Opt_Click(XJ_Mouse::ButtonType::Middle);
				else if (strcmp(argv[2], "R") == 0 || strcmp(argv[2], "r") == 0)
					mouse.Opt_Click(XJ_Mouse::ButtonType::Right);
				else
					printf_s("【参数只能为L/M/R之一，对应左/中/右键】\n");
			}
			else {
				printf_s("【输入参数不足，需要指定鼠标点击键。参数只能为L/M/R之一，对应左/中/右键】\n");
			}
			return 0;
		}	
		else if (strcmp(argv[1], "/W") == 0 || strcmp(argv[1], "/w") == 0) {
			XJ_Mouse mouse;
			if (argc > 2) {//鼠标滚轮滚动
				try {
					int delta=atoi(argv[2]);
					mouse.Opt_Wheel(delta);
				}
				catch (...) {
					printf_s("【输入参数有误，只能为整数】\n");
				}
			}
			else {
				printf_s("【输入参数不足，需要指定滚轮滚动值】\n");
			}
			return 0;
		}
	}
	const char* processName = strrchr(argv[0], '\\');
	if (processName)
		processName += 1;
	else
		processName = argv[0];

	printf_s("\n");
	printf_s("本程序用于键鼠操作：%s [/M] [/K] [/C] [/W] [msg]\n", processName);
	printf_s("/M：如果没有[msg]那么将返回当前鼠标位置；如果有[msg]那么将鼠标位置设置为指定坐标([msg]格式为“xxx,yyy”)\n");
	printf_s("/K：键入数据，数据内容为[msg]，无[msg]则键入失败\n");
	printf_s("/C：鼠标完成一次点击操作，[msg]只能为“L”、“M”、“R”，对应左键、中键、右键\n");
	printf_s("/W：鼠标滚轮滚动，[msg]只能为整数，正数向下滚动，负数向上滚动\n");
	printf_s("\n");
	printf_s("使用示例：\n");
	printf_s("%-30s：%s /M 1000,500\n", "·移动鼠标到(1000,500)位置", processName);
	printf_s("%-30s：%s /M\n", "·获取鼠标位置", processName);
	printf_s("%-30s：%s /K ABCDE\n", "·键入数据“ABCDE”", processName);
	printf_s("%-30s：%s /C L\n", "·左键单击", processName);
	printf_s("%-30s：%s /W 120\n", "·滚轮向下滚动，移动量为120", processName);
	return 0;
}

