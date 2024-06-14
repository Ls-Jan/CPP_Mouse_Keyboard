

#include"XJ_Mouse.h"
#include"XJ_Keyboard.h"

#include<stdio.h>
#include <locale.h>


#include<map>
#include<vector>
#include<string>


std::vector<std::string> Scan(const char* text) {//把text的内容逐字符拆散，其中方括号[]包着的是一个整体
	std::vector<std::string> lst;
	std::string word;
	bool seg = true;
	for (auto ptr = text; *ptr; ++ptr) {
		switch (*ptr) {
		case '[':
			if (!word.empty())
				throw 0;
			seg = false;
			break;
		case ']':
			if (word.empty())
				throw 0;
			seg = true;
			break;
		}
		word.push_back(*ptr);
		if (seg) {
			if (word.front() == '[' && word.back() == ']')
				word = word.substr(1, word.length() - 2);
			lst.push_back(word);
			word.clear();
		}
	}
	return lst;
}

std::string CapitalTranslate(std::string tx, int(*trans)(int)) {//全转大写或小写，仅对字母有效
	for (auto p = tx.begin(); p != tx.end(); ++p)
		*p = trans(*p);
	return tx;
}

static const std::map<std::string, int>VirtualKeys{
	{"ESC",VK_ESCAPE},
	{"BACK",VK_BACK},
	{"ENTER",VK_RETURN},
	{"WIN",VK_LWIN},
	{"TAB",VK_TAB},
	{"DELETE",VK_DELETE},
	{"CAPITAL",VK_CAPITAL},
	{"SHIFT",VK_SHIFT},
	{"CTRL",VK_CONTROL},
	{"ALT",VK_MENU},
	{"HOME",VK_HOME},
	{"PAUSE",VK_PAUSE},
	{"LEFT",VK_LEFT},
	{"RIGHT",VK_RIGHT},
	{"SPACE",VK_SPACE},
	{"UP",VK_UP},
	{"DOWN",VK_DOWN},
	{"F1",VK_F1},
	{"F2",VK_F2},
	{"F3",VK_F3},
	{"F4",VK_F4},
	{"F5",VK_F5},
	{"F6",VK_F6},
	{"F7",VK_F7},
	{"F8",VK_F8},
	{"F9",VK_F9},
	{"F10",VK_F10},
	{"F11",VK_F11},
	{"F12",VK_F12},
};

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "");//设置代码页，防中文乱码：https://learn.microsoft.com/zh-cn/cpp/c-runtime-library/code-pages?view=msvc-170
	XJ_Keyboard keyboard;
	XJ_Mouse mouse;
	if (argc > 1) {//argv第一个往往是程序路径，忽视
		std::string option = CapitalTranslate(argv[1], std::toupper);
		if (option == "/M") {
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
		}
		else if (option == "/C") {
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
		}
		else if (option == "/W") {
			if (argc > 2) {//发送文本
				keyboard.Opt_SendStr(argv[2]);
			}
			else {
				printf_s("【输入参数不足，缺少指定符号】\n");
			}
		}
		else if (option == "/K") {
			if (argc > 2) {//发送文本
				keyboard.Opt_SendStr(argv[2]);
			}
			else {
				printf_s("【输入参数不足，缺少“键入”内容】\n");
			}
		}
		else if (option == "/KS" || option == "/KP" || option == "/KR") {
			void (*Func)(XJ_Keyboard & keyboard, WORD key, BOOL isVirtual) = nullptr;
			if (option == "/KS")
				Func = [](XJ_Keyboard& keyboard, WORD key, BOOL isVirtual) {keyboard.Opt_SendKey(key, isVirtual); };
			else if (option == "/KP")
				Func = [](XJ_Keyboard& keyboard, WORD key, BOOL isVirtual) {keyboard.Opt_PressKey(key); };
			else if (option == "/KR")
				Func = [](XJ_Keyboard& keyboard, WORD key, BOOL isVirtual) {keyboard.Opt_ReleaseKey(key); };
			if (argc > 2) {
				auto lst = Scan(argv[2]);
				for (auto word = lst.begin(); word != lst.end(); ++word) {
					WORD key = NULL;
					BOOL isVirtualKey = FALSE;
					if (word->length() > 1) {
						std::string word_Cap = CapitalTranslate(*word, std::toupper);
						auto vk = VirtualKeys.find(word_Cap);
						if (vk == VirtualKeys.end()) {
							printf_s("【虚拟键有误：[%s]】", word->data());
							break;
						}
						key = vk->second;
						isVirtualKey = TRUE;
					}
					else
						key = word->data()[0];
					Func(keyboard, key, isVirtualKey);
				}
			}
			else {
				printf_s("【输入参数不足，缺少指定内容】\n");
			}
		}
		else if (option == "-VK") {
			printf_s("\n");
			for (auto p = VirtualKeys.begin(); p != VirtualKeys.end(); ++p) {
				printf_s("[%s]\n", p->first.data());
			}
			printf_s("\n");
		}
		else {
			printf_s("【参数类型有误】\n");
		}
	}
	else {
		const char* processName = strrchr(argv[0], '\\');
		if (processName)
			processName += 1;
		else
			processName = argv[0];

		printf_s("\n");
		printf_s("本程序用于键鼠控制：%s [/M] [/K] [/C] [/W] [/Ks] [/Kp] [/Kr] [-vk] [msg]\n", processName);
		printf_s("程序运行时只能选择以下其中一个选项\n");
		printf_s("/M：如果没有[msg]那么将返回当前鼠标位置；如果有[msg]那么将鼠标位置设置为指定坐标([msg]格式为“xxx,yyy”)\n");
		printf_s("/C：鼠标完成一次点击操作，[msg]只能为“L”、“M”、“R”，对应左键、中键、右键\n");
		printf_s("/W：鼠标滚轮滚动，[msg]只能为整数，正数向下滚动，负数向上滚动\n");
		printf_s("/K：键入文本数据，数据内容为[msg]\n");
		printf_s("/Ks：模拟键盘逐个按键进行键入，以弥补命令行的不足。例如[msg]为“[ENTER]”则键入回车。\n");
		printf_s("        /Ks选项是“模拟键盘键入”，故[msg]中不能出现英文字符外的其他符号\n");
		printf_s("/Kp：按下指定按键，以弥补命令行的不足。例如[msg]为“[SHIFT]”则按下shift键\n");
		printf_s("/Kr：松开某个按键，以弥补命令行的不足。例如[msg]为“[CTRL]”则松开ctrl键\n");
		printf_s("-vk：打印特殊按键对应名称\n");

		printf_s("\n");
		printf_s("使用示例：\n");
		printf_s("%-30s：%s /M 1000,500\n", "·移动鼠标到(1000,500)位置", processName);
		printf_s("%-30s：%s /M\n", "·获取鼠标位置", processName);
		printf_s("%-30s：%s /K ABC一二三\n", "·键入数据“ABC一二三”", processName);
		printf_s("%-30s：%s /C L\n", "·左键单击", processName);
		printf_s("%-30s：%s /W 120\n", "·滚轮向下滚动，移动量为120", processName);
		printf_s("%-30s：%s /Kp [SHIFT]\n", "·按下shift键", processName);
		printf_s("%-30s：%s /Kr [ALT]\n", "·松开alt键", processName);
		printf_s("%s\n", "·依次按下TAB键、按键ABCDE#、");
		printf_s("               %-15s：%s /Ks [TAB][SPACE]ABCDE#[BACK][ENTER]\n", "退格键、回车键", processName);
		printf_s("%-30s：%s -vk\n", "·打印特殊按键对应名称", processName);
	}
	return 0;
}






