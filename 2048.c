#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BOX_NUM 16
void init_keyboard();
void close_keyboard();
void cleanScreen();
void updateZeroInfo();
void init();
int fill();
void draw();
int move();
void getRow(int, int []);
void setRow(int, const int []);
void getRank(int, int[]);
void setRank(int, const int[]);
int mergeLeft(int [], int);
int mergeRight(int [], int);
int gameOver();

enum Directory
{
  Up = 0, Left, Down, Right
};

int BoxNum = BOX_NUM;
int boxes[BOX_NUM];
int zeroIndexes[BOX_NUM];
int zeroNum;
const int RowNum = 4;
const int RankNum = 4;
const char *Border    = "---------------------";
const char *Separator = "|----|----|----|----|";

int main(void) {
  init_keyboard();
  init();
  draw();
  int i = -1;
  while(1) {
    if (move()) 
    {
      updateZeroInfo();
      i = fill();
      draw();
    }
    if (gameOver()) {
      close_keyboard();
      printf("Game Over!\n");
      break;
    }    
  }
  return 0;
}

void cleanScreen()
{
	printf("\033[2J");
	printf("\033[0;0H");
}

void init()
{
  int i = 0;
  for (i = 0; i < BoxNum; i++) 
  {
    boxes[i] = 0;
  }
  updateZeroInfo();
  const int InitNum = 2;
  for (i = 0; i < InitNum; i++)
  {
    fill();
  }
}

void updateZeroInfo()
{
  zeroNum = 0;
  int i = 0;
  for (i = 0; i < BoxNum; i++)
  {
    if (boxes[i] == 0)
    {
      zeroIndexes[zeroNum] = i;
      zeroNum ++;
    }
  }
}

int fill()
{
  srand(time(NULL));
  int i = zeroIndexes[rand() % zeroNum];
  // 2%: 4, 98%: 2
  if (rand() % 100 < 2)
  {
    boxes[i] = 4;
  }
  else {
    boxes[i] = 2;
  }
  updateZeroInfo();
  return i;
}

void draw()
{
  int i = 0, j = 0, box = 0;
  cleanScreen();
  printf("%s\n", Border);
  for (i = 0; i < RowNum; i++)
  {
    const int offset = i * RankNum;
    printf("|");
    for (j = 0; j < RankNum; j++) 
    {
      box = boxes[offset + j];
      if (box == 0)
      {
        printf("    |");
      }
      else if (box < 1000) {
        printf("% 4d|", box);
      }
      else {
	printf("%d|", box);
      }
    }
    printf("\n");
    
    if (i == RowNum - 1) 
    {
      break;
    }
    printf("%s\n", Separator);
  }
  printf("%s\n", Border);
}

int move()
{
  char c = -1;
  scanf("%c", &c);
  int merged = 0;
  int i = 0;
  int len = 4;
  int boxes[len];
  enum Directory dir;
  switch (c) 
  {
    case 'h':
	dir = Left;
	break;
    case 'j':
	dir = Down;
	break;
    case 'k':
	dir = Up;
	break;
    case 'l':
	dir = Right;
	break;
    case 'q':
	close_keyboard();
	printf("Quit Game. Bye!\n");
	exit(0);
	break;
  }
  switch (dir) 
  {
    case Up:
      for (i = 0; i < RankNum; i++)
      {
        getRank(i, boxes);
        if (mergeLeft(boxes, len)) 
        {
          merged = 1;
        }
        setRank(i, boxes);
      }
      break;
    case Left:
      for (i = 0; i < RowNum; i++)
      {
        getRow(i, boxes);
        if (mergeLeft(boxes, len))
        {
          merged = 1;
        }
        setRow(i, boxes);
      }
      break;
    case Down:
      for (i = 0; i < RankNum; i++)
      {
        getRank(i, boxes);
        if (mergeRight(boxes, len))
        {
          merged = 1;
        }
        setRank(i, boxes);
      }
      break;
    case Right:
      for (i = 0; i < RowNum; i++)
      {
        getRow(i, boxes);
        if (mergeRight(boxes, len))
        {
          merged = 1;
        }
        setRow(i, boxes);
      }
      break;
  }
  return merged;
}

