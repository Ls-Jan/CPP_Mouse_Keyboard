

#include"XJ_Mouse.h"
#include"XJ_Keyboard.h"

#include<stdio.h>
#include <locale.h>


#include<map>
#include<vector>
#include<string>


std::vector<std::string> Scan(const char* text) {//��text���������ַ���ɢ�����з�����[]���ŵ���һ������
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

std::string CapitalTranslate(std::string tx, int(*trans)(int)) {//ȫת��д��Сд��������ĸ��Ч
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
	setlocale(LC_ALL, "");//���ô���ҳ�����������룺https://learn.microsoft.com/zh-cn/cpp/c-runtime-library/code-pages?view=msvc-170
	XJ_Keyboard keyboard;
	XJ_Mouse mouse;
	if (argc > 1) {//argv��һ�������ǳ���·��������
		std::string option = CapitalTranslate(argv[1], std::toupper);
		if (option == "/M") {
			if (argc == 2) {//�������λ��
				POINT pos = mouse.Get_Position();
				printf_s("%d,%d", pos.x, pos.y);
			}
			else {//�������λ��
				try {
					char* data = argv[2];
					char* back = strchr(data, ',');
					if (back && (strlen(back) > 1)) {
						back += 1;//��������
						char front[128];
						int len = back - data - 1;
						memset(front, 0, 128);
						strncpy_s(front, data, min(len, 127));//��ȡ
						int posX = atoi(front);
						int posY = atoi(back);
						mouse.Set_Position(posX, posY);
					}
					else {
						throw - 1;
					}
				}
				catch (...) {
					printf_s("������Ĳ�������(��ʽ��xxx,yyy)�������º˶ԣ���\n");
				}
			}
		}
		else if (option == "/C") {
			if (argc > 2) {//�����
				if (strcmp(argv[2], "L") == 0 || strcmp(argv[2], "l") == 0)
					mouse.Opt_Click(XJ_Mouse::ButtonType::Left);
				else if (strcmp(argv[2], "M") == 0 || strcmp(argv[2], "m") == 0)
					mouse.Opt_Click(XJ_Mouse::ButtonType::Middle);
				else if (strcmp(argv[2], "R") == 0 || strcmp(argv[2], "r") == 0)
					mouse.Opt_Click(XJ_Mouse::ButtonType::Right);
				else
					printf_s("������ֻ��ΪL/M/R֮һ����Ӧ��/��/�Ҽ���\n");
			}
			else {
				printf_s("������������㣬��Ҫָ���������������ֻ��ΪL/M/R֮һ����Ӧ��/��/�Ҽ���\n");
			}
		}
		else if (option == "/W") {
			if (argc > 2) {//�����ı�
				keyboard.Opt_SendStr(argv[2]);
			}
			else {
				printf_s("������������㣬ȱ��ָ�����š�\n");
			}
		}
		else if (option == "/K") {
			if (argc > 2) {//�����ı�
				keyboard.Opt_SendStr(argv[2]);
			}
			else {
				printf_s("������������㣬ȱ�١����롱���ݡ�\n");
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
							printf_s("�����������[%s]��", word->data());
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
				printf_s("������������㣬ȱ��ָ�����ݡ�\n");
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
			printf_s("��������������\n");
		}
	}
	else {
		const char* processName = strrchr(argv[0], '\\');
		if (processName)
			processName += 1;
		else
			processName = argv[0];

		printf_s("\n");
		printf_s("���������ڼ�����ƣ�%s [/M] [/K] [/C] [/W] [/Ks] [/Kp] [/Kr] [-vk] [msg]\n", processName);
		printf_s("��������ʱֻ��ѡ����������һ��ѡ��\n");
		printf_s("/M�����û��[msg]��ô�����ص�ǰ���λ�ã������[msg]��ô�����λ������Ϊָ������([msg]��ʽΪ��xxx,yyy��)\n");
		printf_s("/C��������һ�ε��������[msg]ֻ��Ϊ��L������M������R������Ӧ������м����Ҽ�\n");
		printf_s("/W�������ֹ�����[msg]ֻ��Ϊ�������������¹������������Ϲ���\n");
		printf_s("/K�������ı����ݣ���������Ϊ[msg]\n");
		printf_s("/Ks��ģ���������������м��룬���ֲ������еĲ��㡣����[msg]Ϊ��[ENTER]�������س���\n");
		printf_s("        /Ksѡ���ǡ�ģ����̼��롱����[msg]�в��ܳ���Ӣ���ַ������������\n");
		printf_s("/Kp������ָ�����������ֲ������еĲ��㡣����[msg]Ϊ��[SHIFT]������shift��\n");
		printf_s("/Kr���ɿ�ĳ�����������ֲ������еĲ��㡣����[msg]Ϊ��[CTRL]�����ɿ�ctrl��\n");
		printf_s("-vk����ӡ���ⰴ����Ӧ����\n");

		printf_s("\n");
		printf_s("ʹ��ʾ����\n");
		printf_s("%-30s��%s /M 1000,500\n", "���ƶ���굽(1000,500)λ��", processName);
		printf_s("%-30s��%s /M\n", "����ȡ���λ��", processName);
		printf_s("%-30s��%s /K ABCһ����\n", "���������ݡ�ABCһ������", processName);
		printf_s("%-30s��%s /C L\n", "���������", processName);
		printf_s("%-30s��%s /W 120\n", "���������¹������ƶ���Ϊ120", processName);
		printf_s("%-30s��%s /Kp [SHIFT]\n", "������shift��", processName);
		printf_s("%-30s��%s /Kr [ALT]\n", "���ɿ�alt��", processName);
		printf_s("%s\n", "�����ΰ���TAB��������ABCDE#��");
		printf_s("               %-15s��%s /Ks [TAB][SPACE]ABCDE#[BACK][ENTER]\n", "�˸�����س���", processName);
		printf_s("%-30s��%s -vk\n", "����ӡ���ⰴ����Ӧ����", processName);
	}
	return 0;
}






//�����ǵ�ʱ���ԡ���������Ƽ��󡱵�����һ����͸����������⣬Ȼ����������ϡ�ò��Դ���
//�Ҽǵ�����ķ�ʽҲ�����������������á�����һ�����û���ĸ���ȥ��
//VS���ù���Ա���ԣ�https://blog.csdn.net/zeqi1991/article/details/96195127

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
				printf_s("�����������<%s>��", word->data());
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
	// ����ƶ�����Ļ�м�
	//mouseStroke[0].flags = INTERCEPTION_MOUSE_MOVE_RELATIVE;
	//mouseStroke[0].flags = INTERCEPTION_MOUSE_MOVE_ABSOLUTE;
	//mouseStroke[0].x = 65535 / 2; // ����ȡֵ��Χ��0-65535
	//mouseStroke[0].y = 65535 / 2;
	//mouseStroke[0].rolling=300;
	// �������Ҽ�
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
	////keyStroke[0].code = 'K';//��
	////keyStroke[0].code = VK_DELETE;//c
	////keyStroke[0].code = MapVirtualKey(VK_DELETE, MAPVK_VSC_TO_VK);//F9
	////keyStroke[0].code = MapVirtualKey('K', MAPVK_VK_TO_CHAR);//��
	////keyStroke[0].code = MapVirtualKey('k', MAPVK_VK_TO_CHAR);//\\��б��
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



