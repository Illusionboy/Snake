#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include <pthread.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include "snake.h"

const short w = WIDTH_MAP;
const short h = HIGHT_MAP;
int shared_ch = '\0';
pthread_mutex_t mutex;

void init_map(short **map);
void draw_map(short **arr, int rows, int cols);
void draw_snake(Snake_ptr snake);
void draw_food(int x, int y);

void* getInput(void* arg){
	//将终端设置为非阻塞
	struct termios term;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	
	while (1) {
		int ch = getch();
		pthread_mutex_lock(&mutex);
		shared_ch = ch;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int getchWithTimeout(int timeoutMillis){
	int ch;

	struct timespec req;
	req.tv_sec = 0;
	req.tv_nsec = 10000000;	//10毫秒
	int totalSleepTime = 0;
	pthread_mutex_lock(&mutex);
	while (shared_ch == '\0' && totalSleepTime < timeoutMillis){
		pthread_mutex_unlock(&mutex);
		nanosleep(&req, NULL);
		totalSleepTime += 10;
		pthread_mutex_lock(&mutex);
	}

	ch = shared_ch;
	shared_ch = '\0';	//初始化共享char
	pthread_mutex_unlock(&mutex);

	return ch;
}

void randNewFood(Snake_ptr snake, short** map, int x_max, int y_max, int* x_food, int* y_food){
	// Cell坐标
	int x;
	int y;
	do {
		x = rand() % x_max;
		y = rand() % y_max;
	} while (map[x][y] == -1 || isInSnake(snake, x, y));
	*x_food = x;
	*y_food = y;
}

bool isInSNodes(SNode_ptr snode, int x, int y){
	while (snode != NULL) {
		if (x == snode->x && y == snode->y)
			return true;
		snode = snode->next;
	}
	return false;
}



int main(){

	short** map = (short**)malloc(h * sizeof(short*));
	for (int i = 0; i < h; i++)
		map[i] = (short*)calloc(w, sizeof(short));
	init_map(map);

	// 初始化Snake对象
	Snake_ptr snake;
	snake = (Snake_ptr)malloc(sizeof(Snake)); 
	appendSNode_head(snake, h>>1, w>>1, RIGHT);

	//初始化食物
	int x_food = (int)(h/2)+1;
	int y_food = (int)(w/2)+1;

	// 初始化ncurses
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	curs_set(0);

	draw_map(map, h, w);
	draw_snake(snake);
	draw_food(x_food, y_food);

	// 等待用户按下按钮
	pthread_t thread1;
	pthread_create(&thread1, NULL, getInput, NULL);

	int exitWhileFlag = 0;
	enum Direction dirc = RIGHT;
	bool eaten = false;

	while (1) {
		int ch = getchWithTimeout(200);
		enum Direction dirc_bak = dirc;
		switch (ch) {
			case KEY_UP:
				dirc = UP;
				break;
			case KEY_DOWN:
				dirc = DOWN;
				break;
			case KEY_RIGHT:
				dirc = RIGHT;
				break;
			case KEY_LEFT:
				dirc = LEFT;
				break;
			case 'q':
				exitWhileFlag = 1;
				break;
			default:
				break;
		}
		if (exitWhileFlag) {
			break;
		}
		if (isDircInverse(snake, dirc))
			dirc = dirc_bak;
		next_snake(snake, dirc, eaten);
		eaten = false;
		draw_map(map, h, w);
		draw_snake(snake);
		draw_food(x_food, y_food);
		if (x_food == snake->head->x && y_food == snake->head->y){
			eaten = true;
			randNewFood(snake, map, h, w, &x_food, &y_food);
		}
		int x_head = snake->head->x;
		int y_head = snake->head->y;
		if (isInSNodes(snake->head->next, x_head, y_head) || map[x_head][y_head] == -1){
			clear();
			// 绘制"GAME OVER"并加边框
			int height = 5;
			int width = 30;
			int start_y = (LINES - height) / 2; // 竖直居中
			int start_x = (COLS - width) / 2; // 水平居中
			WINDOW *win = newwin(height, width, start_y, start_x);
			box(win, 0, 0); // 加边框
			mvwprintw(win, 2, 5, "GAME OVER");
			wrefresh(win);
			usleep(1000000);	// 暂停1s
			break;
		}
		refresh();
	}

	endwin();

	deleteSnake(snake);

	for (int i = 0; i < h; i++)
		free(map[i]);
	free(map);
	return 0;
}

void init_map(short **map){
	/* 
	 * 0为空，-1为墙
	 * 最外层设置为墙
	 */
	short i=0, j=0;
	for(; j < w; j++)
		map[i][j] = -1;
	for(i=1; i < h-1; i++)
	{
		map[i][0] = -1;
		map[i][w-1] = -1;
	}
	for(i=h-1, j=0; j < w; j++)
		map[i][j] = -1;
}

void draw_map(short **arr, int rows, int cols){
	//展示面板
	//
	for (int i = 0;i <rows; i++){
		for (int j = 0; j < cols; j++){
			if (arr[i][j] == -1){
				mvaddch(i, j, (char)219);
		//		mvaddch(i, 2*j+1, (char)219);
			}
			else{
				mvaddch(i, j, (char)32);
		//		mvaddch(i, 2*j+1, (char)32);
			}
		}
	}
}

void draw_snake(Snake_ptr snake){
	//画蛇
	//遍历
	SNode_ptr current = snake->head;
	while (current != NULL){
		int x = current->x;
		int y = current->y;
		mvaddch(x, y, '@');
		current = current->next;
	}
}
	
void draw_food(int x_food, int y_food){
	//画食物
	mvaddch(x_food, y_food, ACS_DIAMOND);
}

