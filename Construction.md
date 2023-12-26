# 构建思路

## 数据类型

### Board底板

用二维数组`map`表示 ，因为`map`初始化要将边墙与内部空间区分开，初始化函数将`map`作为参数，二维数组作为参数有两种方式

```c
void init_map(int** map, int rows, int cols);
void init_map(int (*map)[CONST_COLS]); //第二个维度必须为常量
```

### 蛇

一般用链表作为结构，这里选用双向链表，原因是双向链表添加和删除尾部不用遍历，用内存空间换取时间。

* 优点
  * 删除尾部节点快
  * 添加方向和长度属性更为合理
* 缺点
  * 占用更多内存
  * 编写的内容更多，容易出错

```c
/* 双向链表 */
//方向枚举
enum Direction{LEFT = 0, RIGHT = 2, UP = 1, DOWN = 3};

// 双向链表的节点
typedef struct sNode{
	int x, y;
	struct sNode* next;
	struct sNode* prev;
} SNode, *SNode_ptr;

// snake结构
typedef struct snake{
	int len;
	enum Direction dirc;
	SNode_ptr head;
	SNode_ptr tail;
}Snake, *Snake_ptr;

// 实现函数
void appendSNode_head(Snake_ptr snake, int x, int y, enum Direction dirc);// 头部添加节点
void appendSNode_tail(Snake_ptr snake, int x, int y);	//尾部添加结点
void popSNode(Snake_ptr snake);	// 删除尾部节点
void deleteSnake(Snake_ptr snake);	// 清空内存
bool isInSnake(Snake_ptr snake, int x, int y);	// (x, y)是否在snake节点对象中，不包括最后一个节点，因为这是作为判断下一步的依据，有瑕疵,如果刚好吃到食物并碰到尾部本来也是应该结束游戏，实际没有
bool isDircInverse(Snake_ptr snake, enum Direction dirc); // 判断按键是否为蛇前进的反方向，为后续switch语句做准备，如果为反方向，方向保持不变，tips:main()中应该先保留先前的方向dirc_tmp
void next_snake(Snake_ptr snake, enum Direction dirc, bool eaten); 	//根据方向对下一步进行更新
```

