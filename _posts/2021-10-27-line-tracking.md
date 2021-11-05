---
layout:     post
title:      视觉巡线小车
subtitle:   Visual Line-tracking Car
date:       2021-10-27
author:     炸毛
timecost:   5 minutes
# header-style: black
# header-mask: 0.01
# header-img-credit:      Yuhan Chen
# header-img-year:        2021 
header-img-outchain: false
header-img: img/bg/r_and_m.png
# nav-style: invert
catalog: true
mathjax: true
live2d:  false
byncsa:  true
do-not-show-in-index: false
tags:
    - Robots
---

> DCS361 - Introduction to Robotics 2021 Fall

## 实验目标

巡线就是让小车沿着规定的轨道（通常是黑线）按照一定的速度进行移动。在本次实验中，需要为小车
添加相机，根据图像信息，设计算法使小车能沿着地面的黑线行驶。

## 实验过程

### 实验环境

Windows 10 21H1 19043.1237

Webots R2021b

### 实验步骤

#### 在 `LineFollowing` 的世界基础上为小车添加摄像头

`Robot -> Children` 添加 `Base nodes -> Camera`，此时屏幕上会出现一个显示相机画面的框，现在是一片漆黑。然后再在 `Camera -> Children` 添加 `Base nodes -> Transform`，再在 `Transform -> Children` 添加  `Base nodes -> Shape`，然后在 `appearance` 添加 `Base nodes -> PBRAppearance`，在 `geometry` 添加 `Base nodes -> Cylinder` 。最后设置一下合适的半径和高度，这里是 `radius = 0.01, height = 0.02`。最后设置相机名称，默认是 `camera` 也可以不改。设置相机的偏移量设置，相机的宽、高和视场角等参数，设置直到相机看得见路上的黑线。 注意：添加camera的过程中不设置boundingObject属性和 physics属性。我的参数如下，在走粗一点的的线的时候应该提高广角参数是的看的全。

![image-20211027233017238](/img/in-post/bot_hw4/image-20211027233017238.png)

![image-20211027232958694](/img/in-post/bot_hw4/image-20211027232958694.png)

#### 添加摄像头控制接口代码

##### 使能摄像头

```cpp
#include <webots/Camera.hpp>
  camera = getCamera("camera");
  camera->enable(4 * timeStep);
  int image_width = camera->getWidth();
  int image_height = camera->getHeight();
```

##### 在主循环中读取和处理数据

由于不能使用 OpenCV ，所以首先我们看一下得到的灰度数据，再根据数据进行比较简单粗暴的处理。

```cpp
// Process sensor data here.
const unsigned char *image = camera->getImage();
int gray_img[image_width][image_height];
for (int y = 0; y < image_height; y++){
    for (int x = 0; x < image_width; x++){ 
        gray_img[x][y] = camera->imageGetGray(image, image_width, x, y);
        printf("%d ", gray_img[x][y]);
    }
    printf("\n");
```

得到的数据如下，我们发现高光的地面大概是 200 以上，小车阴影的地面大概是 90 到 110 之间，黑线大概是 50 以下。这样应该可以比较好地判断处黑线。我们也发现了镜面反射会造成一个较亮的光斑，可能影响对黑线的判断。

彩色照片：

![image-20211027164743375](/img/in-post/bot_hw4/image-20211027164743375.png)

无阴影部分：

![image-20211027164712603](/img/in-post/bot_hw4/image-20211027164712603.png)

阴影部分：

![image-20211027164641508](/img/in-post/bot_hw4/image-20211027164641508.png)



做二值化，尝试黑线为0，其他为1。

![image-20211027165420480](/img/in-post/bot_hw4/image-20211027165420480.png)



#### 巡线控制策略

我本来打算写一个外接矩形的算法，后来发现其实只需要从左到右或者从右到左扫描，发现黑点就是黑线的边缘，而多次扫描可以大致构造出黑线在画面中的角度，这样便可以根据黑线在画面中的位置和角度来调节左右偏移和转向。

##### 分析偏移来使用 A 和 D 调整平移

首先判断最底下的一层像素，来决定是否发生偏移。若最底下一行像素黑色靠左，则说明应该往右平移；若最底下一行像素黑色靠右，则说明应该往左平移。若偏差小于阈值，则前进。

```cpp
bottom_sum = bottom_right + bottom_left;

if(bottom_sum > 96) {
    for (int i = 0; i < 4; i++)
        speed2[i] = 0.6*speed_rightward[i];
    printf("rightward ");
} else if(bottom_sum < 30) {
    for (int i = 0; i < 4; i++)
        speed2[i] = 0.6*speed_leftward[i];
    printf("leftward ");
} else {
    for (int i = 0; i < 4; i++)
        speed2[i] = speed_forward[i];
    printf("forward ");
}

printf("lb %d rb %d bl %d\n", bottom_left, bottom_right, bottom_sum-63);
```

这部分控制 `speed2`

##### 分析角度来使用 Q 和 E 调整转向

本来打算取点构造两条竖线测算斜率，后来发现一个比较简单的方法，就是判断上半整幅画面左右的黑色的含量。若左边黑色较多，则说明应该往右偏，若右边的黑色较多，则说明应该往左偏。

