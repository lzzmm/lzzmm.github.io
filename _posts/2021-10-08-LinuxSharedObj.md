---
layout:     post
title:      Linux环境下构造动态链接库
subtitle:   以矩阵乘法函数为例
date:       2021-10-08
author:     炸毛
timecost:   10 mins
# header-style: black
# header-mask: 0.01
# header-img-credit:      Yuhan Chen
# header-img-year:        2019 
header-img-outchain:    true
header-img: https://s1.ax1x.com/2020/03/16/8JDyPP.jpg
# nav-style: invert
catalog: true
mathjax: true
live2d:  false
do-not-show-in-index: false
tags:
    - C/C++
    - Linux
---

# 动态链接库

即使一个非常简单的程序，也需要依赖C标准库和系统库。**链接**其实就是把其他第三方库和自己源代码生成的二进制目标文件融合在一起的过程。经过链接之后，那些第三方库中定义的函数就能被调用执行了。静态链接在链接的时候，把所依赖的第三方库函数都打包到了一起，导致最终的可执行文件非常大。而动态链接在链接的时候并不将那些库文件直接拿过来，而是在运行时，发现用到某些库中的某些函数时，再从这些第三方库中读取自己所需的方法。

无论何种操作系统上，使用动态链接生成的目标文件中凡是涉及第三方库的函数调用都是**地址无关**的。在生成的目标文件中，不会立即确定函数具体地址，而是在运行时去装载这个函数，在装载时，装载器根据当前地址空间情况，动态地分配一块虚拟地址空间，在装载阶段确定此函数的地址。

不同操作系统的动态链接库文件格式稍有不同，Linux称之为共享目标文件（Shared Object），文件后缀为`.so`，Windows的动态链接库（Dynamic Link Library）文件后缀为`.dll`。

`so`文件后面往往跟着很多数字，这表示了不同的版本。`so`文件命名规则被称为SONAME：

```text
libname.so.x.y.z
```

lib是前缀，这是一个约定俗成的规则。x为主版本号（Major Version），y为次版本号（Minor Version），z为发布版本号（Release Version）。

动态链接库的查找先后顺序为：

- `LD_LIBRARY_PATH`环境变量中的路径
- `/etc/ld.so.cache`缓存文件
- `/usr/lib`和`/lib`

使用GCC编译链接时，有两个参数需要注意，一个是`-l`（小写的L），一个是`-L`（大写的L）。我们前面曾提到，Linux有个约定俗成的规则，假如库名是name，那么动态链接库文件名就是`libname.so`。在使用GCC编译链接时，`-lname`来告诉GCC使用哪个库。链接时，GCC的链接器`ld`就会前往`LD_LIBRARY_PATH`环境变量、`/etc/ld.so.cache`缓存文件和`/usr/lib`和`/lib`目录下去查找`libname.so`。我们也可以用`-L/path/to/library`的方式，让链接器`ld`去`/path/to/library`路径下去找库文件。

# 编写 `matrix_multiply.h` 和 `matrix_multiply.c` 

根据要求，我将Lab1的函数移植过来，其中分为单精度浮点和双精度浮点两个函数。本来打算参数是三个矩阵，但是找不到查询指针所指内存块的大小的方法，所以只能把矩阵大小也传入了。并且需要矩阵行列都是8的倍数。

若是MSVC编译器则可使用 **_msize** returns the size (in bytes) as an unsigned integer.

```c
size_t _msize(
   void *memblock
);
```

https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/msize?redirectedfrom=MSDN&view=msvc-160

这不是标准函数，在我的环境下不适用。

```c
#ifndef __MATRIX_MULTIPLY__
#define __MATRIX_MULTIPLY__

#ifdef __cplusplus
extern "C" {
#endif

void matrix_multiply_d(int M, int N, int K, double **A, double **B, double **C);
void matrix_multiply_f(int M, int N, int K, float **A, float **B, float **C);

#ifdef __cplusplus
}
#endif

#endif
```

.h文件中声明两个函数，增加了混合编程确保对g++的兼容。

.c文件中则是这两个函数的实现，这里不再展开详谈。

那么，如何编译生产共享库文件呢？

# 编译.so文件

注意以下几点：

1. 编译时gcc 后加 -fPIC ，这可以使 gcc 产生与位置无关的代码
2. 链接时，使用-shared，指示生成一个共享库文件
3. 共享库文件按照上面讲的约定俗成命名法lib开头+扩展名.so

编译链接命令如下：

```bash
g++ -Wall -g -fPIC -mavx -o matrix_multiply.o -c matrix_multiply.c
g++ -shared -o libmatrix_multiply.so matrix_multiply.o
```

可以看到生成了.o和.so文件。

![image-20211008231006292](img/19335025_%E9%99%88%E7%A6%B9%E7%BF%B0_lab2/image-20211008231006292.png)

# 调用.so文件

我们先在`.bash_profile`中添加

```bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
```

然后在 `main.cpp` 中引用头文件便可调用函数：

```cpp
#include "matrix_multiply.h"
...
int main() {
    ...
    matrix_multiply_f(M_8x, N_8x, K_8x, A, B, C);
    ...
}
```

编译链接：

```bash
g++ -Wall -g -o a.o -c main.cpp
g++ -o a a.o -L. -lmatrix_multiply
```

1. -L.：在当前路径寻找so文件
2. -lmatrix_multiply: 链接libmatrix_multiply.so这个库文件

![image-20211008231743882](img/19335025_%E9%99%88%E7%A6%B9%E7%BF%B0_lab2/image-20211008231743882.png)

可以看到运行结果没有问题。

Makefile 内容如下：

```makefile
.SUFFIXES:.cpp .o
CC = g++
# SRCS = matrix_multiply.cpp
# OBJS = $(SRCS: .cpp=.o)
SO = libmatrix_multiply.so

$(SO): matrix_multiply.o
	$(CC) -shared -o $(SO) matrix_multiply.o
matrix_multiply.o: matrix_multiply.c matrix_multiply.h
	$(CC) -Wall -g -fPIC -mavx -o matrix_multiply.o -c matrix_multiply.c

build_main:
	$(CC) -Wall -g -o a.o -c main.cpp
	$(CC) -o a a.o -L. -lmatrix_multiply
clean:
	rm -f matrix_multiply.o a.o
	rm -f core*
```

# 存在的问题

1. 需要传入矩阵大小作为参数（采用数组或者结构体传入或许好看许多）
2. 矩阵行列大小需要满足某些倍数（可否通过在函数中realloc，计算完后再返回左上角呢？）

#  REFERENCES

[浅析Linux动态链接库](https://zhuanlan.zhihu.com/p/235551437)