//估计是当时尝试“驱动版控制键鼠”但搞了一会儿就给忘到九霄外，然后留下这坨稀烂测试代码
//我记得下面的方式也做不到“驱动级调用”，搞一会儿就没耐心搞下去了
//VS设置管理员调试：https://blog.csdn.net/zeqi1991/article/details/96195127

int main1() {


	Sleep(3000);
	XJ_Keyboard keyboard;
	//keyboard.Opt_SendKey(VK_BACK, TRUE);
	//keyboard.Opt_SendKey(VK_CANCEL, TRUE);
	//return 0;

	//keyboard.Opt_PressKey('A');
	keyboard.Opt_PressKey(VK_SHIFT);
	keyboard.Opt_PressKey(VK_CONTROL);
	keyboard.Opt_PressKey('K');



	return 0;
	//std::string str = "ABCDE<KEY>";
	auto rst = Scan("ABCDE<ENTEr>");
	for (auto word = rst.begin(); word != rst.end(); ++word) {
		if (word->length() > 1) {
			std::string word_Cap = CapitalTranslate(*word, std::toupper);
			auto vk = VirtualKeys.find(word_Cap);
			if (vk == VirtualKeys.end()) {
				printf_s("【虚拟键有误：<%s>】", word->data());
				return 1;
			}
			keyboard.Opt_SendKey(vk->second, TRUE);
		}
		else {
			keyboard.Opt_SendKey(*word->data());
		}

	}
	return 0;
}

#include"Interception/library/interception.h"
#pragma comment(lib,"Interception/library/x86/interception.lib")
#include<windows.h>
enum ScanCode
{
	SCANCODE_X = 0x2D,
	SCANCODE_Y = 0x15,
	SCANCODE_ESC = 0x01
};
#include<winioctl.h>

