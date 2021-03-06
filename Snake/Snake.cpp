
#include "Snake.h"

Snake* Snake::_instance = 0;

Snake::Snake(void)
{
	intTop = 2;
	intLeft = 2;
	intRight = WIDTH - 3;
	intBottom = HEIGHT - 3;
	con = SnakeAPI::getInstance();
	con->setTitle("Funkoi - Snake");
	con->setBackGroundColor(LIGHTGRAY);
	con->setTextColor(BLACK);
	con->clear();
	srand((unsigned int)clock());
}

Snake* Snake::getInstance()
{
	if (!_instance)
		_instance = new Snake;
	return _instance;
}
void Snake::init()
{
	con->clear();
	for (int i = 0; i < WIDTH; ++i)
		for (int j = 0; j < HEIGHT; ++j)
		{
			B[i][j] = 0;
		}

	for (int i = intLeft; i <= intRight; ++i)
	{
		con->putChar('*',i,intTop - 1);
		con->putChar('*',i,intBottom + 1);
		B[i][intTop - 1] = B[i][intBottom + 1] = -1; 
	}

	for (int i = intTop; i <= intBottom; ++i)
	{
		con->putChar('*',intLeft - 1,i);
		con->putChar('*',intRight + 1,i);
		B[intLeft - 1][i] = B[intRight + 1][i] = -1;
	}
	while (!snakeBody.empty()) snakeBody.pop();
	curDirect =  D_EAST;

	int midX = (intRight - intLeft) / 2 + intLeft;
	int midY = (intBottom - intTop) / 2 + intTop;

	snakeHead.X = midX;
	snakeHead.Y = midY;

	B[midX][midY] = -1;

	con->putChar(SNAKE_CHAR, snakeHead.X, snakeHead.Y);

	snakeBody.push(snakeHead);

	eat(D_EAST);
}

void Snake::newGame()
{
	intLenght = 2;
	init();
	play();
}
void Snake::run()
{
	con->clear();
	int mX = (intRight - intLeft) / 2 + intLeft;
	int mY = (intBottom - intTop) / 2 + intTop;
	con->gotoxy(mX , mY - 6);
	newGame();
}

int Snake::moveTo(int direction)
{
	int ret = eat(direction);
	if (ret == 0)
	{
		POSITION tail = snakeBody.front();
		snakeBody.pop();
		con->putChar(' ', tail.X, tail.Y);
		B[tail.X][tail.Y] = 0;
	}  
	return ret;

}
void Snake::endGame()
{
	int mX = (intRight - intLeft) / 2 + intLeft;
	int mY = (intBottom - intTop) / 2 + intTop;
	fflush(stdin);
	_getch();
	fflush(stdin);
}
int Snake::eat(int dir)
{
	int dR;
	if (dir == D_NORTH) 
		dR = 5;
	else if (dir == D_WEST) 
		dR = 0;
	else
		dR = dir;

	if (abs(curDirect - dir) <= 1 || abs(curDirect - dR) <= 1)
		curDirect = dir;

	switch (curDirect)
	{
		case D_NORTH:
			--snakeHead.Y;
			break;
		case D_EAST:
			++snakeHead.X;
			break;
		case D_SOUTH:
			++snakeHead.Y;
			break;
		case D_WEST:
			--snakeHead.X;
	}

	con->putChar(SNAKE_CHAR, snakeHead.X, snakeHead.Y);    

	int ret = B[snakeHead.X][snakeHead.Y];

	B[snakeHead.X][snakeHead.Y] = -1;
	snakeBody.push(snakeHead);

	return ret;
}
bool Snake::play()
{
	int direct;
	int moveRet;
	
	genTarget();
	for (;;)
	{
		Sleep(Speed);
		direct = curDirect;
		if (_kbhit())
		{
			char ch = (char)_getch();
			if (ch < 0)
			{
				ch = (char)_getch();
				switch (ch)
				{
				case KEY_UP:
					direct = D_NORTH;
					break;
				case KEY_DOWN:
					direct = D_SOUTH;
					break;
				case KEY_LEFT:
					direct = D_WEST;
					break;
				case KEY_RIGHT:
					direct = D_EAST;
				}
			}
		}

		if (isPlay)
		{
			moveRet = moveTo(direct);
			if (moveRet == -1)
			{
				endGame();
				return false;
			}
			else if (moveRet > 0)
			{
				++intLenght;
				genTarget();
			}
			con->gotoxy(snakeHead.X, snakeHead.Y);
		}
	}
}
void Snake::genTarget()
{
	int rX, rY;
	do
	{
		rX = rand() % (intRight - intLeft + 1) + intLeft;
		rY = rand() % (intBottom - intTop + 1) + intTop;
	} 
	while (B[rX][rY] == -1);
	B[rX][rY] = 1;
	con->putChar('*', rX, rY);
}
Snake::~Snake(void)
{
}
