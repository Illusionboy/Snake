# 贪吃蛇小游戏

这是一个使用C语言编写的简单贪吃蛇小游戏。游戏使用双向链表结构来表示贪吃蛇，并提供了一些基本操作函数。

## 文件结构

- `snake_game.c`: 主要代码文件，包含游戏的主要逻辑和入口点。
- `snake.h`: 定义了贪吃蛇的双向链表结构以及相关的属性和函数声明。
- `snake.c`: 包含一些对贪吃蛇进行操作的函数的实现。

## 使用方法

1. 克隆项目到本地:

    ```bash
    git clone https://github.com/your-username/snake-game.git
    ```

2. 编译代码:

    ```bash
    gcc snake_game.c snake.c -o snake_game
    ```

3. 运行游戏:

    ```bash
    ./snake_game
    ```

## 贡献

欢迎提出问题、报告bug或者提出改进建议。请在GitHub上创建一个Issue或者提交一个Pull Request。

## 注意事项

- 本项目仅作为学习用途，可能存在一些局限性和未处理的边界情况。

## 授权

该项目采用 [MIT License](LICENSE) 授权。

---
**注意**: 请确保在运行游戏之前已经安装了合适的编译器，并且项目的依赖已经被满足。