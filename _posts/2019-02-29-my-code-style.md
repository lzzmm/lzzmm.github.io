---
layout:     post
title:      Code Style
subtitle:   And how to change the format style on Visual Studio Code
date:       2019-02-29
author:     Programmer ZHAMAO
timecost:   1 minute
header-img: img/home-bg.jpg
# nav-style: invert
catalog: true
mathjax: true
live2d:  true
tags:
---

#### C
- BasedOnStyle: LLVM,
- UseTab: Never,
- IndentWidth: 4,
- TabWidth: 4,
- AllowShortIfStatementsOnASingleLine: true,
- AllowShortLoopsOnASingleLine: true,
- IndentCaseLabels: true,
- PointerAlignment: Right,
- ColumnLimit: 0,
- AccessModifierOffset: -4
- 大括号不换行但是空一格
- 4个空格缩进
- if、for后有一个空格
- 运算符旁空格，指针运算符也没有
- 命名大概是喜欢下划线也喜欢驼峰



## Visual Studio Code Clang Format Settings

in VS Code settings search `clang`, set C_CPP:Clang_format_fallback Style.  
in Visual Studio Code, currently supports: Visual Studio, LLVM, Google, Chromium, Mozilla, WebKit.   
or Use "file" to load the style from a .clang-format file in the current or parent directory.

###### 基于哪个配置文件
BasedOnStyle: LLVM
###### 访问说明符的偏移(public private)
AccessModifierOffset: -4
###### 括号之后,水平对齐参数: Align DontAlign AlwaysBreak
AlignAfterOpenBracket: Align
###### 连续的赋值时,对齐所有的等号
AlignConsecutiveAssignments: true
###### 连续声明时，对齐所有声明的变量名
AlignConsecutiveDeclarations: true
###### 左对齐换行(使用反斜杠换行)的反斜杠 
AlignEscapedNewlinesLeft: true
###### 水平对齐二元和三元表达式的操作数 
AlignOperands: true
###### 对齐连续的尾随的注释  
AlignTrailingComments: true
###### 允许函数声明的所有参数在放在下一行  
AllowAllParametersOfDeclarationOnNextLine: true
###### 允许短的块放在同一行  
AllowShortBlocksOnASingleLine : false
###### 允许短的case标签放在同一行
AllowShortCaseLabelsOnASingleLine: false
###### 允许短的函数放在同一行: None, InlineOnly(定义在类中), Empty(空函数), Inline(定义在类中，空函数), All 
AllowShortFunctionsOnASingleLine: Empty
###### 是否允许短if单行 If true, if (a) return; 可以放到同一行
AllowShortIfStatementsOnASingleLine: false
###### 允许短的循环保持在同一行   
AllowShortLoopsOnASingleLine: false 
###### 总是在定义返回类型后换行(deprecated)   
AlwaysBreakAfterDefinitionReturnType: None
###### 每行字符的限制，0表示没有限制  
ColumnLimit: 100
###### 描述具有特殊意义的注释的正则表达式，它不应该被分割为多行或以其它方式改变
CommentPragmas: '^ IWYU pragma:'
###### 语言: None Cpp Java Objc Protp
Language: Cpp 
###### 指针的*的挨着哪边
PointerAlignment: Right
###### 缩进宽度
IndentWidth: 4
###### 连续的空行保留几行
MaxEmptyLinesToKeep: 1
###### 在 @property 后面添加空格, \@property (readonly) 而不是 \@property(readonly).
ObjCSpaceAfterProperty: true
###### OC block后面的缩进
ObjCBlockIndentWidth: 4
###### 是否允许短方法单行
AllowShortFunctionsOnASingleLine: false
###### 换行的时候对齐操作符
AlignOperands: true
###### 中括号两边空格 [] 
SpacesInSquareBrackets: true
###### 小括号两边添加空格
SpacesInParentheses : false
###### 等号两边的空格
SpaceBeforeAssignmentOperators: true
###### 容器类的空格 例如 OC的字典
SpacesInContainerLiterals: true
###### 缩进
IndentWrappedFunctionNames: true
###### 在block从空行开始
KeepEmptyLinesAtTheStartOfBlocks: true
###### 在构造函数初始化时按逗号断行，并以冒号对齐
BreakConstructorInitializersBeforeComma: true
###### 括号后添加空格
SpaceAfterCStyleCast: false
###### 允许排序#include, 造成编译错误
SortIncludes: true 
###### 缩进case 标签
IndentCaseLabels: true 
###### tab键盘的宽度
TabWidth: 4
UseTab: Never




```c
#include <stdio.h>
#define maxm 100001
int main() {
    int n, cnt[maxm];
    for (; ; );
    if (condition) {
        printf("helloworld\n");
    } else if (condition) {
        printf("你好\n");
        while (1) {
            if (scanf("%d", &n) ==1) printf("True\n");
            else printf("False\n");
        }
    }
    cnt[maxm - n] = cnt[n + 1] + cn[n % 4] + n;
}
```


To be continue;