```cpp
int left_light = 0, right_light = 0;

for(int x = 0; x < image_width/2; x++) {
    for(int y = 0; y < image_height; y++) {
        left_light += bin_img[x][y];
        right_light += bin_img[x + image_width / 2][y];
    }
}

if(right_light - left_light > 100) {
    for (int i = 0; i < 4; i++)
        speed1[i] = 0.4*speed_leftCircle[i];
    printf("leftCircle ");
} else if (right_light - left_light < 100) {
    for (int i = 0; i < 4; i++)
        speed1[i] = 0.4*speed_rightCircle[i];
    printf("rightCircle ");
} else {
    for (int i = 0; i < 4; i++)
        speed1[i] = 0.4*speed_forward[i];
}

printf("l %d r %d\n", left_light, right_light);
```

这部分控制 `speed1`.

#### 在小车身上和圆心放置GPS

`Robot -> Children` 添加 `Base nodes -> GPS`，然后再在 `GPS -> Children` 添加 `Base nodes -> Transform`，再在 `Transform -> Children` 添加  `Base nodes -> Shape`，然后在 `appearance` 添加 `Base nodes -> PBRAppearance`，在 `geometry` 添加 `Base nodes -> Cone` 。

![image-20211027211444636](/img/in-post/bot_hw4/image-20211027211444636.png)

![image-20211027215422954](/img/in-post/bot_hw4/image-20211027215422954.png)

我搞了两个，一个放在车上，另一个放在圆心。由于都绑定在车这个机器人上，所以只需要在 `main` 控制器中就可以使用。

```cpp
// create the GPS instance.
GPS *gps = robot->getGPS("gps");
GPS *center = robot->getGPS("center");
gps->enable(TIME_STEP);
center->enable(TIME_STEP);
```

虽然说在圆心的GPS当车动了之后它的位置就会发生改变，但是我只要在车还没动的时候记录下它的位置，之后通过计算即可得出半径。它的位置是 `-4.91931e-05, -0.500973` 其实没必要也无法精确到如此多位小数，于是取 `0, -0.5` 。此时小车的位置是 `0.0176988, -2.27736` 计算可得 `r=1.785` 。之后就把这个测圆心的 GPS 的 `shape` 删掉了，这样就看不到了。计算误差如下：

```cpp
const double *curpos = gps->getValues();
cout << "Pos:" << curpos[0] << ", " << curpos[1] << endl;
double mistake = abs(sqrt(curpos[0] * curpos[0] + (curpos[1] + 0.5) * (curpos[1] + 0.5)) - 1.785);
total_mistake += mistake;
cout << "mistake = " << mistake << endl;
```

我设置了一个循环计数器，用来记录主循环次数。这样就可以把累计的误差除以循环次数来求出平均误差。

#### 求 motor 的平均和最大速度

这个指标我也不知道可以看出来啥，实现如下：

```cpp
for (int i = 0; i < 4; i++) {
    motors[i]->setVelocity(speed1[i] + speed2[i]);
    totalVelocity[i] += speed1[i] + speed2[i];
    if (maxVelocity[i] < speed1[i] + speed2[i]) maxVelocity[i] = speed1[i] + speed2[i];
    printf("motor #%d %lf\n", i, speed1[i] + speed2[i]);
}
countTime++;
cout << "avg_mistake " << total_mistake / countTime << endl;
for (int i = 0; i < 4; i++) {
    cout << "avg_velocity #" << i << " " << totalVelocity[i] / countTime << endl;
    cout << "max_velocity #" << i << " " << maxVelocity[i] << endl;
}
```

#### 求一周时间

当位置相差小于某值更新时间

```cpp
curTime = time(NULL);
if (oldpos[0] == 0.0 && oldpos[1] == 0.0) {
    oldpos[0] = curpos[0];
    oldpos[1] = curpos[1];
    oldTime =  curTime;
} else if ((fabs(curpos[0] - oldpos[0]) < 0.1) &&
           (fabs(curpos[1] - oldpos[1]) < 0.1)) {
    if (1 < curTime - oldTime) {
        cout << curTime - oldTime << "for a round."  << endl;
        oldTime = curTime;
    }
}
```

结果：

```shell
 1.63535e+09 for a round
```

我目测大概是11秒。

### 实验结果与分析

结果是小车在不断优化后逐渐可以稳定地巡线。

![image-20211027203252442](/img/in-post/bot_hw4/image-20211027203252442.png)

![image-20211027203259511](/img/in-post/bot_hw4/image-20211027203259511.png)

在圆上巡线，运行六分钟都很稳定：

![image-20211027223903121](/img/in-post/bot_hw4/image-20211027223903121.png)

把它弄偏一点它会自己调回来（前提是在看得到黑线的情况下）

![image-20211027224844435](/img/in-post/bot_hw4/image-20211027224844435.png)

![image-20211027224755270](/img/in-post/bot_hw4/image-20211027224755270.png)

![image-20211027224730689](/img/in-post/bot_hw4/image-20211027224730689.png)

一圈时间：

![image-20211027225447162](/img/in-post/bot_hw4/image-20211027225447162.png)

平均误差在0.02左右。

![image-20211027231523558](/img/in-post/bot_hw4/image-20211027231523558.png)

### 实验过程的问题与解决方案

1. 小车经常偏离黑线

   解决方案：增大镜头广角，减少向前速度

2. 小车运行时不稳定，虽然可以巡线但是偏来偏去

   解决方案：减少转向的速度，减少前进的速度

### 可以改进之处

可以使用 PID 控制速度，在下一次实验中实现。