void getRow(int index, int row[])
{
  const int offset = index * RankNum;
  int i = 0;
  for (i = 0; i < RankNum; i++)
  {
    row[i] = boxes[i + offset];
  }
}

void setRow(int index, const int row[])
{
  const int offset = index * RankNum;
  int i = 0;
  for (i = 0; i < RankNum; i++)
  {
    boxes[i + offset] = row[i];
  }  
}

void getRank(int index, int rank[])
{
  int i = 0;
  for (i = 0; i < RowNum; i++)
  {
    rank[i] = boxes[index + i * RankNum];
  }
}  

void setRank(int index, const int rank[])
{
  int i = 0;
  for (i = 0; i < RowNum; i++)
  {
    boxes[index + i * RankNum] = rank[i];
  }
}

int mergeLeft(int boxes[], int len)
{
  int merged = 0;
  int stackTop = 0;
  int operation = 1;
  while (operation < len)
  {
    if (boxes[operation] == 0)
    {
      operation++;
      continue;
    }
    
    if (boxes[stackTop] == 0)
    {
      boxes[stackTop] = boxes[operation];
      boxes[operation] = 0;
      operation++;
      merged = 1;
    }
    else {
      if (boxes[stackTop] == boxes[operation])
      {
        boxes[stackTop] *= 2;
        boxes[operation] = 0;
        stackTop++;
        operation++;
        merged = 1;
      }
      else {
        stackTop++;
        operation = stackTop + 1;
      }
    }
  }
  return merged;
}

int mergeRight(int boxes[], int len)
{
  int merged = 0;
  int stackTop = len - 1;
  int operation = stackTop - 1;
  while (operation >= 0)
  {
    if (boxes[operation] == 0)
    {
      operation--;
      continue;
    }
    
    if (boxes[stackTop] == 0)
    {
      boxes[stackTop] = boxes[operation];
      boxes[operation] = 0;
      operation--;
      merged = 1;
    }
    else {
      if (boxes[stackTop] == boxes[operation]) {
        boxes[stackTop] *= 2;
        boxes[operation] = 0;
        stackTop--;
        operation--;
        merged = 1;
      }
      else {
        stackTop--;
        operation = stackTop - 1;
      }
    }
  }
  return merged;
}

int gameOver()
{
  /*
  * n n *
  n n n n
  n n n n
  * n n *
  */
  const int Len = 4;
  int side[Len] = {1, 2, 13, 14};
  int floor[Len] = {4, 8, 7, 11};
  int cross[Len] = {5, 6, 9, 10};
  int i = 0;
  int index;

  if (zeroNum > 0) {
    return 0;
  }
// side
//  printf("check side\n");
  for (i = 0; i < Len; i++)
  {
  	  index = side[i];
	  if (boxes[index] == boxes[index - 1] || boxes[index] == boxes[index + 1]) {
  	    return 0;
  	  }
	  else {
	    //printf("%d == %d == %d\n", boxes[index - 1], boxes[index], boxes[index + 1]);
	  }
  }
// floor
  //printf("check floor\n");
  for (i = 0; i < Len; i++)
  {
  	  index = floor[i];
	  if (boxes[index] == boxes[index - RankNum] || boxes[index] == boxes[index + RankNum])
          {
	    return 0;
	  }
	  else {
	    //printf("%d == %d == %d\n", boxes[index - RankNum], boxes[index], boxes[index + RankNum]);
	  }
  }
// cross
  //printf("check cross\n");
  for (i = 0; i < Len; i++)
  { 	
	  index = cross[i];
	  //printf("	side\n");
  	  if (boxes[index] == boxes[index - 1] || boxes[index] == boxes[index + 1])
    	  {
	    return 0;
	  }
	  else {
	    //printf("%d == %d == %d\n", boxes[index - 1], boxes[index], boxes[index + 1]);
  	  }
	  //printf("	floor\n");
  	  if (boxes[index] == boxes[index - RankNum] || boxes[index] == boxes[index + RankNum])
  	  {
	    return 0;
	  }
	  else {
	    //printf("%d == %d == %d\n", boxes[index - RankNum], boxes[index], boxes[index + RankNum]);
	  }
  }
  return 1;
}
