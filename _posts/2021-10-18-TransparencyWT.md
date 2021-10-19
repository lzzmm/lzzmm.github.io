---
layout:     post
title:      为Windows Terminal开启透明背景
subtitle:   Acrylic Background for Windows Terminal
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
tags:
    - 花里胡哨
---

## 一种更加简易的方法

整了这么多，今天突然发现在图形界面的设置里都可以很方便地设置。

![imsb](/img/in-post/wt_settings.png)

~~## 开启亚克力效果~~

~~在 Windows Terminal 中按下 `ctrl+,` (Windows) 或者直接在标签栏处进入设置。打开 `settings.json` 文件，然后添加如下两句。~~

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

~~这样便可以使得背景为亚克力透明，透明度在0.4-0.8观感较好。~~

~~## 配置专注模式~~

~~增加下面的几行可以帮助配置成启动专注模式，通过 `ctrl+f` 来开关。进入专注模式需要熟记快捷键，否则可能更难操作。~~

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

使用[screenfetch](https://github.com/KittyKatt/screenFetch)是最简便的一种方法。它会自动检测你的Linux发行版并展示logo和一些基本的硬件信息。

以我的 VM 为例。

### 安装screenfetch

```bash
sudo apt install screenfetch
```

### 使用screenfetch

```bash
screenfetch
```

![screenfetch](/img/in-post/windowsTerminal.png)

## 安装 oh-my-posh

> Oh my Posh is a custom prompt engine for any shell that has the ability to adjust the prompt string with a function or variable.  - [https://ohmyposh.dev/docs/](https://ohmyposh.dev/docs/)

```bash
Install-Module oh-my-posh -Scope CurrentUser
```

同时需要安装支持的字体

[Nerdfronts](https://www.nerdfonts.com/font-downloads)

推荐 _Caskaydia Cove Nerd Font_

安装后在设置中更改字体，然后可以查看oh-my-posh的所有主题

```bash
Get-PoshThemes
```

设置主题

```bash
Set-PoshPrompt -Theme agnosterplus
```

然后可以查看效果。

但是如果想一直使用oh-my-posh，还需要修改`$PROFILE`

```bash
code $PROFILE
```

在打开的文件中写入

```bash
Import-Module oh-my-posh
Set-PoshPrompt -Theme agnosterplus
```

保存并退出。重启Windows Terminal，可以发现oh-my-posh正常运行。

![ohmyposh](/img/in-post/ohmyposh.png)
