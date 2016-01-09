#include "snake.h"
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

SnakePart::SnakePart(int col,int row)
{
	x=col;
	y=row;
}

SnakePart::SnakePart()
{
	x=0;
	y=0;
}

Snake::Snake()
{
	initscr();
	nodelay(stdscr,true);
	keypad(stdscr,true);
	noecho();
	curs_set(false);
	getmaxyx(stdscr,maxHeight,maxWidth);

	snakePartChar='x';
	wallChar=char(219);	
	foodChar='*';

	for (int i=0; i<5; i++)
		snakeVec.push_back(SnakePart(40+i,10));

	points=0;
	delay=110000;
	getFood=false;
	direction='L';
	srand(time(0));
	
	putFood();

	for (int i=0; i<maxWidth-1; i++)
	{
		move(0,i);
		addch(wallChar);
	}

	for (int i=0; i<maxWidth-1; i++)
	{
		move(maxHeight-2,i);
		addch(wallChar);
	}

	for (int i=0; i<maxHeight-1; i++)
	{
		move(i,0);
		addch(wallChar);
	}

	for (int i=0; i<maxHeight-1; i++)
	{
		move(i,maxWidth-2);
		addch(wallChar);
	}

	for (int i=0; i<snakeVec.size(); i++)
	{
		move(snakeVec[i].y,snakeVec[i].x);
		addch(snakePartChar);
	}

	move(maxHeight-1,0);
	printw("POINTS: %d",points);

	move(food.y,food.x);
	addch(foodChar);
	refresh();
}

Snake::~Snake()
{
	nodelay(stdscr,false);
	getch();
	endwin();
}

void Snake::putFood()
{
	while (1)
	{
		int tmpx=rand()%maxWidth+1;
		int tmpy=rand()%maxHeight+1;
		
		for (int i=0; i<snakeVec.size(); i++)
			if ((snakeVec[i].x == tmpx) && (snakeVec[i].y == tmpy))
				continue;

		if (tmpx >= maxWidth-2 || tmpy >= maxHeight-3)
			continue;

		food.x = tmpx;
		food.y = tmpy;

		break;
	}

	move(food.y,food.x);
	addch(foodChar);
	refresh();
}

bool Snake::collision()
{
	if ((snakeVec[0].x == 0) || (snakeVec[0].x == maxWidth-1) || (snakeVec[0].y == 0) || (snakeVec[0].y == maxHeight-2))
		return true;

	for (int i=2; i<snakeVec.size(); i++)
		if ((snakeVec[0].x == snakeVec[i].x) && (snakeVec[0].y == snakeVec[i].y))
			return true;

	if ((snakeVec[0].x == food.x) && (snakeVec[0].y == food.y))
	{
		getFood = true;
		putFood();
		points += 10;
		move(maxHeight-1,0);
		printw("POINTS: %d",points);

		if ((points%100) == 0)
			delay -= 10000;
	}
	else
		getFood = false;

	return false;
}

void Snake::moveSnake()
{
	int temp = getch();

	switch (temp)
	{
	case KEY_LEFT:
		if (direction != 'R')
			direction = 'L';
		break;
	case KEY_UP:
		if (direction != 'D')
			direction = 'U';
		break;
	case KEY_RIGHT:
		if (direction != 'L')
			direction = 'R';
		break;
	case KEY_DOWN:
		if (direction != 'U')
			direction = 'D';
		break;
	case KEY_BACKSPACE:
		direction = 'Q';
		break;
	}

	if (!getFood)
	{
		move(snakeVec[snakeVec.size()-1].y,snakeVec[snakeVec.size()-1].x);
		addch(' ');
		refresh();
		snakeVec.pop_back();
	}

	if (direction == 'L')
		snakeVec.insert(snakeVec.begin(),SnakePart(snakeVec[0].x-1,snakeVec[0].y));
	else if (direction == 'R')
		snakeVec.insert(snakeVec.begin(),SnakePart(snakeVec[0].x+1,snakeVec[0].y));
	else if (direction == 'U')
		snakeVec.insert(snakeVec.begin(),SnakePart(snakeVec[0].x,snakeVec[0].y-1));
	else if (direction == 'D')
		snakeVec.insert(snakeVec.begin(),SnakePart(snakeVec[0].x,snakeVec[0].y+1));

	move(snakeVec[0].y,snakeVec[0].x);
	addch(snakePartChar);
	refresh();
}

void Snake::start()
{
	while (1)
	{
		if (collision())
		{
			move(maxHeight/2-1,maxWidth/2-4);
			printw("GAME OVER");
			break;
		}

		moveSnake();

		if ((direction == 'q') || (direction == 'Q'))
			break;

		usleep(delay);
	}
}
