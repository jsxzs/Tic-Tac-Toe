#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define COM 1  //表示电脑下的棋，打印时用‘X’表示
#define MAN -1 //表示人类下的棋，打印时用‘O’表示

int board[3][3]; //记录棋盘
int num;		 //遍历搜索时 记录已下的棋子个数

void init(void);									   //初始化棋盘
void show(void);									   //打印棋盘
int value(void);									   //评估函数，对当前棋盘情况进行打分
int alpha_beta_Sarch(int alpha, int beta, int player); // alpha-beta剪枝搜索算法，返回当前下法最好的分数

void init(void)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			board[i][j] = 0; //将数组board的所有元素置为0
	num = 0;				 //棋子个数也初始化为0
}

void show(void)
{
	int i, j;
	printf("   1   2   3  \n");
	printf(" +---+---+---+\n");
	for (i = 0; i < 3; i++)
	{
		printf("%d", i + 1);
		for (j = 0; j < 3; j++)
		{
			if (board[i][j] == COM)
			{
				printf("| X ");
			}
			else if (board[i][j] == MAN)
			{
				printf("| O ");
			}
			else
			{
				printf("|   ");
			}
		}
		printf("|\n");
		printf(" +---+---+---+\n");
	}
}

//局面估价函数
int value(void)
{
	// 电脑赢时，返回+1；人类赢时，返回-1；
	for (int i = 0; i < 3; i++)
	{
		//某条横线或竖线上，有一方胜利时
		if (board[i][0] == COM && board[i][1] == COM && board[i][2] == COM)
			return 1;
		if (board[0][i] == COM && board[1][i] == COM && board[2][i] == COM)
			return 1;
		if (board[i][0] == MAN && board[i][1] == MAN && board[i][2] == MAN)
			return -1;
		if (board[0][i] == MAN && board[1][i] == MAN && board[2][i] == MAN)
			return -1;
	}
	//某条斜线上，有一方胜利时
	if (board[0][0] == COM && board[1][1] == COM && board[2][2] == COM)
		return 1;
	if (board[2][0] == COM && board[1][1] == COM && board[0][2] == COM)
		return 1;
	if (board[0][0] == MAN && board[1][1] == MAN && board[2][2] == MAN)
		return -1;
	if (board[2][0] == MAN && board[1][1] == MAN && board[0][2] == MAN)
		return -1;
	return 0; //平局时，返回0
}

int alpha_beta_Sarch(int alpha, int beta, int player)
{
	int ans = value();
	if (num == 9 || ans == 1 || ans == -1) //当棋盘满了，或某一方已经胜利时，直接返回value();
		return ans;

	if (player == 1) //人类回合
	{
		int a = 0, b = 2;
		for (int i = 0; i < 3; i++) //遍历搜索棋盘
			for (int j = 0; j < 3; j++)
			{
				if (board[i][j] == 0) //若该位置为空
				{
					board[i][j] = MAN; //若人类在此位置落子
					num += 1;
					int temp = alpha_beta_Sarch(a, b, 0); //递归，继续下一回合
					num -= 1;
					board[i][j] = 0; //恢复棋盘
					if (temp < b)	 //更新当前节点的最低分
						b = temp;
					if (b <= alpha) //剪枝
						return alpha;
				}
			}
		return b;
	}
	else //电脑回合
	{
		int a = -2, b = 0;
		for (int i = 0; i < 3; i++) //遍历搜索棋盘
			for (int j = 0; j < 3; j++)
			{
				if (board[i][j] == 0) //若该位置为空
				{
					board[i][j] = COM; //若电脑在此位置落子
					num += 1;
					int temp = alpha_beta_Sarch(a, b, 1);
					num -= 1;
					board[i][j] = 0; //恢复棋盘
					if (temp > a)	 //更新当前节点的最高分
						a = temp;
					if (a >= beta) //剪枝
						return beta;
				}
			}
		return a;
	}
}

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	init(); //初始化棋盘
	show();
	int step = 0; //记录回合数，即棋盘上已下的棋子个数
	int alpha, beta;
	while (1)
	{
		if (value() == 1 || value() == -1)
			break;
		printf("\n你的回合\n");
		int x, y; //人类输入的坐标，x表示行数，y表示列数
		printf("请输入位置坐标(行列之间用空格隔开),行数x:1~3,列数y:1~3:");
		scanf("%d %d", &x, &y);
		while (x < 1 || x > 3 || y < 1 || y > 3)
		{
			printf("您输入的坐标错误,请重新输入,行数x:1~3,列数y:1~3:");
			scanf("%d %d", &x, &y);
		}
		while (board[x][y] != 0)
		{
			printf("该位置已有棋，请重新输入:");
			scanf("%d %d", &x, &y);
		}
		x--, y--;
		board[x][y] = MAN;
		num += 1;
		show();

		step++;
		if (step == 9) //棋盘满了
			break;

		alpha = -2, beta = 0;
		//遍历搜索，寻找一个评分最高的下棋位置
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
			{
				if (board[i][j] == 0)
				{
					board[i][j] = COM;
					num += 1;
					int temp = alpha_beta_Sarch(alpha, beta, 1);
					num -= 1;
					board[i][j] = 0;
					if (temp > alpha) //更新
					{
						alpha = temp;
						x = i, y = j;
					}
				}
			}
		board[x][y] = COM;
		num += 1;
		step++;
		printf("\n电脑回合:\n");
		show();
	}
	if (value() == -1)
		printf("你赢了!\n");
	else if (value() == 1)
		printf("你输了!\n");
	else
		printf("平局!\n");
	system("pause");
	return 0;
}
