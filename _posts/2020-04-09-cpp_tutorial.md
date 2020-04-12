---
layout:     post
title:      Object-Oriented Programming!
subtitle:   Learning C++
date:       2020-04-09
author:     蒟蒻炸毛
timecost:   5 minutes
# header-style: black
# header-mask: 0.01
# header-img-credit:      Yuhan Chen
# header-img-year:        2019 
header-img-outchain:    true
header-img: https://s1.ax1x.com/2020/03/16/88O1SO.jpg
catalog: true
mathjax: true
live2d:  false
tags:
    - C++
    - Matrix
---

# Week1 Basic iostream

使用`<iostream>`以及`<iomanip>`库操纵输入输出流。

输入一行，包含一个整数$n$，一个字符$c$，一个整数$w$。

输出这个整数$n$，要求控制输出宽度为$w$，左对齐，空白部分用字符$c$填充，且$n$为非负整数时显示$+$号。

保证$n$的宽度小于$w$。

#### Input

```
4 c 6
```

```
-45 u 8
```

#### Output

```
+4cccc
```

```
-45uuuuu
```

#### C++ code

注释掉的是我一开始写的，也能过，但是不美观。
```cpp
#include <iomanip>
#include <iostream>
using namespace std;

int main() {
    int n, w;
    char c;

    cin >> n >> c >> w;
    cout << left << setfill(c) << setw(w) << showpos << n << endl;
    // if (n >= 0)
    //     cout << '+' << setfill(c) << setw(w - 1) << left << n << endl;
    // else
    //     cout << setfill(c) << setw(w) << left << n << endl;

    return 0;
}
```


# Week1 Create Array

使用C++中的关键字`new`和`delete`创建和回收数组。

实现以下四个函数：

```c++
int* Array(int len);
int** Reshape(int* vec, int row, int col);
void FreeArray(int* vec);
void FreeMatrix(int** mat, int row);
```

其中，`Array`函数创建一个长度为`len`的一维数组；`Reshape`函数将上述数组变为一个`row`$\times$`col`的矩阵，题目数据保证`row`$\times$`col`$=$`len`。

已禁用`stdlib.h`，请勿使用`malloc`及`free`。

#### Input

```
6
1 2 3 4 5 6
2 3
```

### Output

```
1 2 3
4 5 6
```

将长度为$6$的向量reshape为$2 \times 3$的矩阵。

#### Provided: main.cpp

```cpp
// #include "check.h"
// #include <iostream>
#include "functions.h"
#include <stdio.h>

int main() {
    int *vec = nullptr;
    int len, row, col;
    scanf("%d", &len);
    vec = Array(len);
    for (int i = 0; i < len; ++i) {
        scanf("%d", vec + i);
    }
    int **mat = nullptr;
    scanf("%d%d", &row, &col);
    mat = Reshape(vec, row, col);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            printf("%d%c", mat[i][j], j == col - 1 ? '\n' : ' ');
        }
    }
    FreeArray(vec);
    FreeMatrix(mat, row);
    return 0;
}
```


#### Provided: functions.h

```cpp
#ifndef FUNCTION_H
#define FUNCTION_H

int* Array(int len);							
int** Reshape(int* vec, int row, int col);	
void FreeArray(int* vec);
void FreeMatrix(int** mat, int row);

#endif
```


#### Answer: functions.cpp
以下是我原来写的。

```cpp
int *Array(int len) {
    int *array = new int[len];
    return array;
}
int **Reshape(int *vec, int row, int col) {
    int k = 0;
    int **matrix = new int *[row];
    for (int i = 0; i < row; i++) {
        matrix[i] = new int[col];
        for (int j = 0; j < col; j++) {
            matrix[i][j] = vec[k++];
        }
    }
    return matrix;
}
void FreeArray(int *vec) {
    delete[] vec;
}
void FreeMatrix(int **mat, int row) {
    for (int i = 0; i < row; i++) {
        delete[] mat[i];
    }
    delete[] mat;
}
```
其中的 `reshape`函数也可以如下实现。

```cpp
int **Reshape(int *vec, int row, int col) {
    int **matrix = new int *[row];
    for (int i = 0; i < row; i++) {
        matrix[i] = new int[col];
        for (int j = 0; j < col; j++) {
            matrix[i][j] = vec[i*col + j];
        }
    }
    return matrix;
}
```

# Week1 Call by Reference

引用传递函数参数是C++区别于C的一大特性，可看作是指针的语法糖。通过引用传递参数主要有以下两个好处：

- 减少内存开销

设想有一个庞大的自定义结构体，当你想要将其作为函数参数时，若直接传入函数：

```c++
void f(struct HugeStruct hugestruct);
```

实际上是进行了将实参赋值给形参这样的内存拷贝，对于`int`，`bool`这样的类型来说可以忽略，但对于某些庞大的结构来说，这样的内存开销是不可接受的；

- 多个返回值

C++中的函数不支持多个返回值，假设你的函数想返回多个值，一种可行的解决方法是返回一个结构体，但某些情况下这样做可能不太优雅，因此可以通过使用引用作为函数参数，来支持多值返回。

```c++
void g(int x, int& y1, int& y2); // y1, y2是返回值
```



请同学们自行查阅相关资料，完成以下两个函数：

```c++
void addsub(int x, int y, int& sum, int& diff); // 将x, y相加结果储存在sum中，相减结果储存在diff中
void swap(int& x, int& y); // 交换x, y的值
```
<details>
<summary>Provided: main.cpp</summary>

```cpp
#include <iostream>
#include "functions.h"
using std::cin;
using std::cout;
using std::endl;

int main() {
    int x, y, sum, diff;
    cin >> x >> y;
    addsub(x, y, sum, diff);
    cout << sum << '\t' << diff << endl;
    swap(x, y);
    cout << x << '\t' << y << endl;
    swap(sum, diff);
    cout << sum << '\t' << diff << endl;
    return 0;
}
```
</details>

<details>
<summary>Provided: functions.h</summary>

```cpp
#ifndef FUNCTION_H
#define FUNCTION_H

void swap(int&, int&);
void addsub(int, int, int&, int&);

#endif
```
</details>

<details>
<summary>Answer: functions.cpp</summary>

```cpp
// 将x, y相加结果储存在sum中，相减结果储存在diff中
void addsub(int x, int y, int &sum, int &diff) {
    sum = x + y;
    diff = x - y;
}

// 交换x, y的值
void swap(int &x, int &y) {
    int tmp;
    tmp = x;
    x = y;
    y = tmp;
}
```
</details>
