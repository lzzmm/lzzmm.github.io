---
layout:     post
title:      Matrix OJ!
subtitle:   Simple problems in a page
date:       2020-03-04
author:     Traditial ZHAMAO
# header-style: black
header-mask: 1
header-img-outchain: true
header-img: https://s1.ax1x.com/2020/03/15/833RbQ.jpg
catalog: true
mathjax: true
live2d:  true
tags:
    - C
    - Matrix
---

# Day11 异世界六部曲（一）异世界的街道 
### 背景故事
> *这题的出题人正在看番，忽然天空中传来伟大的会长的声音：“在？有空吗？过来出个题？”*  
> *出题人悲伤地关掉 B 站，准备去便利店买点零食再工作。晕乎乎地从便利店出来，出题人突然发现自己到了个不知道什么地方。这里是中世纪的建筑风格，人们也穿着中世纪的衣服，巨大的蜥蜴拉着车在街上跑，一个卖苹果的大叔喊你：“喂，不买苹果别挡着我做生意！”*  
> *出题人突然醒悟：我被召唤到异世界了！！*  
> *初到异世界，出题人很兴奋，想要试试自己获得了什么超能力。然而试了半天，他悲哀地发现自己啥都不会，还遇到了混混。*  

### 题目描述
> 异世界的街道里，共有 $n$ 个混混，$m$ 个平民，以及出题人。大家都在随机游走，其中每分钟:
> 1. 恰有两个人遇到。
> 2. 任意两个人之间相遇的概率是均等的。  
> 如果两个平民相遇，没有事情发生；如果两个混混相遇，他们会互相厮打，最终一起死亡；如果混混遇到平民，混混就会杀掉平民；如果混混遇到出题人，哈哈。。。。出题人的死亡回归能力还没觉醒，要客死他乡了；如果出题人遇到平民，那么出题人会等概率选择杀掉他或不杀掉他。  
> 问出题人活下来去见艾米莉亚的概率。

### 输入格式
> 一行，两个整数，$n$ 和 $m$。($0 \leq n,m \leq 1000$)

### 输出格式
> 一行，一个浮点数（保留 4 位小数），表示出题人存活的概率。

### 题解
好沙雕(￣▽￣)"  
一开始可能觉得摸不着头脑然后去递推，但是其实这道题属于那种刻意添加无关元素的题。
仔细思考会发现平民和出题人（下简称486）的生死并没有什么关系=-=那我们排除平民来看混混，如果混混是奇数只，那肯定到最后至少剩一只，于是486必死；如果是0个那486必活；如果是偶数只，那486碰到n个混混的概率就是`n/n+1`，存货概率`1/n`。
```c
#include <stdio.h>
int main() {
    int n,m;
    scanf("%d%d",&n,&m);            
    if (n == 0) {
        printf("1.0000\n");
    } else if (n%2 != 0) {
        printf("0.0000\n");
    } else {
        printf("%.4f",1.0/(n+1));
    }
    return 0;
}
```


# Day11 异世界六部曲（二）罗姆的常系数齐次线性递推

### 背景故事

*出题人遇到了被偷了徽章的艾米莉亚，他们一起寻找徽章，最后锁定在一家赃物店里，却没有想到，两人相继进店，相继被杀。*

*出题人发现自己拥有死亡回归的能力，读档复活了。出题人又遇见了艾米莉亚，然而艾米莉亚却一副不认识出题人的样子，出题人一脸茫然。*

*不知所措的出题人决定先去店里赎回徽章。来到店里，这里只有罗姆老爷爷，和一个小女孩。*

*罗姆：“你拿什么来赎？”*

### 题目描述

出题人：“当当当当！看这个手机，它有很神奇的能力，它能算递推数列哦！”

罗姆：“递推数列？这个老夫也会，老夫识得斐波那契数列，就是 $f _ 1=1,~f _ 2=1,~f _ n=f _ {n-1}+f _ {n-2}$，这个数列是 $1,1,2,3,5,8,13\cdots$”

出题人：“很好。常系数齐次线性递推就是一种扩展，定义 $f _ 1=a _ 1,~f _ 2=a _ 2,\cdots,f _ k=a _ k$，当 $n>k$ 时，有 $f _ n=c _ 1f _ {n-1}+c _ 2f _ {n-2}+\cdots+c _ kf _ {n-k}$。