int main2() {
	InterceptionContext context;
	InterceptionDevice device;
	InterceptionKeyStroke stroke;

	//raise_process_priority();

	context = interception_create_context();

	//interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN | INTERCEPTION_FILTER_KEY_UP);

	

	Sleep(2000);

	InterceptionMouseStroke mouseStroke[3];
	ZeroMemory(mouseStroke, sizeof(mouseStroke));
	// 鼠标移动到屏幕中间
	//mouseStroke[0].flags = INTERCEPTION_MOUSE_MOVE_RELATIVE;
	//mouseStroke[0].flags = INTERCEPTION_MOUSE_MOVE_ABSOLUTE;
	//mouseStroke[0].x = 65535 / 2; // 坐标取值范围是0-65535
	//mouseStroke[0].y = 65535 / 2;
	//mouseStroke[0].rolling=300;
	// 点击鼠标右键
	//mouseStroke[1].state = INTERCEPTION_MOUSE_RIGHT_BUTTON_DOWN;
	//mouseStroke[2].state = INTERCEPTION_MOUSE_RIGHT_BUTTON_UP;
	mouseStroke[0].state = INTERCEPTION_MOUSE_LEFT_BUTTON_DOWN;
	mouseStroke[1].state = INTERCEPTION_MOUSE_LEFT_BUTTON_UP;
	//mouseStroke[1].state = INTERCEPTION_MOUSE_WHEEL;
	//mouseStroke[0].state = INTERCEPTION_MOUSE_WHEEL;

	//mouseStroke[2].state = INTERCEPTION_MOUSE_RIGHT_BUTTON_UP;
	interception_send(context, INTERCEPTION_MOUSE(0), (InterceptionStroke*)mouseStroke, 2);
	//interception_send(context, INTERCEPTION_MOUSE(0), (InterceptionStroke*)mouseStroke, _countof(mouseStroke));


	////InterceptionContext context2 = interception_create_context();
	//InterceptionKeyStroke keyStroke[2];
	//ZeroMemory(keyStroke, sizeof(keyStroke));
	//
	////keyStroke[0].code = 'K';//左
	////keyStroke[0].code = VK_DELETE;//c
	////keyStroke[0].code = MapVirtualKey(VK_DELETE, MAPVK_VSC_TO_VK);//F9
	////keyStroke[0].code = MapVirtualKey('K', MAPVK_VK_TO_CHAR);//左
	////keyStroke[0].code = MapVirtualKey('k', MAPVK_VK_TO_CHAR);//\\反斜杠
	////keyStroke[0].code = MapVirtualKey(VK_TAB, MAPVK_VK_TO_VSC);
	////keyStroke[0].code = MapVirtualKey('K', MAPVK_VK_TO_VSC);
	//keyStroke[0].code = MapVirtualKey('A', MAPVK_VK_TO_VSC);
	////keyStroke[0].code = MapVirtualKey(VK_DELETE, MAPVK_VK_TO_VSC);
	////keyStroke[0].code = MapVirtualKey(VK_DELETE, MAPVK_VK_TO_VSC_EX);
	////keyStroke[0].code = MapVirtualKey('K', MAPVK_VSC_TO_VK);
	////keyStroke[0].code = MapVirtualKey('K', MAPVK_VSC_TO_VK_EX);
	//keyStroke[0].state = INTERCEPTION_KEY_DOWN;
	//keyStroke[1].code = keyStroke[0].code;
	//keyStroke[1].state = INTERCEPTION_KEY_UP;
	//interception_send(context, INTERCEPTION_KEYBOARD(0), (InterceptionStroke*)keyStroke, _countof(keyStroke));


	//device = interception_wait(context);
	//stroke.code = SCANCODE_Y;
	//stroke.state= INTERCEPTION_KEY_DOWN;
	////stroke.state= INTERCEPTION_KEY_UP;
	//interception_send(context, INTERCEPTION_KEYBOARD(0), (InterceptionStroke*)&stroke, 1);
	//stroke.state= INTERCEPTION_KEY_UP;
	//interception_send(context, INTERCEPTION_KEYBOARD(0), (InterceptionStroke*)&stroke, 1);
	//interception_send(context, device, (InterceptionStroke*)&stroke, 1);


	////while (interception_receive(context, device = interception_wait(context), (InterceptionStroke*)&stroke, 1) > 0)
	////{
	////	if (stroke.code == SCANCODE_X) stroke.code = SCANCODE_Y;

	////	interception_send(context, device, (InterceptionStroke*)&stroke, 1);

	////	if (stroke.code == SCANCODE_ESC) break;
	////}

	interception_destroy_context(context);

	return 0;
}



