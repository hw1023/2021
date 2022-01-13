#include"stdio.h"
#include"conio.h"	//�õ���_kbhit();�������ж����ް������¡�_getch();��������ȡ����ASCII�롣
#include"stdlib.h"	//�õ���srand();rand():��������������������ʹ��ʵ�ֲ����ڹ涨��Χ�ڵ��������system("cls")����ʵ��������exit()����ʵ�ֽ�������
#include"windows.h"	//�õ���Sleep��������������ʱ�ĳ��ȡ���ʵҲ������forѭ��ʵ����ʱ�Ĺ��ܣ����Ǽ���forѭ��������ʱ����׼�ҽṹ���ӣ�����ʹ��Sleep������
#include"time.h"	//�����Ե�ǰʱ��Ϊ�����ṩ���ӹ�rand()�������ɸ�Ϊ����������õ���time(NULL)������
FILE*fp, stream;
#define w 119		//������ASCII�룬Ҳ���Ըĳ�С���̵ķ�����������÷������ʱ��Ҫ������getch()��������Ϊ����һ�η���������������ֵ��
#define s 115
#define a 97
#define d 100
#define SNAKE 1
#define FOOD 2
#define BAR 3
int direction = w;					//Ĭ�Ϸ���Ϊ��
int N = 0;							//����ȫ�ֱ���N���ı�ˢ���ٶȣ��Ӷ����Ӿ��ϸı���Ϸ�ٶ�
char map[17][17] = { 0 };		    //unsigned char��������8λ���ȣ���4λ��Ϊ�����꣨x��������λ��Ϊ�����꣨y��������x�ķ�Χ��0-15.���Ե�ͼ�ı߳�����15+2=17���������߽糤�ȣ�
unsigned char snake[80] = { 136 };	//��ͷ��snake[0]=62,��ʼֵΪ62����λ��������Ϊ1000 1000B��������xΪ8��������yΪ8
unsigned char food = 200;			//����ʳ��ĳ�ʼ���ꡣ��ʼֵΪ200����λ��������Ϊ1100 1000B�� ������xΪ12��������yΪ8
char len = 1;						//��ʼ���߳���Ϊ1
char top = 0;						//����ȫ�ֱ�����߷�
int plus = 0;
void transfer(unsigned char num, unsigned char *j, unsigned char *k);		//��ֵת�������ת������
void printout(void);				//ʵ�ִ�ӡ���������ܵĺ���
int getdirection(int olddirection);	//���հ�������ֵ�ĺ���
void movement(int direction);		//�������ƶ����ɵĺ���������Ϸ���صĺ��Ĺ��ɡ�
unsigned char offerfood(void);		//��������涨��Χ�ڵ�ʳ�ﺯ��
int gameover(void);					//��Ϸ�Ƿ�������жϺ���
void loadgame(void);				//��Ϸ��ȡ��ȡ����
void savegame(void);				//��Ϸ���ݴ洢����
void menuprint(void);				//��ӡ��Ϸ��ʼ����
void speedchoose(void);				//ѡ����Ϸ�ٶȺ���
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
			while (1)						//����û�гԵ��Լ������ڣ���Ϸ������ѭ����
			{
				direction = getdirection(direction);		//��ȡ���µķ����
				movement(direction);		//�ߵ��˶����ɺ����������ߵ��˶���������Ϸ����
				printout();					//��ӡ��Ϸ���ݣ�����ǽ�ڣ������հף�ʳ��
				if (gameover() == 1)			//�ж��Ƿ������Ϸ
				{
				break;
				}
			}
			break;
		case 0:
			N=200;
			while (1)						//����û�гԵ��Լ������ڣ���Ϸ������ѭ����
			{
				direction = getdirection(direction);		//��ȡ���µķ����
				crazymovement(direction);		//�ߵ��˶����ɺ����������ߵ��˶���������Ϸ����
				printout();					//��ӡ��Ϸ���ݣ�����ǽ�ڣ������հף�ʳ��
				if (gameover() == 1)			//�ж��Ƿ������Ϸ
				{
					break;
				}
			}
			break;

	}
	loadtop();
	printf("��Ϸ����\n");
	printf("�����Ϸ�÷�Ϊ��%d\n", len - 1);
	if(len-1>top)
	{	
		top = len - 1;
		savetop();
		printf("��ϲ�㴴�����¼�¼����\n");
	}
	else { printf("��ʷ��߷�Ϊ��%d\n", top); }
	printf("��������˳���Ϸ\n");
	_getch();
	return 0;
}
void transfer(unsigned char num, unsigned char *j, unsigned char *k)   //j����������x��k����������y����������λ�Ʋ�������㷨��8λ���������ĸ���λת��Ϊx�����꣬����λת��Ϊy������
{											//��ֵָ��ʵ�ֶ������ֵ�����ﲻ��ȫ�ֱ�������Ϊʵ�ֶ������ֵ������Ϊ���������������Թ��ˡ�����
	*j = num >> 4;							//������λ����λ��0��ʵ�ֽ�8λ�����Ƹ���λ��ֵ��*j
	*k = num&15;							//ͨ��0000 1111B���߼�����ʵ�ֵ���λ��ֵ��*k
}
int gameover(void)    //��Ϸ�����ж�����������ײǽ����ҧ���Լ�ʱ������ֵΪ1
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
	srand((unsigned int)time(NULL));      //�Ե�ǰʱ��Ϊ�����ṩ���ӹ�rand()�������ɸ�Ϊ�������
	do 
	{
		insnake = 0;
		randomfood = rand() % 255;        //ȡֵ��ΧΪ0-255����2^8-1,��8λ�����Ƶ�ȡֵ��Χ����unsigned char������ͬ
		transfer(randomfood, &fx, &fy);   //�����ֵת��Ϊ������x��������y
			for (int i = 0; i < len; i++)
			{
				if (randomfood == snake[i])//���������������
				{
					insnake = 1;
				}
			}
	} while (fx == 0 || fx == 16 || fy == 0 || fy == 16 || insnake == 1);
	return randomfood;
}
void printout(void)     //j���У�i���С�Ҳ����j��y�ᣬi��x�ᡣ
{						//���ݵ�ͼ��ÿ������������Ϸ�����д�ӡ
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
	if (_kbhit()!=0)			//��kbhit()��getch()���ʵ�ּ�����Ӧ
	{							//kbhit() ��鵱ǰ�Ƿ��м������룬�����򷵻�һ����0ֵ�����򷵻�0
		newdirection = _getch();//������ַ���Ӧ��ASCII�븳ֵ������
		if (newdirection == 113) 
		{
			 savegame(); 
			 printf("����ɹ�\n����������˳���Ϸ\n");
			_getch(); exit(0);
		}
		if (len > 1 && abs(newdirection - olddirection) == 3 || abs(newdirection - olddirection == 4))	//��������ȴ���1�����ܻ�ͷ
		{ newdirection = olddirection; }
		if (newdirection != 119 && newdirection !=115 &&  newdirection != 97 && newdirection != 100)		//���Ʒ���ļ�����İ��������򱣳�
		{ newdirection = olddirection; }
	}
	else{ newdirection = olddirection; }//��������ʱ�����򱣳�
	return newdirection;
}
void movement(int direction)
{
	unsigned char fx, fy;		//ʳ����У�������
	unsigned char x, y;			//�ߵ��У�������
	int last = snake[0];		//��ͷû����ǰ������ֵ��ֵ��last����
	int current;				//last��current����֮����������ĸ���
	transfer(food, &fx, &fy);	//����ת����������ʾʳ������ 
	transfer(snake[0], &x, &y);	//����ת����������ʾ��ͷ���� 
	switch (direction)			//�����˶�������������ͷ����ı仯
	{
	case w:
		y--;					//���˶��������1
		break;
	case s:
		y++;					//���˶��������1
		break;
	case a:
		x--;					//���˶��������1
		break;
	case d:
		x++;					//���˶��������1
		break;
	}
	snake[0] = (x<<4)|y;		//����ͷ��������ֵƴ�������൱�ڸ����˶������������ͷ����ֵ
	for (int i = 1; i < len; i++)
	{								//����ĸ��´ӵڶ��ڿ�ʼ���ڶ��ڵ�����ͷ��ԭ����λ��ֵ�������ڵ��ڵڶ���ԭ����λ��ֵ���Դ�����
		current = snake[i];			//������һ�ڵ�ֵ������current����
		snake[i] = last;			//����ǰһ�ڵ�ֵ��ֵ����һ��
		last = current;				//������һ�ڵ�ֵ��Ϊlast���������Ա㸳ֵ����һ��							
	}								//���ԭ����β��ֵ������last
	if (snake[0] == food)			//����֮�����ͷֵ��ʳ��ֵ���
	{
		food = offerfood();			//����offerfood�������ʳ���ʱfood�������µ�ֵ
		snake[len] = last;			//��ԭ����β������ֵ��ֵ����len+1�ڵ��߽ڣ���len+1�ڳ�Ϊ��β��
		len++;						//�߳���len����1
	}
	for (int j = 0; j < 17; j++)		//j��Ӧ�����꣬i��Ӧ�����꣬���н��и�ֵ
	{
		for (int i = 0; i < 17; i++)
		{
			if (i == 0 || i == 16 || j == 0 || j == 16){ map[i][j] = BAR; }
			else if (i == fx && j == fy){ map[i][j] = FOOD; }//�����fx��fy��δ���µ�ʳ�����꣬Ҳ����˵��������������ͷ��Ȼ�Ѿ���ʳ�������غϣ�ʳ������ֵ�����������µ����ֵ����ʳ���������������껹δ��ˢ��
			else{ map[i][j] = 0; }
		}
		for (int i = 0; i < len; i++)	//���߽��еĸ�ֵ���һ��Ҫ��ʳ��ĸ�ֵ���֮��ִ�У����ܱ�֤���º����ͷ��ʳ���������ʱ�����ܸ���ʳ�Ҳ�����߳Ե�ʳ���ʱ��Ļ��ʳ����ʧ����һ�����˶���ʳ�����������������ˢ��
		{
			transfer(snake[i], &x, &y);
			map[x][y] = SNAKE; 
		}
	}
}
void savegame(void)	//����˳��洢����ǰ״̬�����ݣ��������ȣ�����ʳ����ֵ���߸��ڵ���ֵ
{
	FILE*fp;
	errno_t err;
	err= fopen_s(&fp, "D:\savedata.txt", "w");		//fopen�������벻��ͨ��
	if (err!=0) { printf("�޷��ҵ��浵"); return; }	//�ļ�������ʱ��err����ֵ��0
	fwrite(&len, sizeof(len), 1, fp);
	fwrite(&direction, sizeof(direction), 1, fp);
	fwrite(&food, sizeof(food), 1, fp);
	for (int i = 0; i < len; i++)
	{
		fwrite(&snake[i], sizeof(unsigned char), 1, fp);
	}
	fclose(fp);		//����������
}
void loadgame(void)//����˳���ȡ�ĵ��еĳ��ȣ�����ʳ����ֵ���߸�����ֵ������
{
	FILE*fp;
	errno_t err;
	err = fopen_s(&fp,"D:\savedata.txt", "r");
	if (err!=0) { printf("�浵Ϊ�գ���������Ϸ\n"); return; }//�ļ�������ʱ��err����ֵ��0
	fread(&len, sizeof(len), 1, fp);
	fread(&direction, sizeof(direction), 1, fp);
	fread(&food, sizeof(food), 1, fp);
	for (int i = 0; i < len; i++)
	{
		fread(&snake[i], sizeof(unsigned char), 1, fp);
	}
	fclose(fp);
}
void menuprint(void)//��Ϸ��ʼ����Ĵ�ӡ
{
	printf("********************************\n");
	printf("**                            **\n");
	printf("**    ��ӭ����̰����С��Ϸ    **\n");
	printf("**                            **\n");
	printf("********************************\n\n");
	printf("��ʼ�˵���\n\n");
	printf("  	<�µ���Ϸ>\n\n");
	printf("  	<������Ϸ>\n\n");
	printf("  	<ɾ���浵>\n\n");
	printf("  	<�鿴��ʷ��߷�>\n\n");
	printf("  	<�����߷ּ�¼>\n\n");
	printf("\n\n");
	printf("����˵����\n");
	printf("    ��ʼ���水��<r>	������Ϸ��\n");
	printf("    ��ʼ���水��<m>	ɾ���浵����ʼ�µ���Ϸ��\n");
	printf("    ��ʼ���水��<t>	��ѯ��ʷ��߷֣���ʼ�µ���Ϸ��\n");
	printf("    ��ʼ���水��<c>	�����ʷ��߷֣���ʼ�µ���Ϸ��\n");
	printf("    ��ʼ�����������ⰴ������ʼ�µ���Ϸ��\n");
	printf("    ��Ϸ�а���<q>�˳���Ϸ���Զ����渲�Ǵ浵��\n");
	printf("    ��Ϸ�а���<w><s><a><d> �����������ҡ�\n");
}
void speedchoose(void)//��Ϸ��ʼǰ��Ϸ�ٶȵ�ѡ��ͨ���ı�������ʱʱ����ʵ����Ļˢ�µ�Ƶ�ʵı仯
{
	printf("\n\n��ѡ����Ϸ�����ٶ�\n��Ϸ������ʼ......\n<1>����	<2>��ͨ	<3>����\n");
	int g;
	scanf_s("%d", &g);
	if (g == 1) { N = 500; }
	else if (g == 2) { N = 300; }
	else if (g == 3) { N = 100; }
	else { printf("������������ȷ�ı��\n"); exit(0); }
}
void loadtop(void)
{
	FILE*stream;
	errno_t err1;
	err1 = fopen_s(&stream, "D:/topmark.txt", "r");
	if (err1 != 0) { printf("��ǰ������ߵ÷ּ�¼\n"); return; }
	fread(&top, sizeof(top), 1, stream);
	fclose(stream);
}
void savetop(void)
{
	FILE*stream;
	errno_t err1;
	err1 = fopen_s(&stream, "D:/topmark.txt", "w");
	if (err1 != 0) { printf("�޷��ҵ���򿪴浵\n"); return; }
	fwrite(&top, sizeof(top), 1, stream);
	fclose(stream);
}
void menuoption(void)
{
	int option = _getch();			//�Ƿ��ȡ��Ϸ����
	if (option == 116)
	{
		loadtop();
		printf("\n****************************\n");
		printf("**                        **\n");
		printf("**    ��ʷ��ø߷֣� %d   **\n", top);
		printf("**                        **\n");
		printf("****************************\n\n");

	}
	if (option == 114) { loadgame(); }
	if (option == 99) { remove("D:/topmark.txt"); }
	if (option == 109) { remove("D:/savedata.txt"); }
}
int gamemode(void)
{	
	printf("\nѡ����Ϸģʽ��\n");
	printf("####################################\n");
	printf("##               ##               ##\n");
	printf("##  <a>����ģʽ  ##  <b>���ģʽ  ##\n");
	printf("##               ##               ##\n");
	printf("####################################\nģʽ˵�������ģʽ�£���Ϸ�ٶȿ죬������෴���ߵĳɳ����ʵ�������Ϸ�÷ָ��ࡣ\n	����ս�ɣ�\n");
	int k = getchar();
	if (k == 97) { return 1; }
	else if(k == 98) { return 0; }
	else { printf("��������ȷ����ֵ"); exit(0); }
}
void crazymovement(int direction)
{
	unsigned char fx, fy;		//ʳ����У�������
	unsigned char x, y;			//�ߵ��У�������
	int last = snake[0];		//��ͷû����ǰ������ֵ��ֵ��last����
	int current;				//last��current����֮����������ĸ���
	transfer(food, &fx, &fy);	//����ת����������ʾʳ������ 
	transfer(snake[0], &x, &y);	//����ת����������ʾ��ͷ���� 
	switch (direction)			//�����˶�������������ͷ����ı仯
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
	snake[0] = (x << 4) | y;		//����ͷ��������ֵƴ�������൱�ڸ����˶������������ͷ����ֵ
	for (int i = 1; i < len; i++)
	{								//����ĸ��´ӵڶ��ڿ�ʼ���ڶ��ڵ�����ͷ��ԭ����λ��ֵ�������ڵ��ڵڶ���ԭ����λ��ֵ���Դ�����
		current = snake[i];			//������һ�ڵ�ֵ������current����
		snake[i] = last;			//����ǰһ�ڵ�ֵ��ֵ����һ��
		last = current;				//������һ�ڵ�ֵ��Ϊlast���������Ա㸳ֵ����һ��							
	}								//���ԭ����β��ֵ������last
	if (snake[0] == food)			//����֮�����ͷֵ��ʳ��ֵ���
	{
		plus = plus + 1;
		food = offerfood();			//����offerfood�������ʳ���ʱfood�������µ�ֵ
		for (int v = len;v<=len+plus;v++)
		{							//�Ե�һ��ʳ�ﳤһ�ڣ��Եڶ���ʳ�ﳤ���ڣ��Դ�����
			snake[v] = last;
		}
		len = len + plus;
	}
	for (int j = 0; j < 17; j++)		//j��Ӧ�����꣬i��Ӧ�����꣬���н��и�ֵ
	{
		for (int i = 0; i < 17; i++)
		{
			if (i == 0 || i == 16 || j == 0 || j == 16) { map[i][j] = BAR; }
			else if (i == fx && j == fy) { map[i][j] = FOOD; }//�����fx��fy��δ���µ�ʳ�����꣬Ҳ����˵��������������ͷ��Ȼ�Ѿ���ʳ�������غϣ�ʳ������ֵ�����������µ����ֵ����ʳ���������������껹δ��ˢ��
			else { map[i][j] = 0; }
		}
		for (int i = 0; i < len; i++)	//���߽��еĸ�ֵ���һ��Ҫ��ʳ��ĸ�ֵ���֮��ִ�У����ܱ�֤���º����ͷ��ʳ���������ʱ�����ܸ���ʳ�Ҳ�����߳Ե�ʳ���ʱ��Ļ��ʳ����ʧ����һ�����˶���ʳ�����������������ˢ��
		{
			transfer(snake[i], &x, &y);
			map[x][y] = SNAKE;
		}
	}
}
