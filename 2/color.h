/*
��ʽ��printf("\033[��ʾ��ʽ;�ֱ�����ɫ;������ɫm �ַ��� \033[0m \n" );

��ɫ��Ϊ����ɫ������ɫ��30~39������������ɫ��40~49���ñ�����
        ����ɫ                        ����ɫ
        40: ��                        30: ��
        41: ��                        31: ��
        42: ��                        32: ��
        43: ��                        33: ��
        44: ��                        34: ��
        45: ��                        35: ��
        46: ����                      36: ����
        47: ��ɫ                      37: ��ɫ 

\033[0m   �ر���������   
\033[1m   ���ø�����   
\033[4m   �»���   
\033[5m   ��˸   
\033[7m   ����   
\033[8m   ����   
\033[30m   --   \033[37m   ����ǰ��ɫ   
\033[40m   --   \033[47m   ���ñ���ɫ   
\033[nA   �������n��   
\033[nB   �������n��   
\033[nC   �������n��   
\033[nD   �������n��   
\033[y;xH���ù��λ��   
\033[2J   ����   
\033[K   ����ӹ�굽��β������   
\033[s   ������λ��   
\033[u   �ָ����λ��   
\033[?25l   ���ع��   
\033[?25h   ��ʾ���
*/
#ifndef _COLOR_H_
#define _COLOR_H_

#define NONE                 "\033[0m"          //�ָ���ɫ

//����
#define BLACK                "\033[0;30m"		//ǳ��
#define L_BLACK              "\033[1;30m"		//���
#define RED                  "\033[0;31m"		//ǳ��
#define L_RED                "\033[1;31m"       //���
#define GREEN                "\033[0;32m"		//ǳ��
#define L_GREEN              "\033[1;32m"       //����
#define BROWN                "\033[0;33m"		//ǳ��
#define YELLOW               "\033[1;33m"       //���
#define BLUE                 "\033[0;34m"		//ǳ��
#define L_BLUE               "\033[1;34m"       //����
#define PURPLE               "\033[0;35m"		//ǳ��
#define L_PURPLE             "\033[1;35m"       //����
#define CYAN                 "\033[0;36m"		//ǳ����
#define L_CYAN               "\033[1;36m"       //������
#define GRAY                 "\033[0;37m"		//ǳ��
#define WHITE                "\033[1;37m"       //���
                               
#define BOLD                 "\033[1m"			//�Ӵ�
#define UNDERLINE            "\033[4m"			//�»���
#define BLINK                "\033[5m"			//��˸
#define REVERSE              "\033[7m"			//��ѡ
#define HIDE                 "\033[8m"			//����
#define CLEAR                "\033[2J"			//����

//����+����
#define RED_G_GREEN			 "\033[0;42;31m"	//��-��
#define BLUE_G_PURPLE		 "\033[0;44;35m"	//��-��
#define WHITE_G_RED			 "\033[0;41;37m"	//��-��
#define PURPLE_G_WHITE		 "\033[0;47;35m"	//��-��

#endif

/*
	ʾ����
	printf("[%2u]" BLACK "BLACK " L_BLACK "L_BLACK\n" NONE, __LINE__);
	printf("[%2u]" RED "RED " L_RED "L_RED\n" NONE, __LINE__);
	printf("[%2u]" GREEN "GREEN " L_GREEN "L_GREEN\n" NONE, __LINE__);
	printf("[%2u]" BROWN "BROWN " YELLOW "YELLOW\n" NONE, __LINE__);
	printf("[%2u]" BLUE "BLUE " L_BLUE "L_BLUE\n" NONE, __LINE__);
	printf("[%2u]" PURPLE "PURPLE " L_PURPLE "L_PURPLE\n" NONE, __LINE__);
	printf("[%2u]" CYAN "CYAN " L_CYAN "L_CYAN\n" NONE, __LINE__);
	printf("[%2u]" GRAY "GRAY " WHITE "WHITE\n" NONE, __LINE__);

	//���ñ�����ɫ
	printf("[%2u]\033[1;31;40m Red \033[0m\n",  __LINE__);
	printf("[%2u]" RED_G_GREEN "Red" NONE "\n",  __LINE__);
	
	printf("[%2u]" BOLD "BOLD\n" NONE, __LINE__);
	printf("[%2u]" UNDERLINE "UNDERLINE\n" NONE, __LINE__);
	printf("[%2u]" BLINK "BLINK\n" NONE, __LINE__);
	printf("[%2u]" REVERSE "REVERSE\n" NONE, __LINE__);
	printf("[%2u]" HIDE "HIDE\n" NONE, __LINE__);
*/