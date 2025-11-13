---
layout:     post
title:      编译并启动 Linux 内核
subtitle:   在 Linux 上构建 OS Kernel 开发环境
date:       2021-03-10
author:     lzzmm
timecost:   10 minutes
# header-style: black
# header-mask: 0.01
# header-img-credit:      CHEN Yuhan
# header-img-year:        2021 
header-img-outchain: false
header-img: img/bg/sunrise_girl.jpg
# nav-style: invert
catalog: true
mathjax: true
live2d:  false
byncsa:  true
do-not-show-in-index: true
tags:
    - OS
    - Linux
---

> DCS218 - Operating Systems Lab 2021 Spring

## 前言

本文章介绍了在 Linux 虚拟机上搭建操作系统内核开发环境、编译 Linux 内核、使用 Qemu 启动内核并开启远程调试、制作Initramfs 并编译并启动 Busybox 的过程。

## 目的

- 熟悉现有 Linux 内核的编译过程和启动过程，并在自行编译内核的基础上构建简单应用并启动。
- 利用精简的 Busybox 工具集构建简单的操作系统，熟悉现代操作系统的构建过程。
- 熟悉编译环境、相关工具集，并能够实现内核远程调试。

## 过程

### 1. 搭建操作系统内核开发环境

由于此前已经在 *Vmware Workstation 15.5 Pro* 上安装了 *Ubuntu 20.04 LTS 64-bit* ，故不用推荐的 *18.04*，问题不大。需要安装的还有：

- **nasm** Netwide Assembler，一款x86汇编和反汇编软件
- **qemu** 可以模拟多种架构处理器
- **cmake**
- **bison** 用于自动生成语法分析器程序
- **flex** 是一个生成词法分析器的工具，它可以利用正则表达式来生成匹配相应字符串的C语言代码
- **libssl-dev** 是 OpenSSL 通用库，Secure Socket Layer 是为了传输敏感数据而提出的协议
- **libncurses5-dev** shared libraries for terminal handling
- **libc6-dev-i386** 32-bit shared libraries for AMD64

### 2. 编译 Linux 内核

1. 在根目录创建 `os2021/lab1` 文件夹并进入，到 <https://www.kernel.org/> 下载内核 5.10 到文件夹 `~/os2021/lab1`。

    ![2021-03-07 00-34-59 的屏幕截图](/img/in-post/os/2021-03-07 00-34-59 的屏幕截图.png)

    解压并进入。

    ```shell
    xz -d linux-5.10.20.tar.xz
    tar -xvf linux-5.10.20.tar
    cd linux-5.10.20
    ```

2. 将内核编译为 `i386 32-bit` 版本

    ```shell
    make i386_defconfig
    make menuconfig
    ```

    ![2021-03-07 00-36-49 的屏幕截图](/img/in-post/os/2021-03-07 00-36-49 的屏幕截图.png)

    在打开的图像界面中依次选择 `Kernel hacking`、`Compile-time checks and compiler options`，最后在 `[ ] Compile the kernel with debug info` 输入 `Y` 勾选，保存退出。
    ![2021-03-07 00-37-43 的屏幕截图](/img/in-post/os/2021-03-07 00-37-43 的屏幕截图.png)

    编译内核。

    ```shell
    make -j8
    ```

    ![2021-03-07 00-38-17 的屏幕截图](/img/in-post/os/2021-03-07 00-38-17 的屏幕截图.png)

    等待内核编译完成，检查Linux压缩镜像 `linux-5.10.20/arch/x86/boot/bzImage` 和符号表 `linux-5.10.20/vmlinux` 是否已经生成。

    ![2021-03-07 01-36-47 的屏幕截图](/img/in-post/os/2021-03-07 01-36-47 的屏幕截图.png)

    可以看到已经生成了。

### 3. 启动内核并调试

1. 使用 qemu 启动内核

    ```shell
    qemu-system-i386 -kernel linux-5.10.20/arch/x86/boot/bzImage -s -S -append "console=ttyS0" -nographic
    ```

    此时 qemu 并未输出任何信息，这是因为我们开启了 gdb 调试，而 qemu 在等待 gdb 输入的指令后才能继续执行。接下来启动 gdb，通过 gdb 来告诉 qemu 应该怎么做。

2. 使用 gdb 开启远程调试

    不关闭qemu所在的Terminal，在另外一个 Terminal 下启动 gdb ，加载符号表，连接已经启动的 qemu 进行调试，为 start_kernel 函数设置断点，输入 `c` 运行。

    回到 qemu 的 terminal，输出如下。

    ![2021-03-07 01-39-34 的屏幕截图](/img/in-post/os/2021-03-07 01-39-34 的屏幕截图.png)

    发现内核成功启动并最终以 Kernel panic 宣告结束。![2021-03-07 01-39-53 的屏幕截图](/img/in-post/os/2021-03-07 01-39-53 的屏幕截图.png)看到 call trace 打出来的是在 initrd_load 的时候出错，原因很简单，因为启动系统的时候只指定了 bzImage ，没有指定 initrd 文件，系统无法 mount 上 initrd (init ram disk) 及其 initramfs 文件系统。

