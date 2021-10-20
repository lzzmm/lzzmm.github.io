---
layout:     post
title:      为Windows Terminal开启透明背景等
subtitle:   Acrylic Background for Windows Terminal and more
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

## 安装 oh-my-zsh

Bash是最常见的Linux Shell，Zsh与Bash类似，是另一个很棒的shell，它比Bash更快，更可自定义。关于Zsh的最酷的事情之一是颜色自定义，你只需使用名为[Oh My Zsh](https://ohmyz.sh/)的框架即可更改shell的主题和颜色。想知道你的系统有哪些shell，可以通过以下命令查看：

```shell
cat /etc/shells
```

### 安装 zsh 和 git (若已有可跳至[安装oh-my-zsh](#安装oh-my-zsh)) <!--https://lzzmm.github.io/2021/10/19/oh-my-zsh/-->

```shell
sudo apt-get install zsh
```

安装完成后设置当前用户使用zsh

```shell
chsh -s /bin/zsh
```

安装 git

```shell
sudo apt-get install git
```

### 安装oh-my-zsh

使用 `wget`

```shell
sh -c "$(wget https://raw.github.com/ohmyzsh/ohmyzsh/master/tools/install.sh -O -)"
```

安装完成会出现 "oh my zsh" 字样

![omz](/img/in-post/omz.png)

### 设置主题

> All the current themes can be found in the `themes/` directory in the oh-my-zsh distribution. In order to enable a theme, set `ZSH_THEME` to the name of the theme in your `~/.zshrc`, before sourcing Oh My Zsh; for example: `ZSH_THEME=robbyrussell` If you do not want any theme enabled, just set `ZSH_THEME` to blank: `ZSH_THEME=""`

默认主题是[robbyrussell](https://github.com/ohmyzsh/ohmyzsh/wiki/Themes)，我用的是与Powershell主题差不多的[agnoster](https://github.com/agnoster/agnoster-zsh-theme)。这个主题在Ubuntu系统中推荐安装[powerline](https://github.com/powerline/fonts)。

```shell
sudo apt-get install fonts-powerline
```

在终端中更改字体。

效果如下

![agnoster](/img/in-post/agnoster.png)
