---
layout:     post
title:      Effective C++
subtitle:   55 Specific Ways to Improve Your Programs and Designs
date:       2023-01-09
author:     炸毛
timecost:   10 minutes
# header-style: black
# header-style: text
# header-mask: 0.01
header-img-credit:      CHEN Yuhan
header-img-year:        2022
header-img-outchain: false
header-img: img/bg/goose.jpg
heading-font: CamingoCodeRegular
# nav-style: invert
catalog: true
mathjax: true
live2d:  false
byncsa:  true
do-not-show-in-index: false
tags:
    - C/C++
    - Notes
---

## 前言

22年5月，同专业的关学姐毕业赠送此由侯捷翻译的《Effective C++》，我在夏天时粗浅读过一遍，但没有留下笔记。好书是值得反复总结和阅读的。相较于市面上广泛存在的各种各样 C++ 丛书，此书在有一定基础的情况下已经足够让自己进一步上手规范的 C++ 了。我认为读书不在多而在精，因此在这岁末之际，我想重读一遍此书并记下笔记，也作为自己真正使用 C++ 创造程序这一历程的开始。

## 1. View C++ as a federation of languages

一直以来我使用的 C++ 都是所谓的 “C with class”。然而 C++ 是一个同时支持面向过程、面向对象、函数、泛型、元编程等形式的强大工具。这并不意味着我们必须在编写程序时同时考虑和使用这些特性，而是我们在充分理解这些特性之后根据实际情况选择合适的特性和功能编写程序。

本节将 C++ 分为四个主要的次语言，分别为：

1. **C** C++ 从 C 发源而来，没有模板、异常、重载，用 C 的写法在大多数情况下可以使用 C++ 的编译器实现 C 那样快速高效的程序。C++ 也可以与 C 混合编程。详见 [C/C++/x86asm 混合编程](https://lzzmm.github.io/2021/03/31/os-3.1/)。
2. **面向对象的 C++** 也就是熟悉的 C with class，和古老经典的面向对象守则：封装、继承、多态。
3. **Template C++** 这是我比较薄弱的点，因为少参与项目，日常写算法题做作业基本用不到模板，然而模板是非常强大，显著提高项目中的编码效率的存在，且带来崭新的模板元编程（TMP）。
4. **STL** 标准模板库提供规范的容器、迭代器、算法以及函数对象。详见[TBD]()。

C++ 的高效编程守则取决于你使用 C++ 的哪一部分。

## 2. Prefer `const`, `enum`, `inline` to `#define`s

核心思想是用编译器替代预处理器。

宏定义的常量不会进入符号表，因此不利于 debug。且宏定义的常量不具备封装性，最好使用 `const` 或 `enums` 替换。

形似函数的宏定义，不会招致函数调用带来的开销，个中宏实参必须带小括号。最好使用 `inline` (suggested `template inline` function in book precisely) 来替换，避免发生一些奇怪的错误。

提到了两种特殊情况。

1. `const std::string` 优于 `const char* const`
2. 类专属常量可以在声明中赋初值，此时定义中不可以再设初值。某些编译器不支持上述语法，因此可以将初值放在定义式，但若在类编译期间需要一个类常量，如数组大小，此时可以使用 "the enum hack"，如下所示
  
    ```cpp
    class T {
    private:
        enum { NumTurns = 5 };
        int scores[NumTurns];
    }
    ```

### 关于 "the enum hack"

此行为某些方面来说比较像 `#define` 而非 `const`。如取一个 `const` 的地址是合法的（往往可以通过指针指来指去骗过编译器来修改 `const` 的值），但是取一个 `enum` 的地址同取一个 `#define` 的地址一样是不合法的。

此行为也是 TMP 的基础技术。

## 3. Use `const` whenever possible

面对指针，`const` 可以修饰指针自身或指针所指，如下所示。注意到 C 语言中字符串常量的本质表示其实是一个地址，`const` 在星号左边修饰被指物，与在类型左右无关，在星号右边修饰指针。

```cpp
char greeting[] = "hello";       // non-const pointer and data
char* p = greeting;              // non-const pointer const data
char* const p = greeting;        // const pointer non-const data
const char * const p = greeting; // const pointer and data
char const * const p = greeting; // const pointer and data
```

### STL iterator

STL 迭代器作用类似于 `T*` 指针，声明一个迭代器为 `const` 类似于声明指针为 `const` (`T* const`)，若需要迭代器所指东西不可改动 (`const T *`)，则需要 `const_iterator`，如下。

```cpp
std::vector<int> vec;
const std::vector<int>::iterator it = vec.begin();   // T* const
*it = 10;   // correct
++it;       // error
std::vector<int>::const_iterator c_it = vec.begin(); // const T*
*c_it = 10; // error
++c_it;     // correct
```

### 面对函数声明时

令函数返回一个常量值，在诸如 `operator*` 中可以避免对 `*` 操作的结果赋值。

令参数为常量，可以避免函数中对参数的不小心的修改。

`const` 修饰成员函数，有俩流行概念：**bitwise constness (physical constness)** and **logical constness**。前者相信 `const` 成员函数不可以更改对象内任何 non-static 成员变量。但是许多成员函数虽然不十足具备 `const` 性质但却可以通过 bitwise 测试，如成员变量是个指针，而 `const` 成员函数可以修改此指针指向的数据。

```cpp
class CTextBook {
public:
    char& operator[] (std::size_t pos) const { return pText[pos]; }
private:
    char* pText;
};

const CTextBook cctb("hello");
char* pc = &cctb[0];    // gets a ptr
*pc = 'k';              // "kello"
```

后者的主张则是一个 `const` 成员函数可以在客户端检测不出的情况下修改所处理的对象的某些位。编译器不同意修改的时候我们使用 `mutable` 关键字来修饰 non-static 成员使得它的 bitwise constness 约束得以释放。

```cpp
class CTextBook {
public:
    std::size_t length() const;
private:
    char* pText;
    mutable std::size_t textLength;
    mutable bool  lengthIsValid;
}

std::size_t CTextBook::length() const {
    if(!lengthIsValid) {
        textLength = std::strlen(pText); // correct
        lengthIsValid = true;            // also correct
    }
    return textLength;
}
```

然而，若把许多功能都同时放进 `const` 和 non-`const` 版本的成员函数中，代码会有很大的重复。所以在 `const` 成员函数做掉 non-`const` 版本的成员函数所应该做的一切而唯一不同只是返回类型多了个 `const` 修饰时，我们的 non-`const` 版本的成员函数只需要调用 `const` 成员函数并对返回值进行 casting 即可。

```cpp
class TextBook {
public:
    const char& operator[](std::size_t pos) const {
        ...
        return text[pos];
    }
    char& operator[](std::size_t pos) {
        return const_cast<char&>(
            static_cast<const TextBook&>(*this)[pos] // call const op[]
        );
    }
}

```

首先使用了 `static_cast` 为 `*this` 添加 `const`，这样才可以调用 `const operator[]`，然后将返回值的 `const` 去掉，使用了 `const_cast`。

注意，由 non-`const` 版本成员函数调用 `const` 版本成员函数的方法如上所述，但是反过来是不可以的，因为这违背了 `const` 成员函数不改变其对象逻辑状态的承诺。

## 4. Make sure that objects are initialized before they're used

## 5. Know what functions C++ sliently writes and calls

## 6. Expilicitly disallow the use of compiler-generated functions you do not want

## 7. Declare destructors virtual in polymorphic base classes

## TBD