“比如，令 $k=2,~a _ 1=a _ 2=c _ 1=c _ 2=1$，那么它就成了斐波那契数列。

“现在给你 $a _ 1,\cdots,a _ k$ 这 $k$ 个初始项、$c _ 1,\cdots,c _ k$ 这 $k$ 个递推系数，以及 $n$，你能求出 $f _ n$ 吗？注意 $n$ 很大哟~你不会了吧~”

罗姆：“这。。老夫还真不会，你这破盒子就能算对了？”

你作为出题人手机这个程序的编写者，是时候向罗姆爷展现你的能力了！

由于答案可能很大，你只需要输出答案对 $10^9+7$ 取模。易证，对于乘法运算：$(a\times b) \bmod p=((a \bmod p) \times (b \bmod p)) \bmod p$，对于加法运算：$(a+b) \bmod p=((a \bmod p)+(b \bmod p))\bmod p$。即，对于你的程序中跟答案直接相关的加法和乘法运算，每次运算完之后都对 $10^9+7$ 取模，这样就不会溢出 long long 了。

**本题附有提示，但在阅读提示前请务必进行充分的独立思考，否则对你的学习能力提升没有意义。**

### 输入格式

第一行两个正整数 $k,n$。（$1 \leq k \leq 20$，$k \leq n \leq 10^{18}$）

第二行 $k$ 个整数，表示 $a _ 1,\cdots,a _ k$。

第三行 $k$ 个整数，表示 $c _ 1,\cdots,c _ k$。（$1 \leq a _ i,c _ i \leq 10^9$）

### 输出格式

一行，一个整数，表示 $f _ n$ 对 $10^9+7$ 取模。

### 样例输入1
```
2 6
1 1
1 1
```
### 样例输出1
```
8
```
### 样例输入2
```
3 6
1 2 3
3 2 1
```
### 样例输出2
```
181
```

### 提示1

聪明的你一定学过线性代数，我们来简单应用一下。

考虑斐波那契数列 $ f _ n=f _ {n-1}+f _ {n-2} $，我们构造一个向量 $\vec v=\begin{bmatrix}f _ {n-1} \newline f _ {n-2}\end{bmatrix}$，把它乘一个矩阵 $A=\begin{bmatrix}1&1 \newline 1&0\end{bmatrix}$，会得到什么呢？
$$
A\vec v=\begin{bmatrix}1&1 \newline 1&0\end{bmatrix}\begin{bmatrix}f _ {n-1} \newline f _ {n-2}\end{bmatrix}=\begin{bmatrix}f _ {n-1}+f _ {n-2} \newline f _ {n-1}\end{bmatrix}=\begin{bmatrix}f _ n \newline f _ {n-1}\end{bmatrix}
$$

会发现 $f$ 的下标往前推进了一位！这就产生了递推！

聪明的你一定想到了扩展到常系数齐次线性递推要怎么做。

### 提示2

给定 $x,y$，而 $y$ 很大，要怎么求 $x^y$ 呢？

如果 $y$ 是偶数，你可以先求 $x^{\frac y2}$，然后给它平方一下；

如果 $y$ 是奇数，你可以先求 $x^{\frac{y-1}{2}}$，然后给它平方一下，然后再乘个 $x$。

聪明的你一定知道这样做的时间复杂度是多少。

### 提示3

离散课本里好像还给出了一种直接解特征方程的方法呢！哪个方法比较好呢？课本里的方法能不能用在这个题里呢？两种方法有什么联系呢？能不能互相推导出来呢？

聪明的你一定被激发了学习的热情！

### 题解

