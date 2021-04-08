---
layout:     post
title:      Live2D
subtitle:   Deploy dynamic poster girl with Live2D on web page
date:       2020-02-21
author:     炸毛
catalog:    true
mathjax:    true
live2d:     true
timecost:   2 minutes
header-style:   black
nav-style:      invert
header-img:     img/post-bg-live2d.jpg
tags:
    - Web
    - GitHub Pages
---

# 简单部署看板娘到网页

> [看板娘(かんばんむすめ)](https://baike.baidu.com/item/%E7%9C%8B%E6%9D%BF%E5%A8%98/2581794?fr=aladdin)是一种职业和习惯称呼，也是ACGN次文化中的萌属性之一。简而言之就是小店的女服务生，也有“吸引顾客，招揽生意，提高人气”等作用类似品牌形象代言人的含义。
> Live2D是一种应用于电子游戏的绘图渲染技术，技术由日本[Cybernoids](https://www.live2d.com/)公司开发。通过一系列的连续图像和人物建模来生成一种类似三维模型的二维图像，对于以动画风格为主的冒险游戏来说非常有用.Live 2D人物无法大幅度转身，开发商正设法让该技术可显示360度图像。  
—— 百度百科

具体效果请看左下角。  

# 添加JS代码到网页

```html
<script type="text/javascript" charset="utf-8"  src="{{ site.baseurl }}/js/L2Dwidget.0.min.js"></script>
<script type="text/javascript" charset="utf-8"  src="{{ site.baseurl }}/js/L2Dwidget.min.js"></script>
<script type="text/javascript">
    L2Dwidget.init({"display": {
        "superSample": 2,
        "width": 200,
        "height": 400,
        "position": "left",
        "hOffset": 0,
        "vOffset": 0
        }
    });
</script>
```
参数很容易理解。

# 引用JS文件
然后我们发现在这段代码中引用了两个JS文件。这两个文件可以通过DevTools(`F12`)从这个页面拿走。

#### 具体操作
DevTools(`F12`) -> `Sources` -> 左边(Chrome为例)`Page`栏 -> `top` -> `cleveryh.github.io` -> `js` -> `L2Dwidget.0.min.js` 和 `L2Dwidget.min.js` , 右键`Save as`即可保存。

然后根据这两个JS文件在你网页上的目录修改上面脚本代码头两行，依葫芦画瓢。

# CSS

```css
#live2dcanvas {
    border: 0 !important;
}
```
添加CSS代码。

# 成功！

其他功能有空再研究。

<https://github.com/xiazeyu/live2d-widget.js>