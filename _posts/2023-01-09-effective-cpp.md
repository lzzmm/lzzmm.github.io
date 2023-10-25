---
layout:     post
title:      Effective C++
subtitle:   55 Specific Ways to Improve Your Programs and Designs
date:       2023-01-09
author:     lzzmm
timecost:   30 minutes
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

2022年5月，同专业的关学姐毕业赠送此由 Scott Meyers 先生著、侯捷先生翻译的《Effective C++》，我在夏天时粗浅读过一遍，但没有留下笔记。好书是值得反复总结和阅读的。相较于市面上广泛存在的各种各样 C++ 丛书，此书在有一定基础的情况下可以让自己进一步上手规范的 C++。我认为读书不在多而在精，因此在这岁末之际，我想重读一遍此书并记下笔记，也作为自己真正使用 C++ 创造程序这一历程的开始。

***注意：此笔记中的文字虽有些摘抄但大部分是我自己的语言，样例代码相较书中也有些更改。如果在阅读中遇到问题或错误欢迎留言讨论。***

## Chapter 1 <br> Accustoming Yourself to C++

### 1. View C++ as a federation of languages

一直以来我使用的 C++ 都是所谓的 “C with class”。然而 C++ 是一个同时支持面向过程、面向对象、函数、泛型、元编程等形式的强大工具。这并不意味着我们必须在编写程序时同时考虑和使用这些特性，而是我们在充分理解这些特性之后根据实际情况选择合适的特性和功能编写程序。

本节将 C++ 分为四个主要的次语言，分别为：

