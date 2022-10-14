/*
格式：printf("\033[显示方式;字背景颜色;字体颜色m 字符串 \033[0m \n" );

颜色分为背景色和字体色，30~39用来设置字体色，40~49设置背景：
        背景色                        字体色
        40: 黑                        30: 黑
        41: 红                        31: 红
        42: 绿                        32: 绿
        43: 黄                        33: 黄
        44: 蓝                        34: 蓝
        45: 紫                        35: 紫
        46: 深绿                      36: 深绿
        47: 白色                      37: 白色 

\033[0m   关闭所有属性   
\033[1m   设置高亮度   
\033[4m   下划线   
\033[5m   闪烁   
\033[7m   反显   
\033[8m   消隐   
\033[30m   --   \033[37m   设置前景色   
\033[40m   --   \033[47m   设置背景色   
\033[nA   光标上移n行   
\033[nB   光标下移n行   
\033[nC   光标右移n行   
\033[nD   光标左移n行   
\033[y;xH设置光标位置   
\033[2J   清屏   
\033[K   清除从光标到行尾的内容   
\033[s   保存光标位置   
\033[u   恢复光标位置   
\033[?25l   隐藏光标   
\033[?25h   显示光标
*/
#ifndef _COLOR_H_
#define _COLOR_H_

#define NONE                 "\033[0m"          //恢复颜色

//字体
#define BLACK                "\033[0;30m"		//浅黑
#define L_BLACK              "\033[1;30m"		//深黑
#define RED                  "\033[0;31m"		//浅红
#define L_RED                "\033[1;31m"       //深红
#define GREEN                "\033[0;32m"		//浅绿
#define L_GREEN              "\033[1;32m"       //深绿
#define BROWN                "\033[0;33m"		//浅黄
#define YELLOW               "\033[1;33m"       //深黄
#define BLUE                 "\033[0;34m"		//浅蓝
#define L_BLUE               "\033[1;34m"       //深蓝
#define PURPLE               "\033[0;35m"		//浅紫
#define L_PURPLE             "\033[1;35m"       //深紫
#define CYAN                 "\033[0;36m"		//浅蓝绿
#define L_CYAN               "\033[1;36m"       //深蓝绿
#define GRAY                 "\033[0;37m"		//浅白
#define WHITE                "\033[1;37m"       //深白
                               
#define BOLD                 "\033[1m"			//加粗
#define UNDERLINE            "\033[4m"			//下划线
#define BLINK                "\033[5m"			//闪烁
#define REVERSE              "\033[7m"			//反选
#define HIDE                 "\033[8m"			//消隐
#define CLEAR                "\033[2J"			//清屏

//字体+背景
#define RED_G_GREEN			 "\033[0;42;31m"	//红-绿
#define BLUE_G_PURPLE		 "\033[0;44;35m"	//蓝-紫
#define WHITE_G_RED			 "\033[0;41;37m"	//白-红
#define PURPLE_G_WHITE		 "\033[0;47;35m"	//紫-白

#endif

/*
	示例：
	printf("[%2u]" BLACK "BLACK " L_BLACK "L_BLACK\n" NONE, __LINE__);
	printf("[%2u]" RED "RED " L_RED "L_RED\n" NONE, __LINE__);
	printf("[%2u]" GREEN "GREEN " L_GREEN "L_GREEN\n" NONE, __LINE__);
	printf("[%2u]" BROWN "BROWN " YELLOW "YELLOW\n" NONE, __LINE__);
	printf("[%2u]" BLUE "BLUE " L_BLUE "L_BLUE\n" NONE, __LINE__);
	printf("[%2u]" PURPLE "PURPLE " L_PURPLE "L_PURPLE\n" NONE, __LINE__);
	printf("[%2u]" CYAN "CYAN " L_CYAN "L_CYAN\n" NONE, __LINE__);
	printf("[%2u]" GRAY "GRAY " WHITE "WHITE\n" NONE, __LINE__);

	//设置背景颜色
	printf("[%2u]\033[1;31;40m Red \033[0m\n",  __LINE__);
	printf("[%2u]" RED_G_GREEN "Red" NONE "\n",  __LINE__);
	
	printf("[%2u]" BOLD "BOLD\n" NONE, __LINE__);
	printf("[%2u]" UNDERLINE "UNDERLINE\n" NONE, __LINE__);
	printf("[%2u]" BLINK "BLINK\n" NONE, __LINE__);
	printf("[%2u]" REVERSE "REVERSE\n" NONE, __LINE__);
	printf("[%2u]" HIDE "HIDE\n" NONE, __LINE__);
*/