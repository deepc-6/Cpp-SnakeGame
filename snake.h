#ifndef SNAKE_H
#define SNAKE_H

#include <vector>

struct SnakePart
{
	int x,y;
	SnakePart(int col,int row);
	SnakePart();
};

class Snake
{
private:
	int points,delay,maxWidth,maxHeight;
	char direction,snakePartChar,wallChar,foodChar;
	bool getFood;
	SnakePart food;
	std::vector<SnakePart> snakeVec;

	void putFood();
	bool collision();
	void moveSnake();
public:
	Snake();
	~Snake();
	void start();
};

#endif // SNAKE_H