1. **C** C++ 从 C 发源而来，没有模板、异常、重载，用 C 的写法在大多数情况下可以使用 C++ 的编译器实现 C 那样快速高效的程序。C++ 也可以与 C 混合编程。详见 [C/C++/x86asm 混合编程](https://lzzmm.github.io/2021/03/31/os-3.1/)。
2. **面向对象的 C++** 也就是熟悉的 C with class，和古老经典的面向对象守则：封装、继承、多态。
3. **Template C++** 这是我比较薄弱的点，因为少参与项目，日常写算法题做作业基本用不到模板，然而模板是非常强大，显著提高项目中的编码效率的存在，且带来崭新的模板元编程（TMP）。
4. **STL** 标准模板库提供规范的容器、迭代器、算法以及函数对象。详见[The Annotated STL Sources](https://lzzmm.github.io/2023/01/07/STL/)。

C++ 的高效编程守则取决于你使用 C++ 的哪一部分。

### 2. Prefer `const`, `enum`, `inline` to `#define`s

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

#### 关于 "the enum hack"

此行为某些方面来说比较像 `#define` 而非 `const`。如取一个 `const` 的地址是合法的（往往可以通过指针指来指去骗过编译器来修改 `const` 的值），但是取一个 `enum` 的地址同取一个 `#define` 的地址一样是不合法的。

此行为也是 TMP 的基础技术。

### 3. Use `const` whenever possible

面对指针，`const` 可以修饰指针自身或指针所指，如下所示。注意到 C 语言中字符串常量的本质表示其实是一个地址，`const` 在星号左边修饰被指物，与在类型左右无关，在星号右边修饰指针。

```cpp
char greeting[] = "hello";       // const pointer non-const data
char* p = greeting;              // non-const pointer and data
char const* p = greeting;        // non-const pointer const data
char* const p = greeting;        // const pointer non-const data
const char * const p = greeting; // const pointer and data
char const * const p = greeting; // const pointer and data
```

#### STL iterator

STL 迭代器作用类似于 `T*` 指针，声明一个迭代器为 `const` 类似于声明**指针**为 `const` (`T* const`)，若需要迭代器所指东西不可改动 (`const T *`)，则需要 `const_iterator`，如下。

```cpp
std::vector<int> vec;
const std::vector<int>::iterator it = vec.begin();   // T* const
*it = 10;   // correct
++it;       // error
std::vector<int>::const_iterator c_it = vec.begin(); // const T*
*c_it = 10; // error
++c_it;     // correct
```

#### 面对函数声明时

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

首先使用了 `static_cast` 为 `*this` 添加 `const`，这样才可以调用 `const operator[]`，然后使用了 `const_cast` 将返回值的 `const` 去掉。

注意，由 non-`const` 版本成员函数调用 `const` 版本成员函数的方法如上所述，但是反过来是不可以的，因为这违背了 `const` 成员函数不改变其对象逻辑状态的承诺。

### 4. Make sure that objects are initialized before they're used

使用 C part of C++ 且初始化可能招致运行期成本，那就不保证发生初始化。使用 non-C parts of C++ 时尽量保证初始化。如来自 C 的 `array` 不保证内容初始化而来自 STL 的 `vector` 保证初始化。

对于内置类型使用手工的方式初始化，而对其他东西使用构造函数对每个成员进行初始化。

C++ 规定对象的成员变量的初始化动作发生在进入构造函数本体之前，因此比起在构造函数中赋值（区分于初始化，因为初始化在这之前已经发生了），更应该做的是使用 **member initialization list （成员初值列）** 来进行初始化，这样效率更高。要 default 构造一个成员变量，只需要指定 nothing 作为初始化实参。规定总是在初值列中列出所有成员变量，若有许多构造函数，为了避免重复，可以将某些赋值表现像初始化一样好的成员变量放在某个私有函数中赋值然后此函数供诸多构造函数调用。

成员初始化次序：base classes 更早于其 derived classes 被初始化，类成员变量总以其声明次序被初始化。所以在成员初值列中最好以各成员声明的次序将其列出。

另一件需要操心的事情是，不同编译单元内定义之 non-local static 对象的初始化次序（我有点想直接读英文版了，这个中文翻译看着很烦）。

`static` 对象的寿命从被构造出来直到程序结束，包括 `global` 对象、定义于 `namespace` 作用域内的对象、在类中、在函数中、以及在 file 作用域内被声明为 `static` 的对象。函数内的 `static` 对象是 local static 对象，其他为 non-local static 对象，它们的析构函数在 `main()` 结束时调用。

问题在于，某编译单元里的 non-local static 对象初始化动作使用了另一编译单元内的某 non-local static 对象，而此时这个对象还未被初始化。C++ 没有明确定义不同编译单元中的 non-local static 对象的初始化次序，因为太难决定了。最常见的形式是多个编译单元内的 non-local static 对象经由 **模板隐式具现化 (implicit template instantiations)** 形成。

要解决此问题，需要把这些 non-local static 对象搬到自己专属的函数内变为 local static 对象，这些函数返回 reference，用户调用这些函数。这样能行是因为 C++ 保证 local static 对象会在函数被调用期间首次遇上该对象定义式时被初始化。这是 ***Signleton*** 模式的一个常见实现手法。

```cpp
class FileSystem { ... };
FileSystem& tfs() {
    static FileSystem fs;
    return fs;
}
class Dir { ... };
Dir::Dir(params) {
    ...
    // std::size_t disks = tfs.numDisks();
    std::size_t disks = tfs().numDisks();
    ...
}
```

## Chapter 2 <br> Constructors, Destructors, and Assignment Operators

构造函数和 copy assignment 操作符决定了对象的诞生，而析构函数决定了对象的消亡。

### 5. Know what functions C++ silently writes and calls

需要了解编译器会默默生成和调用啥东西。在这里主要介绍了类中的一些函数。

空类，编译器为它声明一个 copy 构造函数、一个 copy assignment 操作符、一个析构函数、一个 default 构造函数，所有这些均 `public` 且 `inline`。此析构函数在此类的基类声明有 virtual 析构函数时是 virtual 的，否则是 non-virtual 的。

C++ 不允许让 reference 改指不同的对象，若类内含 reference 成员或内含 `const` 成员，编译器拒绝生成 copy assignment 操作符。基类将 copy assignment 操作符声明为 `private` 则派生类也不能自动生成 copy assignment 操作符。

### 6. Expilicitly disallow the use of compiler-generated functions you do not want

如何阻止自动生成的 copy 构造函数和 copy assignment 操作符呢？将它们声明为 `private`，这样除了 member function 和 friend function 外都不可以调用，编译器也不会自动生成。进一步地，不去定义它们，这样一旦被调用则会产生一个 linkage error。这种方法在 iostream 中被使用。注意到，这样声明的函数不用写参数名称只需要写参数类型。

更进一步地，为了将链接期的错误移至编译期，我们在一个专门阻止 copying 的基类中声明 `private` 的 copy 构造函数和 copy assignment 操作符，只要继承这个基类那么便可以防止编译器自动生成 copy 构造函数和 copy assignment 操作符。

```cpp
class Uncopyable {
protected:
    Uncopyable();   // allow derived object construct and destruct
    ~Uncopyable();
private:
    Uncopyable(const Uncopyable&);
    Uncopyable& operator=(const Uncopyable&);
};

class T: private Uncopyable { ... };
```

### 7. Declare destructors virtual in polymorphic base classes

派生类对象经由基类指针被删除，若基类带 non-virtual 析构函数，则结果未定义，一般来讲是调用基类析构函数而不调用派生类析构函数。消除这个问题需要把基类的析构函数声明为 `virtual`。当类中含至少一个 virtual 函数时为它声明 virtual 析构函数。

声明了 virtual 函数的对象携带 **vptr (virtual table pointer)** 指向一个由函数指针构成的数组 **vtbl (virtual table)**，调用 virtual 函数时实际上编译器在虚表中寻找适当的函数指针进行调用。

抽象类声明 pure virtual 析构函数，必须为它提供一份定义。

并非被设计来经由基类接口处理派生类对象的类不需要虚析构函数。

### 8. Prevent exceptions from leaving destructors

析构函数抛出异常经常导致不明确行为或程序结束。若析构函数必须执行一个失败时会抛出异常的动作，必须阻止异常从析构函数传播出去，可以记下错误并调用 `std::abort()` 终止程序或者吞掉异常，通常后者比前者合理。更合理的做法是重新设计接口使得用户可以有机会在析构函数执行前手动执行某些动作，析构函数中判断此动作是否已经被执行，若否则再由析构函数调用并执行。

### 9. Never call virtual functions during construction or destruction

基类构造过程中虚函数不会下降到派生类层——此时派生类的变量尚未初始化哩！实际上在派生类对象的基类构造期间，RTTI 对象类型都是基类，虚函数也会被解析至基类。

正确的做法是在基类将虚函数改为非虚函数并要求派生类的构造函数传递必要信息给基类构造函数，之后便可以调用这个非虚函数。注意派生类中 `private static` 函数，这样更为可读且不可能指向未定义的其他成员变量。

```cpp
class Base {
public:
    explicit Base(const std::string& logInfo);
    void logBase(const std::string& logInfo) const;
    ...
};
Base::Base(const std::string& logInfo) {
    ...
    logBase(logInfo);
}
class Derived: public Base {
public:
    Derived( param ): Base(createLogString( param )) { ... }
    ...
private:
    static std::string createLogString( param );
};
```

### 10. Have assignment operators return a reference to `*this`

赋值采用右结合律，所以可以写成连锁赋值如

```cpp
int x, y, z;
x = y = z = 1;
```

相当于

```cpp
x = (y = (z = 1));
```

所以为了实现此操作，赋值相关运算（包括 `+=`, `-=`, `*=`, `/=` 等）最好返回一个指向操作符左侧实参的 reference，如

```cpp
class WIdget {
public:
    Widget& operator+=(const Widget& rhs) {
        ...
        return *this;
    }
    ...
};
```

### 11. Handle assignment to self in `operator=`

自我赋值，也就是对象赋值给自己。

证同测试（identity test）

```cpp
Widget& Widget::operator=(const Widget& rhs) {
    if (this == &rhs) return *this;  // 证同测试
    delete pb;                  // 删除原先的对象
    pb = new Bitmap(*rhs.pb);   // 拷贝构造新对象
    return *this;
}
```

上述代码不具备异常安全性，因为拷贝构造出现异常可能会使指针指向一个被释放的内存。下面实现一个异常安全的代码，对于自我赋值也可以处理，所以所谓证同测试现在不去实现它防止运行效率的降低。

```cpp
Widget& Widget::operator=(const Widget& rhs) {
    Bitmap* pOrig = pb;         // 记住原先的对象
    pb = new Bitmap(*rhs.pb);   // 拷贝构造新对象
    delete pOrig;               // 删除原先的对象
    return *this;
}
```

更进一步地，我们可以使用 **copy and swap** 来替代上述代码。

```cpp
class Widget {
    ...
    void swap(Widget& rhs);
    ...
};
Widget& Widget::operator=(const WIdget& rhs) {
    Widget temp(rhs);   // copy rhs
    swap(temp);         // swap *this and copy of rhs
    return *this;
}
```

另一种方法利用了 pass by value 方式会造成一份副本的原理在函数参数构造阶段实现复制。

```cpp
Widget& Widget::operator=(Widget rhs) {
    swap(rhs);
    return *this;
}
```

### 12. Copy all parts of an object

这一点是面向对象的基本。亮点在于作者比较幽默地把编译器对于没有实现完全深拷贝没有报错的行为理解为编译器对你自己声明 copying 函数而不用 default 实现的函数的报复。

**除了复制所有 local 变量** 之外值得注意的一点是派生类的 copying 函数要记得调用相应的基类函数**实现基类成分的 copy**，如在成员初值列中调用基类的 copy 构造函数等。

```cpp
Derived::Derived(const Derived& rhs)
    : Base(rhs),
      var(rhs.var),
      ...
{
    ...
}

Derived& Derived::operator=(const Derived& rhs) {
    Base::operator=(rhs);
    var = rhs.var;
    ...
    return *this;
}
```

若发现 copy 构造函数与 copy assignment 操作符有着近似相同的本体，消除重复代码的做法**不是**让他们调用另一个函数，而是建立一个新的 `private` 成员函数（一般命名为 `init`）供他们俩调用。

## Chapter 3 <br> Resource Management

资源就是用户向系统借来的东西——有借有还再借不难。除了动态分配内存，还有 file descriptors、 mutex locks、 GUI 中的字型和笔刷、数据库连接、网络 sockets。

### 13. Use objects to management resources

### 14. Think

## Chapter 4 <br> Designs and Declarations

### TBD
