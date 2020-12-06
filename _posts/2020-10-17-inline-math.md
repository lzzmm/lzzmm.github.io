---
layout:     post
title:      Resolve Inline $Math$ in Typora
subtitle:   
date:       2020-10-17
author:     zhamao
timecost:   
header-style: text
# header-mask: 0.01
# header-img-credit:      Chen Yuhan
# header-img-year:        2020
# header-img-outchain:    true
# header-img: https://s3.ax1x.com/2020/11/22/D8TLJe.png
# nav-style: invert
catalog: true
mathjax: true
live2d:  false
tags:
    - Markdown
---

Inline $math$ $expression$ is the externed syntax of  $\LaTeX$ but not the generic standard of Markdown. So the inline math expression won't be  resolved by Typora in default.

All we should do is go to 

```
File -> Preferances -> Markdown -> Syntax Support
```

and turn on `Inline Math` .

Remember to restart Typora.

Try to type

```md
hello $world$ hahaha
```

and you will see:

hello $world$ hahaha

Now you can write inline math in Typora!
