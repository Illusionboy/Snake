#define WIDTH_MAP 50
#define HIGHT_MAP 30

enum Direction{LEFT = 0, RIGHT = 2, UP = 1, DOWN = 3};

typedef struct sNode{
	int x, y;
	struct sNode* next;
	struct sNode* prev;
} SNode, *SNode_ptr;

typedef struct snake{
	int len;
	enum Direction dirc;
	SNode_ptr head;
	SNode_ptr tail;
}Snake, *Snake_ptr;

void appendSNode_head(Snake_ptr snake, int x, int y, enum Direction dirc);
void appendSNode_tail(Snake_ptr snake, int x, int y);
void popSNode(Snake_ptr snake);
void deleteSnake(Snake_ptr snake);
bool isInSnake(Snake_ptr snake, int x, int y);
bool isDircInverse(Snake_ptr snake, enum Direction dirc);
void next_snake(Snake_ptr snake, enum Direction dirc, bool eaten);

