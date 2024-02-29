---
layout:     post
title:      修复 mathjax 渲染多行公式出现的错误
subtitle:   
date:       2021-10-22
author:     zhamao
timecost:   6 minutes
# header-style: text
# header-mask: 0.01
# header-img-credit:      Chen Yuhan
# header-img-year:        2020
# header-img-outchain:    true
# nav-style: invert
header-img: img/bg/paimon_dying.jpg
catalog: true
mathjax: true
live2d:  false
do-not-show-in-index:   true
do-not-show-preview:    true
byncsa:  true
tags:
---

mathjax 渲染多行公式的问题困扰了我很久，之前魔改主页的时候不知道动了啥导致渲染的时候会被多一层 `\[` 和 `\]` 包围。

![wrong mathjax](/img/in-post/wrong-math.png)

比较粗鲁的解决方法是修改 `text/x-mathjax-config` ，在 `displayMath` 中加入  `['\\[', '\\]']` .

```html
<script type="text/x-mathjax-config">
  MathJax.Hub.Config({
    TeX: {
      equationNumbers: {
        autoNumber: "AMS"
      }
    },
    tex2jax: {
      inlineMath: [ ['$','$'] ],
      displayMath: [ ['$$','$$'], ['\\[','\\]'] ],
      processEscapes: true,
    }
  });
</script>
```

使用 $\LaTeX$ 的 $\left[\right]$ 的时候记得使用 `\left[\right]`。

```latex
$$
\begin{equation}
\begin{aligned}
\displaystyle\lim_{n \to \infty}\frac{2n^2+1}{n^2+n-2}=2
\end{aligned}
\end{equation}
$$

$$
\begin{equation}
\begin{aligned}
    \left[a\right]=\left\{x\in X|x \sim a\right\}
\end{aligned}
\end{equation}
$$
```

$$

\begin{equation}
\begin{aligned}
\displaystyle\lim_{n \to \infty}\frac{2n^2+1}{n^2+n-2}=2
\end{aligned}
\end{equation}

$$

$$

\begin{equation}
\begin{aligned}
    \left[a\right]=\left\{x\in X|x \sim a\right\}
\end{aligned}
\end{equation}

$$
