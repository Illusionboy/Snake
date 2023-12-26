#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "snake.h"

void appendSNode_head(Snake_ptr snake, int x, int y, enum Direction dirc){
        //在头部添加节点
        SNode_ptr newNode = (SNode_ptr)malloc(sizeof(SNode));
        if (newNode != NULL){
                newNode->x = x;
                newNode->y = y;
                newNode->prev = NULL;
        }
        else{
                printf("Memory allocation error.\n");
                return;
        }
	
	snake->dirc = dirc;
        if (snake->head == NULL){
                snake->len = 1;
                snake->head = newNode;
                snake->tail = newNode;
                return;
        }
        else{
                snake->len ++;
                SNode_ptr current = snake->head;
                snake->head = newNode;
                newNode->next = current;
                current->prev = newNode;
        }
}

void appendSNode_tail(Snake_ptr snake, int x, int y){
        //在尾部添加节点
        SNode_ptr newNode = (SNode_ptr)malloc(sizeof(SNode));
        if (newNode != NULL){
                newNode->x = x;
                newNode->y = y;
                newNode->next = NULL;
        }
        else{
                printf("Memory allocation error.\n");
                return;
        }

        if (snake->head == NULL){
                snake->len = 1;
                snake->head = newNode;
                snake->tail = newNode;
        }
        else{
                snake->len ++;
                SNode_ptr current = snake->tail;
                snake->tail = newNode;
                newNode->prev = current;
                current->next = newNode;
        }
}

void popSNode(Snake_ptr snake){
        //删除最后一个节点
        SNode_ptr tail = snake->tail;
        SNode_ptr current = tail->prev;
        snake->tail = current;
	if (current != NULL)
		current->next = NULL;
        free(tail);
        snake->len--;
}

void deleteSnake(Snake_ptr snake){
        //释放内存
        int *len_ptr = &(snake->len);
        while (*len_ptr > 0)
                popSNode(snake);
        free(snake);
}

bool isInSnake(Snake_ptr snake, int x, int y){
	//不包括尾巴
	SNode_ptr head = snake->head;
	while (head->next != NULL){
		if (x == head->x && y == head->y)
			return true;
		head = head->next;
	}
	return false;
}

bool isDircInverse(Snake_ptr snake, enum Direction dirc){
	if (snake->dirc != dirc && (snake->dirc - dirc) % 2 ==0)
		return true;
	return false;
}

void next_snake(Snake_ptr snake, enum Direction dirc, bool eaten){
        //更新snake
        //eaten是否吃到食物，是否需要保留tail
        SNode_ptr head = snake->head;
        int x_head = head->x;
        int y_head = head->y;

        switch (dirc) {
                case UP:
                        appendSNode_head(snake, x_head-1, y_head, dirc);
                        break;
                case DOWN:
                        appendSNode_head(snake, x_head+1, y_head, dirc);
                        break;
                case LEFT:
                        appendSNode_head(snake, x_head, y_head-1, dirc);
                        break;
                case RIGHT:
                        appendSNode_head(snake, x_head, y_head+1, dirc);
                        break;
                default:
                        return;

        }
        if (!eaten){
                //去掉尾部
                popSNode(snake);
        }
}
