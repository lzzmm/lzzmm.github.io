---
layout:     post
title:      为Windows Terminal开启透明背景
subtitle:   
date:       2021-10-18
author:     炸毛
timecost:   5 minutes
# header-style: black
# header-mask: 0.01
# header-img-credit:      Yuhan Chen
# header-img-year:        2021 
header-img-outchain: false
header-img: img/in-post/windowsTerminal.png
# nav-style: invert
catalog: true
mathjax: true
live2d:  false
byncsa:  true
do-not-show-in-index: false
# tags:
#     - C/C++
#     - HPC
---

## 开启亚克力效果

在 Windows Terminal 中按下 `ctrl+,` (Windows) 或者直接在标签栏处进入设置。打开 `settings.json` 文件，然后添加如下两句。

```json
    "profiles": {
        "defaults": {
            "acrylicOpacity": 0.65000000000000002,
            "useAcrylic": true
        },
        "list": [
            {
                "commandline": "powershell.exe",
                ···
```

这样便可以使得背景为亚克力透明，透明度在0.4-0.8观感较好。

## 配置专注模式

增加下面的几行可以帮助配置成启动专注模式，通过 `ctrl+f` 来开关。进入专注模式需要熟记快捷键，否则可能更难操作。

```json
"actions": [
        {
            "command": "toggleFocusMode",
            "keys": "ctrl+f"
        }
    ],
    "defaultProfile": "{61c54bbd-c2c6-5271-96e7-009a87ff44bf}",
    "launchMode": "focus",
```

## 在终端里展示Linux发行版的logo及系统信息

使用screenfetch是最简便的一种方法。它会自动检测你的Linux发行版并展示logo和一些基本的硬件信息。

### 安装screenfetch

```bash
sudo apt install screenfetch
```

### 使用screenfetch

```bash
screenfetch
```

![screenfetch](/img/in-post/windowsTerminal.png)

To Be Continued...

TODO: oh-my-posh,