![83JhUH.png](https://s1.ax1x.com/2020/03/15/83JhUH.png)
这题涉及到两个常用算法：矩阵乘法求递推、快速幂。




# Day11 异世界六部曲（三）艾米莉亚的OJ

### 背景故事

*后来，出题人通过死亡的经历，知道了猎肠者的阴谋，救下了艾米莉亚，艾米莉亚把出题人带回了宅邸，出题人留下成为了仆人。*

*出题人迫不及待地想跟艾米莉亚约会，但是发现女仆们都防着他，终有一天出题人约到了艾米莉亚，愉快的一天过后他竟然被杀死了。*

*出题人十分不解，又死了几次才终于明白了原因。原来，艾米莉亚努力考上了中山大学计算机系，每天都在刷程设题。而出题人只是中大的混混，女仆们怕艾米莉亚整天跟出题人玩会变得颓废，于是要把他清理掉。*

*不甘心的出题人决定教艾米莉亚学程设，证明自己是优秀的大学生。*

### 题目描述

艾米莉亚利用课余时间刷了很多题目，但她是个很健忘的孩子，经常会忘记自己刷过一些什么题目，所以出题人想写一个 OJ 来帮她管理做过的题目。

经过一个星期的努力，出题人的 OJ 基本成型，只是还差一个 Contest 的模块没有实现。出题人觉得这个模块很难实现，所以他希望找你来帮忙。

一个OJ的基础元素包括：

1、题目，可以用pid唯一标识，pid为正整数。

2、比赛，可以用cid唯一标识，cid为正整数。

3、用户，可以用uid唯一标识，uid为正整数。

4、提交状态，可以由sid唯一标识，sid为正整数。一个提交状态是由sid, cid, pid, uid, result组成的，分别表示本条状态的提交ID，所属比赛ID，题目ID，用户ID以及评测结果。 

简单起见，这里的result只有AC,UNAC,WAIT三种状态，分别表示通过，不通过，等待评测。 

一个比赛模块需要实现以下请求： 

1. createContest cid t pid _ 1 pid _ 2 ... pid _ t  
表示要创建一个比赛，cid是一个正整数，是这场比赛的唯一标识。  
t表示这场比赛有t(1<=t<=1000)道题目，接下来t个不同的整数，表示这场比赛的题目编号。

2. submission sid cid pid uid result  
该条状态的sid要么之前没出现过，要么以前出现过，但是被rejudge了。
result为AC或者UNAC。

3. getRank cid uid  
在一场比赛中，所有有提交的用户都应该算在排名内(包括被rejudge的提交)，用户的排名按照通过的题目数从大到小排序，如果题目数相同，则按随机顺序排序。  
该指令需要统计用户uid在cid这场比赛中的**当前通过题目数**，**当前可能最高排名**以及**当前可能最低排名**。 
值得注意的是，用户uid在cid这场比赛中同一道题目的多个通过记录只算一次。  
**输出格式**: uid solved highest lowest   
分别代表用户ID，通过题目数量，最高排名以及最低排名，其中highest <= lowest。

4. rejudge sid  
重测以sid标识的提交记录，即将该记录的result改成WAIT

### 输入格式

第一行三个整数pcnt, ucnt, m(1<=pcnt<=5000, 1<=ucnt<=5000, 1<=m<=300000)，分别表示OJ有pcnt道题目，ucnt个用户以及m条请求。

pid的范围是1000 ~ 1000 + pcnt - 1，uid的范围是1 ~ ucnt，cid的范围为1 ~ 50，1<=sid<=m。

接下来有m行，每行一个请求，请求为题述四种请求之一，请求是需要按输入顺序执行。

**输入满足以下几点：**

1、对于createContest请求，保证cid不会与之前的createContest的cid重复。

2、对于submission请求，在此请求前，保证比赛cid在已经创建，题目pid是该场比赛的题目之一。

3、对于getRank指令，在此请求前，保证比赛cid已经创建，用户uid至少在该比赛中有一个提交记录。

4、对于rejudge指令，在此请求前，保证存在以sid标识的提交。

### 输出格式

对于每一个**getRank**请求，根据要求输出用户ID，通过题目数量，最高排名以及最低排名，整数之间用一个空格隔开。

### 样例输入

```
7 5 17
createContest 1 5 1001 1004 1002 1005 1006
submission 1 1 1001 1 AC
submission 2 1 1001 1 AC
submission 3 1 1001 2 UNAC
submission 4 1 1004 3 UNAC
getRank 1 1
getRank 1 2
getRank 1 3
rejudge 3
submission 3 1 1001 2 AC
getRank 1 2
submission 5 1 1006 2 AC
getRank 1 1
submission 6 1 1006 2 UNAC
getRank 1 2
rejudge 5
getRank 1 2
```

### 样例输出

```
1 1 1 1
2 0 2 3
3 0 2 3
2 1 1 2
1 1 2 2
2 2 1 1
2 1 1 2
```

### 数据范围

1 <= pcnt, ucnt <= 5000, 1 <= m <= 300000, 1 <= cid <= 50

### 题解

此题为竞赛题，主要考察数据结构运用和巧妙处理数据。
1. createContest  
其实啥都不用干，把该读入的读入掉就行了。
2. submission  
更新`participated数组`和`con[cid]`。
即如果这个人是第一次参加这个比赛，则 
```
con[cid].sum[0]++
```
如果是一条AC记录，则需要判断这个人在这场比赛里是否曾经AC过这道题，如果不是，则
```c
con[cid].sum[ con[cid].solnum[uid] ]--,
con[cid].solnum[uid]++,
con[cid].sum[ con[cid].solnum[uid] ]++;
```
3. 问题来了：如何知道一个人在一场比赛中是否做出过某道题？  
直接开三维数组？不行，空间复杂度爆了。
注意一个特性：每条操作最多增加一个这样的记录，而操作数只有m个。
开一个数组 `sub[i][j]`（每个元素为一条链表） 表示 i 这个人对于 j 这道题，在哪些比赛里有提交过，通过次数是多少。链表元素的总量最多 m 个，空间复杂度 𝑂(𝑚)。
判断 `uid` 这个人在 `cid` 这场比赛中是否做出过 `pid` 这道题，只需查看 `sub[uid][pid]` 这条链上是否有 `cid` 这个元素，以及其通过次数是多少。
每个元素的链长最多为 `maxcid`，因此更新、查找操作的时间复杂度都是 `𝑂(𝑚𝑎𝑥𝑐𝑖𝑑)`。
4. getRank  
设 `con[cid].solnum[uid]=h`，则最好排名为` 1+∑_(𝑖=ℎ+1)^𝑝𝑐𝑛𝑡▒[𝑐𝑜𝑛[𝑐𝑖𝑑].𝑠𝑢𝑚[𝑖] ]`，最坏排名为` ∑_(𝑖=ℎ)^𝑝𝑐𝑛𝑡▒[𝑐𝑜𝑛[𝑐𝑖𝑑].𝑠𝑢𝑚[𝑖] ]`。
5. 问题来了：如果每次询问都这样求一次和，时间复杂度就爆掉了。  
解决方法：改变 `con[cid].sum[i]` 的含义。`con[cid].sum[i]` 表示在 `cid` 这场比赛中，做出题数 >= i 的有多少人。
于是回答getRank的时候就不用求和了，直接回答 `1+con[cid].sum[h+1]` 以及 `con[cid].sum[h]` 就行了。
而前面submission中对于 `con[cid].sum` 的维护也很简单，去掉第一条即可。
__比如，曾经做出>=3题的有x人，做出>=4题的有y人，现在有位选手从3题变成4题了，那么就有 y++，其他都不受影响。__
6. rejudge  
如果这条记录原本是 UNAC 或者 WAIT，那么不用管
如果原本是 AC，那么跟 submission 中的 `con[cid].sum` 的维护是类似的，反过来而已。

最终，这题的时间复杂度是 `𝑂(𝑚∙𝑚𝑎𝑥𝑐𝑖𝑑)`，空间复杂度是 `𝑂(𝑝𝑐𝑛𝑡⋅𝑚𝑎𝑥𝑐𝑖𝑑+𝑚)`。



```cpp
#include <stdio.h>
#include <stdbool.h>

#define fo(i,a,b) for (int i = a; i <= b; i++)
#define maxcnt 5005
#define maxcid 55
#define maxm 300005

typedef struct Contest {
    int solution_num[maxcnt], sum[maxcnt];
} Contest;

typedef struct Submission {
    int cid, pid, uid;
    bool result;
} Submission;

int pcnt, ucnt, m, con0[maxcnt];
Contest con[maxcid];
Submission sub[maxm];
bool participant[maxcnt][maxcid];

int total, go[maxm], cnt[maxm], next[maxm], f1[maxcnt][maxcnt];
void ins(int uid, int pid, int cid) {
    go[++total] = cid;
    next[total] = f1[uid][pid];
    f1[uid][pid] = total;
}
int find(int uid, int pid, int cid) {
    for (int p = f1[uid][pid]; p; p = next[p])
        if (go[p] == cid) return p;
    return 0;
}

char s[20];
int main() {
    scanf("%d %d %d", &pcnt, &ucnt, &m);
    while (m--) {
        scanf("%s", s+1);
        if (s[1] == 'c') {  // 通过操作名称首字母不一样巧妙判断操作
            int cid, pnum, pid;
            scanf("%d %d", &cid, &pnum);
            fo(i,1,pnum) scanf("%d", &pid);
        } else if (s[1] == 's') {
            int sid, cid, pid, uid;
            scanf("%d %d %d %d %s", &sid, &cid, &pid, &uid, s+1);
            pid -= 999; // 注意pid范围，合理利用空间
            sub[sid] = (Submission){cid, pid, uid, (s[1] == 'A')};
            if (!participant[uid][cid]) {
                participant[uid][cid] = 1;
                con[cid].sum[0]++;
            }
            if (s[1] == 'A') {
                int p = find(uid, pid, cid);
                if (!p) ins(uid, pid, cid), p = total;
                if (++cnt[p] == 1) con[cid].sum[++con[cid].solution_num[uid]]++;
            }
        } else if (s[1] == 'g') {
            int cid, uid;
            scanf("%d %d", &cid, &uid);
            int h = con[cid].solution_num[uid];
            printf("%d %d %d %d\n", uid, h, con[cid].sum[h+1]+1,con[cid].sum[h]);
        } else {
            int sid, cid, pid, uid;
            scanf("%d", &sid);
            cid = sub[sid].cid, pid = sub[sid].pid, uid = sub[sid].uid;
            if (sub[sid].result) {
                int p = find(uid, pid, cid);
                if (--cnt[p] == 0) con[cid].sum[con[cid].solution_num[uid]--]--;
            }
        }
    }
    return 0;
}
```




# Day9 小蒟蒻喝果汁

### 题目描述

>小蒟蒻设计完游戏后身心俱疲，于是他从冰箱里拿出他最喜欢的果汁，从左往右倒了 n 杯果汁，第 i 杯果汁有 a[i] 毫升。
现在小蒟蒻很好奇，如果给定一个 k ，则任意连续的 k 杯果汁中，最多的一杯装了多少果汁，最少的一杯装了多少果汁？
换句话说，你要分别告诉小蒟蒻序号在[1, k],[2, k + 1] …… [n – k + 1, n]区间内果汁的 ai 的最大和最小值。

>蒟蒻发现最大值时高时低，他认为这不能给他带来开心，他认为当杯子里的果汁从左到右组成的序列字典序最小时，自己就会很开心。
于是他想进行任意多次操作，每次操作，小蒟蒻先可以将一段连续的杯子内的果汁变得平均，即选择区间[l, r]，l到r序号内所有的杯子里的果汁将变为 (a[l] + a[l + 1] + …… + a[r]) / (r – l + 1)。
可以进行任意次的以上操作，要让最后杯子内果汁的量的字典序最小，求最后每杯内的果汁容量。比如若有两杯果汁，从左到右果汁量为 2和1他就会不开心，因为平均后1.5和1.5的字典序更小，关于字典序详见下面描述 。

>[什么是字典序小呢？我们设通过两个方案 B 和C后，杯子内的果汁分别为——方案B： b[1],b[2],……,b[n] 和方案C：c[1],c[2],……,c[n]；若称方案B最后杯子内果汁的量的字典序更小(即方案 B 更优)，当且仅当存在一个整数k(1≤k≤n)，使得对于 i＜k 有 b[i] ＝ c[i]，并且对于i ＝k 有 b[i] ＜ c[i],最后的答案应该为所有方案中最后杯子内果汁的量的字典序最小的那一个！]。
>小蒟蒻不太聪明，快帮帮他吧！

### 输入格式

>输入的第一行包括两个整数 n (1≤n≤1000,000) 和 k(1≤k≤n)，分别表示果汁的杯数和题目中要求统计的相邻的果汁数。
输入的第二行包括 n 个数a1，a2……an(ai＜1000,000,000)，其中 ai 表示初始时第 i 杯果汁中果汁的量。

### 输出格式
	
>你一共需要输出三行。
第一行（n – k + 1） 个整数，第 i 个数代表下标在[i, i + k - 1]的果汁中最大的果汁量是多少。
第二行（n – k + 1） 个整数，第 i 个数代表下标在[i, i + k - 1]的果汁中最少的果汁量是多少。
第三行 n 个实数(保留两位小数)，代表经过若干次操作后得到的字典序最小的每一杯的果汁量。
相邻两个数中间有一个空格！


### 样例输入
	7 3
	1 2 3 4 3 2 1


### 样例输出
	3 4 4 4 3
	1 2 3 2 1
	1.00 2.00 2.60 2.60 2.60 2.60 2.60

### 题解

输出的前两行其实是一个问题，求一定区间的最大或最小值。用单调队列的方式。
第三行

```c
#include <stdio.h>

typedef struct node_queue { // 队列节点结构体，包括数值和下标
    int val, index;
}Node_queue;
Node_queue new_node;
Node_queue min_queue[1100000], max_queue[1100000];
int max_ans[1100000], min_ans[1100000];
int min_head = 0, min_tail = 0, max_head = 0, max_tail = 0;

typedef struct node_stack { // 栈节点结构体，包括平均值和长度
    double avg;
    int length;
}Node_stack;
Node_stack stack[1100000], now_node;
int top = 0;

int a[1100000];
int total = 0, range = 0;

int main() {
    scanf("%d %d", &total, &range);
    for (int i = 1; i <= total; i++) scanf("%d", &a[i]);
    // 区间最大和最小用单调队列
    new_node.index = 1; new_node.val = a[1];    // a[1]入队并初始化队列
    min_head = 1; min_tail = 1; min_queue[min_head] = new_node; min_ans[1] = min_queue[min_head].val;
    max_head = 1; max_tail = 1; max_queue[max_head] = new_node; max_ans[1] = max_queue[max_head].val;
    for (int i = 2; i <= total; i++) {
        new_node.index = i; new_node.val = a[i];    // 取a[i]并判断队头是否在当前区间以外
        if (min_queue[min_head].index <= i - range) min_head++; 
        if (max_queue[max_head].index <= i - range) max_head++;

        while ((new_node.val < min_queue[min_tail].val) && (min_tail >= min_head)) min_tail--;  //当a[i]的值小于队尾节点值时，队尾节点的值不会是接下来区域内最小值，故去掉，以下同理
        while ((new_node.val > max_queue[max_tail].val) && (max_tail >= max_head)) max_tail--;

        min_queue[++min_tail] = new_node;   // a[i]入队
        max_queue[++max_tail] = new_node;

        min_ans[i] = min_queue[min_head].val;   //此时区间最值为队头
        max_ans[i] = max_queue[max_head].val;
    }
    
    printf("%d", max_ans[range]);
    for (int i = range + 1; i <= total; i++) printf(" %d", max_ans[i]);
    printf("\n");

    printf("%d", min_ans[range]);
    for (int i = range + 1; i <= total; i++) printf(" %d", min_ans[i]);
    printf("\n");

    /*------------------------------*/
    //单调栈使字典序最小
    for (int i = 1; i <= total; i++) {
        now_node.length = 1; now_node.avg = a[i];   // 取a[i]
        while (top) {
            if (now_node.avg <= stack[top].avg) {   //如果小于当前平均值，则再进行平均
                now_node.avg = (now_node.avg * (double) now_node.length + stack[top].avg * (double) stack[top].length) / (double)(now_node.length + stack[top].length);
                now_node.length += stack[top].length;
                top--;
            } else break;
        }
        stack[++top] = now_node;
    }

    for (int i = 1; i <= top; i++) {
        for (int j = 1; j <= stack[i].length; j++) {
            if (i + j > 2) printf(" ");
            printf("%.2lf", stack[i].avg);
        }
    }
    return 0;
}
```

通过这题可以更加深入地理解队列和栈的使用，可以感受到出题人的一片苦心。
