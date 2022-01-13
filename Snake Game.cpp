#include"stdio.h"
#include"conio.h"	//用到了_kbhit();函数来判定有无按键按下。_getch();函数来获取键入ASCII码。
#include"stdlib.h"	//用到了srand();rand():函数，这两个函数联合使用实现产生在规定范围内的随机数。system("cls")函数实现清屏。exit()函数实现结束程序。
#include"windows.h"	//用到了Sleep函数，来决定延时的长度。其实也可以用for循环实现延时的功能，但是鉴于for循环控制延时不精准且结构繁杂，所以使用Sleep函数。
#include"time.h"	//由于以当前时间为参数提供种子供rand()函数生成更为随机的数，用到了time(NULL)函数。
FILE*fp, stream;
#define w 119		//按键的ASCII码，也可以改成小键盘的方向键，但是用方向键的时候要用两个getch()函数，因为按下一次方向键会产生两个数值。
#define s 115
#define a 97
#define d 100
#define SNAKE 1
#define FOOD 2
#define BAR 3
int direction = w;					//默认方向为上
int N = 0;							//定义全局变量N来改变刷新速度，从而在视觉上改变游戏速度
char map[17][17] = { 0 };		    //unsigned char类型数据8位长度，高4位作为列坐标（x），低四位作为行坐标（y），所以x的范围是0-15.所以地图的边长等于15+2=17（加两个边界长度）
unsigned char snake[80] = { 136 };	//蛇头是snake[0]=62,初始值为62，八位二进制码为1000 1000B，列坐标x为8，行坐标y为8
unsigned char food = 200;			//定义食物的初始坐标。初始值为200，八位二进制码为1100 1000B， 列坐标x为12，行坐标y为8
char len = 1;						//初始化蛇长度为1
char top = 0;						//定义全局变量最高分
int plus = 0;
void transfer(unsigned char num, unsigned char *j, unsigned char *k);		//数值转化坐标的转换函数
void printout(void);				//实现打印，清屏功能的函数
int getdirection(int olddirection);	//接收按键反馈值的函数
void movement(int direction);		//描述蛇移动规律的函数，该游戏遵守的核心规律。
unsigned char offerfood(void);		//随机产生规定范围内的食物函数
int gameover(void);					//游戏是否结束的判断函数
void loadgame(void);				//游戏读取读取函数
void savegame(void);				//游戏数据存储函数
void menuprint(void);				//打印游戏开始界面
void speedchoose(void);				//选择游戏速度函数
void loadtop(void);
void savetop(void);
void menuoption(void);
int gamemode(void);
void crazymovement(int direction);
int main(void)
{
	menuprint();
	menuoption();
	switch (gamemode())
	{
		case 1:
			speedchoose();
			while (1)						//假如没有吃到自己或碰壁，游戏是无限循环的
			{
				direction = getdirection(direction);		//获取按下的方向键
				movement(direction);		//蛇的运动规律函数，根据蛇的运动，更新游戏坐标
				printout();					//打印游戏内容，包括墙壁，蛇身，空白，食物
				if (gameover() == 1)			//判断是否结束游戏
				{
				break;
				}
			}
			break;
		case 0:
			N=200;
			while (1)						//假如没有吃到自己或碰壁，游戏是无限循环的
			{
				direction = getdirection(direction);		//获取按下的方向键
				crazymovement(direction);		//蛇的运动规律函数，根据蛇的运动，更新游戏坐标
				printout();					//打印游戏内容，包括墙壁，蛇身，空白，食物
				if (gameover() == 1)			//判断是否结束游戏
				{
					break;
				}
			}
			break;

	}
	loadtop();
	printf("游戏结束\n");
	printf("你的游戏得分为：%d\n", len - 1);
	if(len-1>top)
	{	
		top = len - 1;
		savetop();
		printf("恭喜你创造了新纪录！！\n");
	}
	else { printf("历史最高分为：%d\n", top); }
	printf("按任意键退出游戏\n");
	_getch();
	return 0;
}
void transfer(unsigned char num, unsigned char *j, unsigned char *k)   //j代表列坐标x，k代表行坐标y。这里利用位移补零的运算法则，8位二进制数的高四位转换为x轴坐标，低四位转换为y轴坐标
{											//数值指针实现多个返回值。这里不用全局变量是因为实现多个返回值，是因为程序会出错。。。我试过了。。。
	*j = num >> 4;							//右移四位，高位补0，实现将8位二进制高四位赋值给*j
	*k = num&15;							//通过0000 1111B与逻辑运算实现低四位赋值给*k
}
int gameover(void)    //游戏结束判定函数，当蛇撞墙或者咬到自己时，返回值为1
{
	int selfeat ;
	selfeat = 0;
	unsigned char x, y;
	transfer(snake[0], &x, &y);
	for (int i = 1; i < len; i++) 
	{
		if (snake[0] == snake[i]) 
		{
			selfeat = 1;
			return 1;
		}
	}
	if (x == 0 || x == 16 || y == 0 || y == 16 ) return 1;
	else return 0;
}
unsigned char offerfood(void)
{
	unsigned char randomfood;
	unsigned char fx, fy;
	int insnake ;
	srand((unsigned int)time(NULL));      //以当前时间为参数提供种子供rand()函数生成更为随机的数
	do 
	{
		insnake = 0;
		randomfood = rand() % 255;        //取值范围为0-255，即2^8-1,即8位二进制的取值范围，与unsigned char类型相同
		transfer(randomfood, &fx, &fy);   //将随机值转化为列坐标x，行坐标y
			for (int i = 0; i < len; i++)
			{
				if (randomfood == snake[i])//如果出现在蛇身上
				{
					insnake = 1;
				}
			}
	} while (fx == 0 || fx == 16 || fy == 0 || fy == 16 || insnake == 1);
	return randomfood;
}
void printout(void)     //j是行，i是列。也就是j是y轴，i是x轴。
{						//根据地图上每点的情况绘制游戏，逐行打印
	for (int j = 0; j < 17; j++) 
	{
		for (int i = 0; i < 17;i++) 
		{
			if (map[i][j] == SNAKE) { putchar('*'); }
			else if (map[i][j] == BAR) { putchar('x'); }
			else if (map[i][j] == FOOD) { putchar('o'); }
			else { putchar(' '); }
		}
		printf("\n");
	}
	Sleep(N);
	system("cls");
}
int getdirection(int olddirection)
{
	int newdirection = olddirection;
	if (_kbhit()!=0)			//用kbhit()与getch()组合实现键盘响应
	{							//kbhit() 检查当前是否有键盘输入，若有则返回一个非0值，否则返回0
		newdirection = _getch();//键入的字符对应的ASCII码赋值给变量
		if (newdirection == 113) 
		{
			 savegame(); 
			 printf("保存成功\n按下任意键退出游戏\n");
			_getch(); exit(0);
		}
		if (len > 1 && abs(newdirection - olddirection) == 3 || abs(newdirection - olddirection == 4))	//如果蛇身长度大于1，则不能回头
		{ newdirection = olddirection; }
		if (newdirection != 119 && newdirection !=115 &&  newdirection != 97 && newdirection != 100)		//控制方向的键以外的按键，方向保持
		{ newdirection = olddirection; }
	}
	else{ newdirection = olddirection; }//不按按键时，方向保持
	return newdirection;
}
void movement(int direction)
{
	unsigned char fx, fy;		//食物的列，行坐标
	unsigned char x, y;			//蛇的列，行坐标
	int last = snake[0];		//蛇头没更新前的坐标值赋值给last变量
	int current;				//last与current用于之后蛇身坐标的更新
	transfer(food, &fx, &fy);	//调用转换函数，表示食物坐标 
	transfer(snake[0], &x, &y);	//调用转换函数，表示蛇头坐标 
	switch (direction)			//根据运动方向来决定蛇头坐标的变化
	{
	case w:
		y--;					//上运动行坐标减1
		break;
	case s:
		y++;					//下运动行坐标加1
		break;
	case a:
		x--;					//左运动列坐标减1
		break;
	case d:
		x++;					//右运动列坐标加1
		break;
	}
	snake[0] = (x<<4)|y;		//将蛇头行列坐标值拼起来，相当于根据运动方向更新了蛇头的数值
	for (int i = 1; i < len; i++)
	{								//蛇身的更新从第二节开始，第二节等于蛇头的原来的位置值，第三节等于第二节原来的位置值，以此类推
		current = snake[i];			//将蛇这一节的值保存至current变量
		snake[i] = last;			//将蛇前一节的值赋值给这一节
		last = current;				//将蛇这一节的值作为last变量保存以便赋值给下一节							
	}								//最后，原来蛇尾的值留在了last
	if (snake[0] == food)			//更新之后的蛇头值与食物值相等
	{
		food = offerfood();			//调用offerfood随机产生食物，此时food被赋予新的值
		snake[len] = last;			//将原来蛇尾的坐标值赋值给第len+1节的蛇节（第len+1节成为蛇尾）
		len++;						//蛇长度len自增1
	}
	for (int j = 0; j < 17; j++)		//j对应行坐标，i对应列坐标，按行进行赋值
	{
		for (int i = 0; i < 17; i++)
		{
			if (i == 0 || i == 16 || j == 0 || j == 16){ map[i][j] = BAR; }
			else if (i == fx && j == fy){ map[i][j] = FOOD; }//这里的fx，fy是未更新的食物坐标，也就是说如果这个过程中蛇头虽然已经和食物坐标重合，食物坐标值立即被赋予新的随机值，但食物的列坐标和行坐标还未被刷新
			else{ map[i][j] = 0; }
		}
		for (int i = 0; i < len; i++)	//对蛇进行的赋值语句一定要在食物的赋值语句之后执行，才能保证更新后的蛇头与食物坐标相等时，蛇能覆盖食物，也就是蛇吃到食物此时屏幕上食物消失，下一次蛇运动后，食物的列坐标和行坐标才刷新
		{
			transfer(snake[i], &x, &y);
			map[x][y] = SNAKE; 
		}
	}
}
void savegame(void)	//按照顺序存储程序当前状态的数据，包括长度，方向，食物数值，蛇各节的数值
{
	FILE*fp;
	errno_t err;
	err= fopen_s(&fp, "D:\savedata.txt", "w");		//fopen函数编译不能通过
	if (err!=0) { printf("无法找到存档"); return; }	//文件不存在时，err返回值非0
	fwrite(&len, sizeof(len), 1, fp);
	fwrite(&direction, sizeof(direction), 1, fp);
	fwrite(&food, sizeof(food), 1, fp);
	for (int i = 0; i < len; i++)
	{
		fwrite(&snake[i], sizeof(unsigned char), 1, fp);
	}
	fclose(fp);		//结束数据流
}
void loadgame(void)//按照顺序读取文档中的长度，方向，食物数值，蛇各节数值的数据
{
	FILE*fp;
	errno_t err;
	err = fopen_s(&fp,"D:\savedata.txt", "r");
	if (err!=0) { printf("存档为空，创建新游戏\n"); return; }//文件不存在时，err返回值非0
	fread(&len, sizeof(len), 1, fp);
	fread(&direction, sizeof(direction), 1, fp);
	fread(&food, sizeof(food), 1, fp);
	for (int i = 0; i < len; i++)
	{
		fread(&snake[i], sizeof(unsigned char), 1, fp);
	}
	fclose(fp);
}
void menuprint(void)//游戏开始界面的打印
{
	printf("********************************\n");
	printf("**                            **\n");
	printf("**    欢迎进入贪吃蛇小游戏    **\n");
	printf("**                            **\n");
	printf("********************************\n\n");
	printf("开始菜单：\n\n");
	printf("  	<新的游戏>\n\n");
	printf("  	<载入游戏>\n\n");
	printf("  	<删除存档>\n\n");
	printf("  	<查看历史最高分>\n\n");
	printf("  	<清除最高分纪录>\n\n");
	printf("\n\n");
	printf("操作说明：\n");
	printf("    开始界面按键<r>	载入游戏。\n");
	printf("    开始界面按键<m>	删除存档，开始新的游戏。\n");
	printf("    开始界面按键<t>	查询历史最高分，开始新的游戏。\n");
	printf("    开始界面按键<c>	清除历史最高分，开始新的游戏。\n");
	printf("    开始界面其他任意按键，开始新的游戏。\n");
	printf("    游戏中按键<q>退出游戏，自动保存覆盖存档。\n");
	printf("    游戏中按键<w><s><a><d> 控制上下左右。\n");
}
void speedchoose(void)//游戏开始前游戏速度的选择，通过改变程序的延时时间来实现屏幕刷新的频率的变化
{
	printf("\n\n请选择游戏进行速度\n游戏即将开始......\n<1>慢速	<2>普通	<3>快速\n");
	int g;
	scanf_s("%d", &g);
	if (g == 1) { N = 500; }
	else if (g == 2) { N = 300; }
	else if (g == 3) { N = 100; }
	else { printf("请重新输入正确的编号\n"); exit(0); }
}
void loadtop(void)
{
	FILE*stream;
	errno_t err1;
	err1 = fopen_s(&stream, "D:/topmark.txt", "r");
	if (err1 != 0) { printf("当前暂无最高得分纪录\n"); return; }
	fread(&top, sizeof(top), 1, stream);
	fclose(stream);
}
void savetop(void)
{
	FILE*stream;
	errno_t err1;
	err1 = fopen_s(&stream, "D:/topmark.txt", "w");
	if (err1 != 0) { printf("无法找到或打开存档\n"); return; }
	fwrite(&top, sizeof(top), 1, stream);
	fclose(stream);
}
void menuoption(void)
{
	int option = _getch();			//是否读取游戏数据
	if (option == 116)
	{
		loadtop();
		printf("\n****************************\n");
		printf("**                        **\n");
		printf("**    历史最得高分： %d   **\n", top);
		printf("**                        **\n");
		printf("****************************\n\n");

	}
	if (option == 114) { loadgame(); }
	if (option == 99) { remove("D:/topmark.txt"); }
	if (option == 109) { remove("D:/savedata.txt"); }
}
int gamemode(void)
{	
	printf("\n选择游戏模式：\n");
	printf("####################################\n");
	printf("##               ##               ##\n");
	printf("##  <a>经典模式  ##  <b>疯狂模式  ##\n");
	printf("##               ##               ##\n");
	printf("####################################\n模式说明：疯狂模式下，游戏速度快，方向键相反，蛇的成长速率递增，游戏得分更多。\n	来挑战吧！\n");
	int k = getchar();
	if (k == 97) { return 1; }
	else if(k == 98) { return 0; }
	else { printf("请输入正确的数值"); exit(0); }
}
void crazymovement(int direction)
{
	unsigned char fx, fy;		//食物的列，行坐标
	unsigned char x, y;			//蛇的列，行坐标
	int last = snake[0];		//蛇头没更新前的坐标值赋值给last变量
	int current;				//last与current用于之后蛇身坐标的更新
	transfer(food, &fx, &fy);	//调用转换函数，表示食物坐标 
	transfer(snake[0], &x, &y);	//调用转换函数，表示蛇头坐标 
	switch (direction)			//根据运动方向来决定蛇头坐标的变化
	{
	case w:
		y=y+1;					
		break;
	case s:
		y=y-1;					
		break;
	case a:
		x=x+1;					
		break;
	case d:
		x=x-1;					
		break;
	}
	snake[0] = (x << 4) | y;		//将蛇头行列坐标值拼起来，相当于根据运动方向更新了蛇头的数值
	for (int i = 1; i < len; i++)
	{								//蛇身的更新从第二节开始，第二节等于蛇头的原来的位置值，第三节等于第二节原来的位置值，以此类推
		current = snake[i];			//将蛇这一节的值保存至current变量
		snake[i] = last;			//将蛇前一节的值赋值给这一节
		last = current;				//将蛇这一节的值作为last变量保存以便赋值给下一节							
	}								//最后，原来蛇尾的值留在了last
	if (snake[0] == food)			//更新之后的蛇头值与食物值相等
	{
		plus = plus + 1;
		food = offerfood();			//调用offerfood随机产生食物，此时food被赋予新的值
		for (int v = len;v<=len+plus;v++)
		{							//吃第一个食物长一节，吃第二个食物长两节，以此类推
			snake[v] = last;
		}
		len = len + plus;
	}
	for (int j = 0; j < 17; j++)		//j对应行坐标，i对应列坐标，按行进行赋值
	{
		for (int i = 0; i < 17; i++)
		{
			if (i == 0 || i == 16 || j == 0 || j == 16) { map[i][j] = BAR; }
			else if (i == fx && j == fy) { map[i][j] = FOOD; }//这里的fx，fy是未更新的食物坐标，也就是说如果这个过程中蛇头虽然已经和食物坐标重合，食物坐标值立即被赋予新的随机值，但食物的列坐标和行坐标还未被刷新
			else { map[i][j] = 0; }
		}
		for (int i = 0; i < len; i++)	//对蛇进行的赋值语句一定要在食物的赋值语句之后执行，才能保证更新后的蛇头与食物坐标相等时，蛇能覆盖食物，也就是蛇吃到食物此时屏幕上食物消失，下一次蛇运动后，食物的列坐标和行坐标才刷新
		{
			transfer(snake[i], &x, &y);
			map[x][y] = SNAKE;
		}
	}
}
