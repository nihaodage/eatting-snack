#include <iostream>
#include <easyx.h>
#include <conio.h>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <mmsystem.h>
using namespace std;
#pragma comment (lib,"winmm.lib") 
#define wide 900//���ڵĿ�
#define high 700//���ڵĸ�
#define sx 50//�ߺ�ʳ��Ĵ�Сx
#define sy 50//�ߺ�ʳ��Ĵ�Сy
#define Max_size 500//������
bool HaveApple = 0;//�Ƿ���ƻ����0Ϊû�У�1Ϊ��
int Apple_x, Apple_y;
int score = 0;
char s[100];//s�Ƿ���
IMAGE head[4],Apple;
IMAGE body[4];//����
int dire[5][2] = { {1,0},{-1,0},{0,1},{0,-1 } };
void game();
void move();
void apple()
{
	if (HaveApple == 0)//���û��ƻ��������һ����ƻ��
	{
		Apple_x = (rand()*rand()+rand() + 114514) % 550;
		Apple_y=  (rand()*rand() + 1919810) % 600;
		HaveApple = 1;
	}
	else
	{
		putimage(Apple_x, Apple_y, &Apple);
	}
}
struct snake
{
	int size;//�ߵĽ���
	int speed;//�ٶ�
	int dire;//��ǰ����
	POINT coor[Max_size];//�ߵ�����
}snake;
void snakeinit()//��ʼ���ߵ�����
{
	snake.size = 2;//Ĭ��ͷ+����������
	snake.speed = 30;//�ٶ�Ϊ10
	snake.dire = 1;//����Ϊ��2Ϊ�ң�3Ϊ�ϣ�4Ϊ��
	snake.coor[0].x = 650 / 2;//��ǰ�ߵ�λ��x
	snake.coor[0].y = high / 2;//��ǰ�ߵ�λ��y
	/*snake.coor[1].x = 650 / 2 + 30;
	snake.coor[2].x = 650 / 2 + 60;
	snake.coor[1].y = high / 2;
	snake.coor[2].y = high / 2;*/
}
void first()//�������Ƶ�һ�����壬�Է���סͷ
{
	if (snake.dire == 1)
	{
		snake.coor[1].x = snake.coor[0].x + 30;
		snake.coor[1].y = snake.coor[0].y;
	}
	if (snake.dire == 2)
	{
		snake.coor[1].x = snake.coor[0].x - 30;
		snake.coor[1].y = snake.coor[0].y;
	}
	if (snake.dire == 3)
	{
		snake.coor[1].x = snake.coor[0].x;
		snake.coor[1].y = snake.coor[0].y+30;
	}
	if (snake.dire == 4)
	{
		snake.coor[1].x = snake.coor[0].x;
		snake.coor[1].y = snake.coor[0].y-30;
	}
}
void outimage()
{
	game();

		putimage(snake.coor[0].x, snake.coor[0].y, &head[snake.dire-1]);//���ݷ��������Ӧ��ͷ
		
		first();//�������Ƶ�һ������

		for (int i = snake.size; i >1; i--)//����һ����������������
		{
			snake.coor[i].x = snake.coor[i - 1].x;
			snake.coor[i].y = snake.coor[i - 1].y;
		}
		
		for (int i = 1; i <= snake.size; i++)//�������
		{
			putimage(snake.coor[i].x, snake.coor[i].y, &body[snake.dire-1]);
		}
		
}
void load()
{
	
	loadimage(&Apple, "./image/apple.jpg");//����ƻ��ͼƬ

	loadimage(&head[0], "./image/head_1.jpg");//������ͷͼƬ
	loadimage(&head[1], "./image/head_2.jpg");
	loadimage(&head[2], "./image/head_3.jpg");
	loadimage(&head[3], "./image/head_4.jpg");
	
	for (int i = 1; i <= snake.size; i++)
	{
		snake.coor[i].x = snake.coor[i - 1].x + 30;
		snake.coor[i].y = snake.coor[i - 1].y;
	}
	
}
void init()
{
	score = 0;

	mciSendString("open ./image/dead.mp3", NULL, 0, NULL);//������Ч
	mciSendString("open ./image/deadsceen.mp3",NULL, 0, NULL);//��������
	mciSendString("open ./image/eat.mp3", NULL, 0, NULL);//�Թ���
	mciSendString("open ./image/menu.mp3", NULL, 0, NULL);//������
	mciSendString("open ./image/game.mp3", NULL, 0, NULL);//��Ϸ����

		loadimage(&body[0], "./image/body_1.jpg");//����������
		loadimage(&body[1], "./image/body_2.jpg");//�����Һ���
		loadimage(&body[2], "./image/body_3.jpg");//�����Ϻ���
		loadimage(&body[3], "./image/body_4.jpg");//�����º���
	

	initgraph(900, 700);///��������
	setbkcolor(WHITE);//���ñ���ɫΪ��ɫ
	cleardevice();//�������豸�������ɫ
	
	settextstyle(100,0,"����");//�������ֿ�Ⱥ�����
	settextcolor(BLACK);//����������ɫΪ��ɫ
	outtextxy(wide / 2-150, high / 4-50, "̰����");//�������

	setlinecolor(BLACK);//���þ��α߿���ɫ
	rectangle(200, 450, 700, 550);//������α߿�

	settextstyle(50, 0, "����");//����������ʽ
	settextcolor(BLACK);//����������ɫ
	outtextxy(700 / 2-textwidth("���ո�ʼ��Ϸ")/4+5, (450 + 550) / 2-textheight("���ո�ʼ��Ϸ")/2, "���ո�ʼ��Ϸ");//����������·�����
	
}
void game()//������Ϸ����
{
	setbkcolor(WHITE);
	cleardevice();//��ձ��⻭��
	
	setfillcolor(BLACK);//���������ɫΪ��ɫ
	setfillstyle(BS_HATCHED, HS_BDIAGONAL);//���������ʽΪ���µ����ϵ�����
	fillrectangle(650, 0, 900, 700);//��������Ϊ����

	setlinestyle(PS_SOLID, 5);//���÷ָ�����ʽ
	setlinecolor(BLACK);//���÷ָ�����ɫΪ��ɫ
	line(650, 0, 650, 700);//����ָ�������
}
void move()
{
	if (snake.dire == 1)
	{
		snake.coor[0].x -= snake.speed;
	}
	if (snake.dire == 2)
	{
		snake.coor[0].x += snake.speed;
	}
	if (snake.dire == 3)
	{
		snake.coor[0].y -= snake.speed;
	}
	if (snake.dire == 4)
	{
		snake.coor[0].y += snake.speed;
	}
}
void Whatkey()//���ݰ����жϷ���
{
	if ((GetAsyncKeyState(VK_UP) & 0x8000 ? 1 : 0 || GetAsyncKeyState(0x57) & 0x8000 ? 1 : 0)&&snake.dire!=4)//���ϻ�w�򽫷����Ϊ3���ϣ������ͬ��
	{
		snake.dire = 3;
	}
	if ((GetAsyncKeyState(VK_DOWN) & 0x8000 ? 1 : 0 || GetAsyncKeyState(0x53) & 0x8000 ? 1 : 0)&&snake.dire!=3)
	{
		snake.dire = 4;
	}
	if ((GetAsyncKeyState(VK_LEFT) & 0x8000 ? 1 : 0 || GetAsyncKeyState(0x41) & 0x8000 ? 1 : 0)&&snake.dire!=2)
	{
		snake.dire = 1;
	}
	if ((GetAsyncKeyState(VK_RIGHT) & 0x8000 ? 1 : 0 || GetAsyncKeyState(0x44) & 0x8000 ? 1 : 0)&&snake.dire!=1)
	{
		snake.dire = 2;
	}
}
int eat()
{
	for (int i = snake.coor[0].x; i <= snake.coor[0].x+30; i++)//�����ͷ�������е����������ƻ�������������غ����ж�Ϊ�Ե���ƻ��
	{
		for (int j = snake.coor[0].y; j <= snake.coor[0].y + 30; j++)
		{
			if (i >= Apple_x && i <= Apple_x + 30 && j >= Apple_y && j <= Apple_y + 30)
			{
				mciSendString("close ./image/eat.mp3", NULL, 0, NULL);
				mciSendString("play ./image/eat.mp3 ", NULL, 0, NULL);
				snake.size++;
				HaveApple = 0;
				score++;
				return 0;
			}
		}
	}
}
void PassWall()//��ǽ
{
	if (snake.coor[0].x+50<= 0)snake.coor[0].x = 600;
	else if (snake.coor[0].x +40>=650)snake.coor[0].x = 0;
	else if (snake.coor[0].y + 30 <= 0)snake.coor[0].y = 680;
	else if (snake.coor[0].y  >= 700)snake.coor[0].y = 0;
}
bool IsTailDead()//���ײ��β�;���
{
	if (snake.size <= 4)return 1;//�������Ľ�������4�Ͳ����жϣ��϶�������
	for (int i = snake.coor[0].y; i <= snake.coor[0].y + 30; i++)//ֻ�ж�ͷ��ǰ����һ�е�����
	{
		for (int j = 5; j <= snake.size; j++)
		{
			if (snake.coor[0].x >= snake.coor[j].x && snake.coor[0].x <= snake.coor[j].x + 30 && i >= snake.coor[j].y && i <= snake.coor[j].y + 30)
			{
				return 0;
			}
		}
	}
	return 1;
}
bool IsWallDead()//��ǽ��
{
	if (snake.coor[0].x <= 0-40 || snake.coor[0].x >= 650+4|| snake.coor[0].y <= 0-50|| snake.coor[0].y >= 700+20)return 1;
	else return 0;
}
void ShowScore()
{
	sprintf(s, "%d", score);//��int�÷ַŽ��ַ�����

	settextcolor(BLACK);//����������ɫ
	settextstyle(30,0, "����");//����������ֺ�
	outtextxy(710, 50, "�÷�:");//���ǰ���
	outtextxy(800, 50,s);//�������

}
void Deadscreen()
{
	BeginBatchDraw();
	setbkcolor(DARKGRAY);
	cleardevice();
	//putimage(0, 0, &dead_bk);

	settextstyle(100, 0, "����");//�������ֿ�Ⱥ�����
	settextcolor(RED);//����������ɫΪ��ɫ
	outtextxy(wide / 2 - 150, high / 4 - 50, "������");//�����ʾ

	settextcolor(WHITE);
	settextstyle(50, 0, "����");//�������ֿ�Ⱥ�����
	outtextxy(wide / 2 - 150-100+50+20+15, high / 4 - 50+150, "���յ÷�:");//��ʾ���
	outtextxy(wide / 2 + 150-50, high / 4 - 50 + 150, s);//����
	
	settextstyle(50, 0, "����");//�������ֿ�Ⱥ�����
	outtextxy(wide / 2 -300-100, high / 4 - 50 + 150 + 200, "���س�������");
	outtextxy(wide / 2 , high / 4 - 50 + 150 + 200, "��ESC�˳���Ϸ");

	setfillcolor(WHITE);
	rectangle(wide / 2 - 300 - 100-10, high / 4 - 50 + 150 + 200-10, wide / 2 - 300 - 100 + textwidth("���ո�������")+10, high / 4 - 50 + 150 + 200 + 50+10);
	rectangle(wide / 2-10, high / 4 - 50 + 150 + 200-10, wide / 2-10 + textwidth("��ESC�˳���Ϸ")+20, high / 4 - 50 + 150 + 200 + textheight("��ESC�˳���Ϸ")+10);

	EndBatchDraw();
}
int main()
 {
	
	mciSendString("play ./image/menu.mp3 repeat", NULL, 0, NULL);
	srand(time(0));
	start:
	init();//ˢ��������
		
	while (1)
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x8000 )break;//������ո�Ϳ�ʼ��Ϸ
		mciSendString("close ./image/choose.mp3", NULL, 0, NULL);
		mciSendString("play ./image/choose.mp3", NULL, 0, NULL);
	}
	
	snakeinit();
	load();//��������

	if (MessageBox(NULL, "�Ƿ�����ǽģʽ��", "��ʾ", MB_YESNO | MB_SYSTEMMODAL) == IDYES)
	{
		mciSendString("stop ./image/menu.mp3 ", NULL, 0, NULL);
		mciSendString("play ./image/game.mp3 repeat", NULL, 0, NULL);
		while (1)
		{
			
			
			BeginBatchDraw();
			outimage();
			ShowScore();
			apple();//����ƻ��
			Whatkey();//�жϰ���
			Sleep(100);//�Ż��ٶ�
			move();//�ߵ��ƶ�
			eat();//�Ե�ƻ��
			PassWall();//��ǽ���������ѡ�񲻴�ǽ����ǽ��
			if (!IsTailDead())
			{
				break;
			}
			EndBatchDraw();
		}
	}
	else
	{
		mciSendString("stop ./image/menu.mp3 ", NULL, 0, NULL);
		mciSendString("play ./image/game.mp3 repeat", NULL, 0, NULL);
	while (1)
	{
		
		BeginBatchDraw();
		outimage();
		ShowScore();
		apple();//����ƻ��
		Whatkey();//�жϰ���
		Sleep(100);//�Ż��ٶ�
		move();//�ߵ��ƶ�
		eat();//�Ե�ƻ��
		if (!IsTailDead())
		{
			break;
		}
		if (IsWallDead())
		{
			break;
		}
		EndBatchDraw();
	}
}
	mciSendString("stop ./image/game.mp3", NULL, 0, NULL);
	mciSendString("play ./image/deadsceen.mp3 repeat", NULL, 0, NULL);
	Deadscreen();
	while (1)
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			mciSendString("stop ./image/deadsceen.mp3 ", NULL, 0, NULL);
			goto start;
		}
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		{
			exit(1);
		}
	}
}
	