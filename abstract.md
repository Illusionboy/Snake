# ncurses库的`timeout`函数

在`ncurses`库中，`timeout`函数用于设置输入函数（如`getch`）的阻塞时间。它可以使输入函数在等待用户输入时，最多阻塞指定的时间，然后立即返回，而不会一直等待用户输入。如果在指定时间内没有输入，输入函数会返回`ERR`。

`timeout`函数的原型如下：

```c
int timeout(int delay);
```

参数`delay`是以毫秒为单位的时间延迟。当设置了`timeout`之后，后续调用`getch`等输入函数时，如果没有输入字符，它会等待指定的时间`delay`，然后立即返回。如果在指定时间内有输入字符，则输入函数会立即返回该字符。

如果您在`ncurses`程序中设置了`timeout`函数，并且未捕获到任何输入，那么输入函数将在等待指定时间后返回。这种行为和使用`sleep`函数类似，但有一点不同：`timeout`函数只会阻塞等待用户输入的时间，而不会暂停程序的执行，因此程序在等待用户输入的同时仍然可以执行其他操作。而`sleep`函数会完全暂停程序的执行，直到指定的睡眠时间结束。

总结来说，`timeout`函数是为`ncurses`库中的输入函数设置阻塞时间的功能，使得输入函数在等待用户输入时最多阻塞指定的时间，然后立即返回，这使得它具有类似于`sleep`函数的效果，但不会完全暂停程序的执行。

可以通过多线程`pthread.h`和`termios.h`库实现，`pthread.h`提供多线程支持和互斥锁，`termios.h`可以改变当前终端的标准输入，当终端检测到键盘输入时**立即捕获**字符值。

```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>

// 全局变量表示从标准输入获取的字符
char capturedChar = '\0';

// 锁用于保护全局变量
pthread_mutex_t mutex;

// 获取字符的线程函数，!*传入线程pthread_create的函数参数应定义原型为函数指针*!
void* getInput(void* arg) {
    // 将终端设置为非阻塞模式
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    //ICANON为标准输入行模式
    /*ECHO为回显，指当键盘输入字符时，显示在终端上
    */
    term.c_lflag &= ~(ICANON | ECHO);	//关闭标准输入模式和回显，以便在输入时立即返回字符。
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    while (1) {
        char ch = getchar();

        // 获取互斥锁，保护全局变量
        pthread_mutex_lock(&mutex);

        // 将读取的字符保存在全局变量中
        capturedChar = ch;

        // 释放互斥锁
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// 获取阻塞时间内的字符
char getchWithTimeout(int timeoutMillis) {
    char ch;

    // 获取互斥锁，保护全局变量
    pthread_mutex_lock(&mutex);

    // 在指定的超时时间内等待字符
    int sleepTime = 10; // 每次休眠10毫秒
    int totalSleepTime = 0;
    while (capturedChar == '\0' && totalSleepTime < timeoutMillis) {
        pthread_mutex_unlock(&mutex);
        usleep(sleepTime * 1000); // 休眠指定的毫秒数
        totalSleepTime += sleepTime;
        pthread_mutex_lock(&mutex);
    }

    // 获取到字符后，从全局变量中读取，并清空全局变量
    ch = capturedChar;
    capturedChar = '\0';

    // 释放互斥锁
    pthread_mutex_unlock(&mutex);

    return ch;
}

int main() {
    // 初始化互斥锁
    pthread_mutex_init(&mutex, NULL);

    // 创建线程，用于从标准输入中获取字符
    pthread_t thread;
    pthread_create(&thread, NULL, getInput, NULL);

    while (1) {
        printf("Waiting for input...\n");
        char ch = getchWithTimeout(2000); // 阻塞等待2秒
        if (ch != '\0') {
            printf("Captured char: %c\n", ch);
        } else {
            printf("Timeout! No input.\n");
        }
    }

    // 销毁互斥锁
    pthread_mutex_destroy(&mutex);

    return 0;
}

```

* 传入线程`pthread_create`的函数参数应定义原型为函数指针

# 多维数组作为函数参数

建议动态生成多维数组，并将每个维度的长度作为 函数参数

例：二维数组

```c
//分配内存
short** map = (short**)malloc(rows * sizeof(short*));
for (int i = 0; i < h; i++)
    map[i] = (short*)calloc(cols, sizeof(short));
init_map(map);
//函数调用，参数为二维数组
draw_map(map, rows, cols);

//...

//释放内存
for (int i = 0; i < rows; i++)
    free(map[i]);
free(map);
return 0;
```