### 4. 制作 initramfs

1. 在 `lab1` 文件夹中新建 `helloworld.c` 文件，写入以下代码

    ```c
    #include <stdio.h>
    
    void main()
    {
        printf("lab1: Hello World!\n");
        fflush(stdout);
        while(1);
    }
    ```

    然后编译为 32-bit 可执行文件。

    ```shell
    gcc -o helloworld -m32 -static helloworld.c
    ```

2. 加载 initramfs

    用 cpio 打包 initramfs

    ```shell
    echo helloworld | cpio -o --format=newc > hwinitramfs
    ```

    启动内核并加载 initramfs。

    ```shell
    qemu-system-i386 -kernel linux-5.10.20/arch/x86/boot/bzImage -initrd hwinitramfs -s -S -append "console=ttyS0 rdinit=helloworld" -nographic
    ```

    ![2021-03-07 01-54-39 的屏幕截图](/img/in-post/os/2021-03-07 01-54-39 的屏幕截图.png)

    重复上面的gdb的调试过程，可以看到gdb中输出了 `lab1: Hello World!\n`

    ![2021-03-07 01-55-42 的屏幕截图](/img/in-post/os/2021-03-07 01-55-42 的屏幕截图.png)

### 5. 编译并启动 Busybox

1. 下载 Busybox 到 `lab1` 并解压。

2. 编译它

    ```shell
    make defconfig
    make menuconfig
    ```

    ![2021-03-07 02-06-39 的屏幕截图](/img/in-post/os/2021-03-07 02-06-39 的屏幕截图.png)

3. 进入 `settings`，然后在 `Build static binary(no shared libs)` 处输入 `Y` 勾选，然后分别设置 `() Additional CFLAGS` 和 `() Additional LDFLAGS` 为 `(-m32 -march=i386) Additional CFLAGS` 和 `(-m32) Additional LDFLAGS` ，保存退出。

    ![2021-03-07 02-09-37 的屏幕截图](/img/in-post/os/2021-03-07 02-09-37 的屏幕截图.png)

    编译。

    ```shell
    make -j8
    make install
    ```

4. 制作Initramfs

    1. 将安装在_install目录下的文件和目录取出放在 `lab1/mybusybox` 处。![2021-03-07 02-13-24 的屏幕截图](/img/in-post/os/2021-03-07 02-13-24 的屏幕截图.png)

    2. initramfs 需要一个 init 程序，可以写一个简单的 shell 脚本作为 init 程序。用 gedit 打开文件 `init `，复制入如下内容，保存退出。

        ```shell
        #!/bin/sh
        mount -t proc none /proc
        mount -t sysfs none /sys
        echo -e "\nBoot took $(cut -d' ' -f1 /proc/uptime) seconds\n"
        exec /bin/sh
        ```

        ![2021-03-07 02-19-23 的屏幕截图](/img/in-post/os/2021-03-07 02-19-23 的屏幕截图.png)

    3. 加上执行权限。

        ```shell
        chmod u+x init
        ```

    4. 将 x86-busybox 下面的内容打包归档成 cpio 文件，以供 Linux 内核做 initramfs 启动执行。

        ```shell
        find . -print0 | cpio --null -ov --format=newc | gzip -9 > ~/os2021/lab1/initramfs-busybox-x86.cpio.gz
        ```

5. 加载 busybox

    ```shell
    qemu-system-i386 -kernel linux-5.10.20/arch/x86/boot/bzImage -initrd initramfs-busybox-x86.cpio.gz -nographic -append "console=ttyS0"
    ```

    ![2021-03-07 02-24-19 的屏幕截图](/img/in-post/os/2021-03-07 02-24-19 的屏幕截图.png)

6. 内核启动后使用 `ls` 命令查看当前文件夹。

    ![2021-03-07 02-22-54 的屏幕截图](/img/in-post/os/2021-03-07 02-22-54 的屏幕截图.png)

7. 退出

    退出qemu有两种方法，一种是按下 `Ctrl+A`，然后按 `X` ，另一种是kill掉它的进程。

    ![2021-03-07 02-24-15 的屏幕截图](/img/in-post/os/2021-03-07 02-24-15 的屏幕截图.png)

## 总结

学会了搭建 os kernel 开发环境，使用 qemu 启动内核，制作 initramfs ，使内核启动时运行程序，编译启动 Busybox，使用 gdb 远程调试跟踪代码运行，克服了对 Linux 命令行的不熟悉，熟悉了常用的 Linux 命令，学会了 kill 进程。也了解了一点关于操作系统启动的知识。

## 参考

<https://gitee.com/nelsoncheung/sysu-2021-spring-operating-system/tree/main>
