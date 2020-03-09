---
layout:     post
title:      My Code Style
subtitle:   大括号换不换行？
date:       2020-02-29
author:     Programmer ZHAMAO
header-img: img/home-bg.jpg
catalog: true
mathjax: true
live2d:  true
tags:
    - C
    - Coding
---

#### C
- 大括号不换行但是空一格
- 4个空格Tab
- if、for后有一个空格
- 运算符旁空格，作为下标时没有，指针运算符也没有
- 命名大概是喜欢下划线也喜欢驼峰
<details>
  <summary> 展开查看代码 </summary>

  ```c
    #include <stdio.h>
    #define maxm 100001
    int main() {
        int n, cnt[maxm];
        for (;;);
        if (condition) {
            printf("helloworld\n");
        } else if (condition) {
            printf("你好\n");
            while (1) {
                if (scanf("%d", &n) == 1) printf("True\n");
                else printf("False\n");
            }
        }
        cnt[maxm-n] = cnt[n+1] + cnt[n%4] + n;
    }
  ```

</details>

To be continue;