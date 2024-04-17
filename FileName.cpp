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
#define wide 900//窗口的宽
#define high 700//窗口的高
#define sx 50//蛇和食物的大小x
#define sy 50//蛇和食物的大小y
#define Max_size 500//最大节数
bool HaveApple = 0;//是否有苹果，0为没有，1为有
int Apple_x, Apple_y;
int score = 0;
char s[100];//s是分数
IMAGE head[4],Apple;
IMAGE body[4];//身体
int dire[5][2] = { {1,0},{-1,0},{0,1},{0,-1 } };
void game();
void move();
void apple()
{
	if (HaveApple == 0)//如果没有苹果就生成一个新苹果
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
	int size;//蛇的节数
	int speed;//速度
	int dire;//当前方向
	POINT coor[Max_size];//蛇的坐标
}snake;
void snakeinit()//初始化蛇的数据
{
	snake.size = 2;//默认头+上两节身子
	snake.speed = 30;//速度为10
	snake.dire = 1;//方向为左，2为右，3为上，4为下
	snake.coor[0].x = 650 / 2;//当前蛇的位置x
	snake.coor[0].y = high / 2;//当前蛇的位置y
	/*snake.coor[1].x = 650 / 2 + 30;
	snake.coor[2].x = 650 / 2 + 60;
	snake.coor[1].y = high / 2;
	snake.coor[2].y = high / 2;*/
}
void first()//单独控制第一节身体，以防挡住头
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

		putimage(snake.coor[0].x, snake.coor[0].y, &head[snake.dire-1]);//根据方向输出对应的头
		
		first();//单独控制第一节身体

		for (int i = snake.size; i >1; i--)//将第一节以外的身体跟上来
		{
			snake.coor[i].x = snake.coor[i - 1].x;
			snake.coor[i].y = snake.coor[i - 1].y;
		}
		
		for (int i = 1; i <= snake.size; i++)//输出身体
		{
			putimage(snake.coor[i].x, snake.coor[i].y, &body[snake.dire-1]);
		}
		
}
void load()
{
	
	loadimage(&Apple, "./image/apple.jpg");//加载苹果图片

	loadimage(&head[0], "./image/head_1.jpg");//加载蛇头图片
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

	mciSendString("open ./image/dead.mp3", NULL, 0, NULL);//加载音效
	mciSendString("open ./image/deadsceen.mp3",NULL, 0, NULL);//死亡界面
	mciSendString("open ./image/eat.mp3", NULL, 0, NULL);//吃果子
	mciSendString("open ./image/menu.mp3", NULL, 0, NULL);//主界面
	mciSendString("open ./image/game.mp3", NULL, 0, NULL);//游戏音乐

		loadimage(&body[0], "./image/body_1.jpg");//跟在左后面的
		loadimage(&body[1], "./image/body_2.jpg");//跟在右后面
		loadimage(&body[2], "./image/body_3.jpg");//跟在上后面
		loadimage(&body[3], "./image/body_4.jpg");//跟在下后面
	

	initgraph(900, 700);///创建窗口
	setbkcolor(WHITE);//设置背景色为白色
	cleardevice();//清空输出设备来输出白色
	
	settextstyle(100,0,"黑体");//设置文字宽度和字体
	settextcolor(BLACK);//设置文字颜色为黑色
	outtextxy(wide / 2-150, high / 4-50, "贪吃蛇");//输出标题

	setlinecolor(BLACK);//设置矩形边框颜色
	rectangle(200, 450, 700, 550);//输出矩形边框

	settextstyle(50, 0, "黑体");//设置文字样式
	settextcolor(BLACK);//设置文字颜色
	outtextxy(700 / 2-textwidth("按空格开始游戏")/4+5, (450 + 550) / 2-textheight("按空格开始游戏")/2, "按空格开始游戏");//输出主界面下方文字
	
}
void game()//加载游戏画面
{
	setbkcolor(WHITE);
	cleardevice();//清空标题画面
	
	setfillcolor(BLACK);//设置填充颜色为黑色
	setfillstyle(BS_HATCHED, HS_BDIAGONAL);//设置填充样式为左下到右上的线条
	fillrectangle(650, 0, 900, 700);//填充矩形作为线条

	setlinestyle(PS_SOLID, 5);//设置分割画面的样式
	setlinecolor(BLACK);//设置分割画面的颜色为黑色
	line(650, 0, 650, 700);//输出分割画面的线条
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
void Whatkey()//根据按键判断方向
{
	if ((GetAsyncKeyState(VK_UP) & 0x8000 ? 1 : 0 || GetAsyncKeyState(0x57) & 0x8000 ? 1 : 0)&&snake.dire!=4)//按上或w则将方向改为3即上，下面的同理
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
	for (int i = snake.coor[0].x; i <= snake.coor[0].x+30; i++)//如果蛇头部分所有的整数坐标和苹果的任意坐标重合则判断为吃掉该苹果
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
void PassWall()//穿墙
{
	if (snake.coor[0].x+50<= 0)snake.coor[0].x = 600;
	else if (snake.coor[0].x +40>=650)snake.coor[0].x = 0;
	else if (snake.coor[0].y + 30 <= 0)snake.coor[0].y = 680;
	else if (snake.coor[0].y  >= 700)snake.coor[0].y = 0;
}
bool IsTailDead()//如果撞到尾巴就死
{
	if (snake.size <= 4)return 1;//如果身体的解暑少于4就不用判断，肯定不会死
	for (int i = snake.coor[0].y; i <= snake.coor[0].y + 30; i++)//只判断头最前面那一列的坐标
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
bool IsWallDead()//碰墙死
{
	if (snake.coor[0].x <= 0-40 || snake.coor[0].x >= 650+4|| snake.coor[0].y <= 0-50|| snake.coor[0].y >= 700+20)return 1;
	else return 0;
}
void ShowScore()
{
	sprintf(s, "%d", score);//将int得分放进字符串中

	settextcolor(BLACK);//设置文字颜色
	settextstyle(30,0, "黑体");//设置字体和字号
	outtextxy(710, 50, "得分:");//输出前面的
	outtextxy(800, 50,s);//输出分数

}
void Deadscreen()
{
	BeginBatchDraw();
	setbkcolor(DARKGRAY);
	cleardevice();
	//putimage(0, 0, &dead_bk);

	settextstyle(100, 0, "黑体");//设置文字宽度和字体
	settextcolor(RED);//设置文字颜色为黑色
	outtextxy(wide / 2 - 150, high / 4 - 50, "你输了");//输出提示

	settextcolor(WHITE);
	settextstyle(50, 0, "黑体");//设置文字宽度和字体
	outtextxy(wide / 2 - 150-100+50+20+15, high / 4 - 50+150, "最终得分:");//提示语句
	outtextxy(wide / 2 + 150-50, high / 4 - 50 + 150, s);//分数
	
	settextstyle(50, 0, "黑体");//设置文字宽度和字体
	outtextxy(wide / 2 -300-100, high / 4 - 50 + 150 + 200, "按回车重新玩");
	outtextxy(wide / 2 , high / 4 - 50 + 150 + 200, "按ESC退出游戏");

	setfillcolor(WHITE);
	rectangle(wide / 2 - 300 - 100-10, high / 4 - 50 + 150 + 200-10, wide / 2 - 300 - 100 + textwidth("按空格重新玩")+10, high / 4 - 50 + 150 + 200 + 50+10);
	rectangle(wide / 2-10, high / 4 - 50 + 150 + 200-10, wide / 2-10 + textwidth("按ESC退出游戏")+20, high / 4 - 50 + 150 + 200 + textheight("按ESC退出游戏")+10);

	EndBatchDraw();
}
int main()
 {
	
	mciSendString("play ./image/menu.mp3 repeat", NULL, 0, NULL);
	srand(time(0));
	start:
	init();//刷新主界面
		
	while (1)
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x8000 )break;//如果按空格就开始游戏
		mciSendString("close ./image/choose.mp3", NULL, 0, NULL);
		mciSendString("play ./image/choose.mp3", NULL, 0, NULL);
	}
	
	snakeinit();
	load();//加载数据

	if (MessageBox(NULL, "是否开启穿墙模式？", "提示", MB_YESNO | MB_SYSTEMMODAL) == IDYES)
	{
		mciSendString("stop ./image/menu.mp3 ", NULL, 0, NULL);
		mciSendString("play ./image/game.mp3 repeat", NULL, 0, NULL);
		while (1)
		{
			
			
			BeginBatchDraw();
			outimage();
			ShowScore();
			apple();//生成苹果
			Whatkey();//判断按键
			Sleep(100);//放缓速度
			move();//蛇的移动
			eat();//吃掉苹果
			PassWall();//穿墙函数，如果选择不穿墙则碰墙死
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
		apple();//生成苹果
		Whatkey();//判断按键
		Sleep(100);//放缓速度
		move();//蛇的移动
		eat();//吃掉苹果
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
	