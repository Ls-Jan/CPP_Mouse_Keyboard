

#include"XJ_Mouse.h"
#include"XJ_Keyboard.h"

#include<stdio.h>
#include <locale.h>

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "");//���ô���ҳ�����������룺https://learn.microsoft.com/zh-cn/cpp/c-runtime-library/code-pages?view=msvc-170
	if (argc > 1) {//argv��һ�������ǳ���·��������		
		if (strcmp(argv[1], "/M") == 0 || strcmp(argv[1], "/m") == 0) {
			XJ_Mouse mouse;
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
			return 0;
		}
		else if (strcmp(argv[1], "/K") == 0 || strcmp(argv[1], "/k") == 0) {
			XJ_Keyboard keyboard;
			if (argc > 2) {//�����ı�
				keyboard.Opt_SendStr(argv[2]);
			}
			else {
				printf_s("������������㣬ȱ�١����͡����ݡ�\n");
			}
			return 0;
		}
		else if (strcmp(argv[1], "/C") == 0 || strcmp(argv[1], "/c") == 0) {
			XJ_Mouse mouse;
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
			return 0;
		}	
		else if (strcmp(argv[1], "/W") == 0 || strcmp(argv[1], "/w") == 0) {
			XJ_Mouse mouse;
			if (argc > 2) {//�����ֹ���
				try {
					int delta=atoi(argv[2]);
					mouse.Opt_Wheel(delta);
				}
				catch (...) {
					printf_s("�������������ֻ��Ϊ������\n");
				}
			}
			else {
				printf_s("������������㣬��Ҫָ�����ֹ���ֵ��\n");
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
	printf_s("���������ڼ��������%s [/M] [/K] [/C] [/W] [msg]\n", processName);
	printf_s("/M�����û��[msg]��ô�����ص�ǰ���λ�ã������[msg]��ô�����λ������Ϊָ������([msg]��ʽΪ��xxx,yyy��)\n");
	printf_s("/K���������ݣ���������Ϊ[msg]����[msg]�����ʧ��\n");
	printf_s("/C��������һ�ε��������[msg]ֻ��Ϊ��L������M������R������Ӧ������м����Ҽ�\n");
	printf_s("/W�������ֹ�����[msg]ֻ��Ϊ�������������¹������������Ϲ���\n");
	printf_s("\n");
	printf_s("ʹ��ʾ����\n");
	printf_s("%-30s��%s /M 1000,500\n", "���ƶ���굽(1000,500)λ��", processName);
	printf_s("%-30s��%s /M\n", "����ȡ���λ��", processName);
	printf_s("%-30s��%s /K ABCDE\n", "���������ݡ�ABCDE��", processName);
	printf_s("%-30s��%s /C L\n", "���������", processName);
	printf_s("%-30s��%s /W 120\n", "���������¹������ƶ���Ϊ120", processName);
	return 0;
}

