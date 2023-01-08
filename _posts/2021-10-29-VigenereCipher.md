---
layout:     post
title:      维吉尼亚密码加密与解密
subtitle:   Vigenere Cipher Encrypt and Decrypt
date:       2021-10-29
author:     炸毛
timecost:   10 minutes
# header-style: black
# header-mask: 0.01
# header-img-credit:      CHEN Yuhan
# header-img-year:        2021 
header-img-outchain: false
header-img: img/bg/green_night.jpg
# nav-style: invert
catalog: true
mathjax: true
live2d:  false
byncsa:  true
do-not-show-in-index: false
tags:
    - C/C++
    - Information Security
---

> DCS397 - Information Security Technology 2021 Fall

## 维吉尼亚密码加密与解密

### 背景

维吉尼亚密码是使用一系列凯撒密码组成密码字母表的加密算法，属于多表密码的一种简单形式。在一个凯撒密码中，字母表中的每一字母都会作一定的偏移，例如偏移量为3时，A就转换为了D、B转换为了E……而维吉尼亚密码则是由一些偏移量不同的恺撒密码组成。

为了生成密码，需要使用表格法。这一表格包括了26行字母表，每一行都由前一行向左偏移一位得到。具体使用哪一行字母表进行编译是基于密钥进行的，在过程中会不断地变换。

假设明文

```
stupid
```

密钥

```
abcdef
```

则加密后密文为

```
suwsmi
```

用数字0-25代替字母`A`-`Z`，维吉尼亚密码的加密文法可以写成同余的形式：

$C_i \equiv P_i + K_i (mod 26) $

解密方法则能写成：

$P_i \equiv C_i - K_i (mod 26) $

### 加密

加密使用上面所讲述的同余算法，加密的时候一致转换为大写或者小写，然后只对英文字母加密。下面的函数是加密函数，默认没有密钥。

```cpp
string encrypt(string s, string key = "a", bool uppercase = true, bool delete_space = false) {
    // input a string to be encrypted and a key_length
    // output a string encrypted with the key
    string ret;
    char a;
    if (uppercase)
        a = 'A';
    else
        a = 'a';

    s = to_lower(s, delete_space);
    key = to_lower(key);
    unsigned int string_length = s.length();
    unsigned int key_length = key.length();
    for (int i = 0; i < string_length; i++) {
        if (s[i] <= 122 && s[i] >= 97)
            ret.push_back((s[i] + key[i % key_length] - 2*(int)a) % 26 + a);
        else
            ret.push_back(s[i]);
    }
    return ret;
}
```

将共产党宣言进行加密：

加密前：

原文见 [`The_Communist_Manifesto.txt`](/downloads/The_Communist_Manifesto.txt)

![image-20211031211710484](/img/in-post/vig/image-20211031211710484.png)

加密后：

`Encrypted_The_Communist_Manifesto.txt`

![image-20211031211729141](/img/in-post/vig/image-20211031211729141.png)


### 解密

对包括维吉尼亚密码在内的所有多表密码的破译都是以字母频率为基础的，但直接的频率分析却并不适用。例如，如果P是密文中出现次数最多的字母，则P很有可能对应E（前提是明文的语言为英语）。原因在于E是英语中使用频率最高的字母。然而，由于在维吉尼亚密码中，E可以被加密成不同的密文，因而简单的频率分析在这里并没有用。

破译维吉尼亚密码的关键在于它的密钥是循环重复的。如果我们知道了密钥的长度，那密文就可以被看作是交织在一起的凯撒密码，而其中每一个都可以单独破解。使用卡西斯基试验和弗里德曼试验来得到密钥的长度。

#### 使用弗里德曼试验（重合指数法）获取密钥长度

$\kappa_p$指目标语言中两个任意字母相同的概率（英文中为0.067），$\kappa_r$指字母表中这种情况出现的概率（英文中为1/26=0.0385），从而密钥长度可以估计为：
$$
\frac{\kappa_p - \kappa_r}{\kappa_o - \kappa_r}
$$
其中，观察概率为
$$
\kappa_o = \frac{\sum^c_{i=1}n_i(n_i-1)}{N(N-1)}
$$

其中，c是指字母表的长度（英文为26），N指文本的长度，$n_1$到$n_c$是指密文的字母频率，为整数。

此方法只是一种估计，会随着文本长度的增加而更为精确。在实践中，会尝试接近此估计的多个密钥长度。一种更好的方法是将密文写成矩阵形式，其中列数与假定的密钥长度一致，将每一列的重合指数单独计算，并求得平均重合指数。对于所有可能的密钥长度，平均重合指数最高的最有可能是真正的密钥长度。这样的试验可以作为卡西斯基试验的补充。

我的实现如下：

```cpp
unsigned int key_length(string s) {
    unsigned int key_length = 1;
    unsigned int string_length = s.length();
    while (key_length <= string_length && key_length <= 100) {
        // when key length is greater than 100, the result is not reliable.
        double CI[key_length]; // Coincidence Index
        double avg_CI = 0.0f;  // average Coincidence Index

        for (int i = 0; i < key_length; i++) {
            // count the frequency of every character in the string.
            int count[26] = {0};
            for (int j = 0; i + j * key_length < string_length; j++) {
                if ((s[i + j * key_length] <= 122 && s[i + j * key_length] >= 97) || (s[i + j * key_length] <= 90 && s[i + j * key_length] >= 65))
                    count[(int)(s[i + j * key_length] - 'a')]++;
            }
            double e = 0.0f;
            int L = 0;
            for (int k = 0; k < 26; k++) {
                L += count[k];
            }
            L *= (L - 1); // L = L * (L - 1)
            for (int k = 0; k < 26; k++) {
                if (count[k] != 0) {
                    e += (double)count[k] * (double)(count[k] - 1) / (double)L;
                }
            }
            CI[i] = e; // CI'
        }
        for (int i = 0; i < key_length; i++) {
            avg_CI += CI[i];
        }
        avg_CI /= key_length;
        // cout << avg_CI << " " << key_length << endl;
        if (avg_CI > 0.06) // close to 0.065
            break;
        else
            key_length++;
    }
    return key_length;
}
```

#### 使用拟合重指数法来破解密钥

一旦能够确定密钥的长度，密文就能重新写成多列，列数与密钥长度对应。这样每一列其实就是一个凯撒密码，而此密码的密钥（偏移量）则对应于维吉尼亚密码密钥的相应字母。与破译凯撒密码类似的方法，就能将密文破译。

其中 $n$ 代表某种语言由 $n$ 个字母组成， $p_i$ 表示字母i在维吉尼亚密文分布中发生的概率，$q_i$ 表示字母 $i$ 在正常英语文本分布中发生的概率，则拟重合指数可以定义为
$$
\chi = \sum^n_{i=1}p_iq_i
$$
当两个频率分布类似时， $\chi$ 的值相对要高。

我们首先统计每个字母的频数，并获得通常文本的字母概率分布。对概率分布进行移位 i (0<=i<=25)，分别计算相关值，最大的或者超过了某个阈值的就是对位移量的估计值。

```cpp
string get_key(string s, unsigned int key_length) {
    double p[] = {
        8.17,
        1.49,
        2.78,
        4.25,
        12.70,
        2.23,
        2.02,
        6.09,
        6.97,
        0.15,
        0.77,
        4.03,
        2.41,
        6.75,
        7.51,
        1.93,
        0.10,
        5.99,
        6.33,
        9.06,
        2.76,
        0.98,
        2.36,
        0.15,
        1.97,
        0.07,
    }; // frequency table from a to z

    string key = "";
    unsigned int string_length = s.length();
    for (int i = 0; i < key_length; i++) {
        unsigned int offset = 0;
        for (int k = 0; k < 26; k++) {
            double X = 0.0f;
            int count[26] = {0};
            for (int j = 0; i + j * key_length < string_length; j++) {
                if ((s[i + j * key_length] <= 122 && s[i + j * key_length] >= 97) || (s[i + j * key_length] <= 90 && s[i + j * key_length] >= 65)) count[(int)(s[i + j * key_length] - 'a')]++;
            }
            int L = 0;
            for (int j = 0; j < 26; j++) {
                L += count[j];
            }
            for (int j = 0; j < 26; j++) {
                X += p[j] * count[(j + offset) % 26];
            }
            if (X / L > 5.5) {
                key.push_back('a' + offset);
                break;
            } else {
                offset++;
            }
        }
    }
    return key;
}
```

#### 使用密钥来解密

有了密钥之后解密就十分轻松了，只需循环执行先前的同余操作即可。

```cpp
string decrypt(string s, string key) {
    // input a string and a key for decrypting it
    // output is a string decrypted
    string ret;
    s = to_lower(s, false);
    key = to_lower(key);
    unsigned int string_length = s.length();
    unsigned int key_length = key.length();
    for (int i = 0; i < string_length; i++) {
        if (s[i] <= 122 && s[i] >= 97)
            ret.push_back((s[i] - key[i % key_length] + 26) % 26 + 'a');
        else
            ret.push_back(s[i]);
    }
    return ret;
}
```

### 验证

##### 1

首先对课本中的一段密文进行解密，原文在 `Encrypted.txt` ，得出密钥为 

```
JANET
```

对解密后的文字重新加密发现与原文相同。得出的文件在 `Decrypted.txt`

![image-20211031205902834](/img/in-post/vig/image-20211031205902834.png)

##### 2

解密之前加密的共产党宣言：

![image-20211031214113184](/img/in-post/vig/image-20211031214113184.png)

![image-20211031214131978](/img/in-post/vig/image-20211031214131978.png)

全部变成小写。得出的文件在 `Decrypted_The_Communist_Manifesto.txt` .

### 参考



### 附录

源代码 `VigenereCipher.cpp`

```cpp
// VigenereCipher.cpp
//
// Author:  CHEN Yuhan
// Date:    2021/10/30
//
// Vigenere cipher encryption and decryption
// Using Friedman test (Coincidence Index) and Frequency analysis for decryption

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#define TEST 0

using namespace std;

string del_space(string s) {
    if (s.empty()) return s;
    int index = 0;
    while ((index = s.find(' ', index)) != string::npos) {
        s.erase(index, 1);
    }
    return s;
}

string to_lower(string s, bool delete_space = true) {
    if (s.empty()) return s;
    if (delete_space) s = del_space(s);
    for (int i = 0; i < s.length(); i++)
        s[i] = tolower(s[i]);
    return s;
}

string to_upper(string s, bool delete_space = true) {
    if (s.empty()) return s;
    if (delete_space) s = del_space(s);
    for (int i = 0; i < s.length(); i++)
        s[i] = toupper(s[i]);
    return s;
}

unsigned int key_length(string s) {
    unsigned int key_length = 1;
    unsigned int string_length = s.length();
    while (key_length <= string_length && key_length <= 100) {
        // when key length is greater than 100, the result is not reliable.
        double CI[key_length]; // Coincidence Index
        double avg_CI = 0.0f;  // average Coincidence Index

        for (int i = 0; i < key_length; i++) {
            // count the frequency of every character in the string.
            int count[26] = {0};
            for (int j = 0; i + j * key_length < string_length; j++) {
                if ((s[i + j * key_length] <= 122 && s[i + j * key_length] >= 97) || (s[i + j * key_length] <= 90 && s[i + j * key_length] >= 65))
                    count[(int)(s[i + j * key_length] - 'a')]++;
            }
            double e = 0.0f;
            int L = 0;
            for (int k = 0; k < 26; k++) {
                L += count[k];
            }
            L *= (L - 1); // L = L * (L - 1)
            for (int k = 0; k < 26; k++) {
                if (count[k] != 0) {
                    e += (double)count[k] * (double)(count[k] - 1) / (double)L;
                }
            }
            CI[i] = e; // CI'
        }
        for (int i = 0; i < key_length; i++) {
            avg_CI += CI[i];
        }
        avg_CI /= key_length;
        // cout << avg_CI << " " << key_length << endl;
        if (avg_CI > 0.06) // close to 0.065
            break;
        else
            key_length++;
    }
    return key_length;
}

string get_key(string s, unsigned int key_length) {
    double p[] = {
        8.17,
        1.49,
        2.78,
        4.25,
        12.70,
        2.23,
        2.02,
        6.09,
        6.97,
        0.15,
        0.77,
        4.03,
        2.41,
        6.75,
        7.51,
        1.93,
        0.10,
        5.99,
        6.33,
        9.06,
        2.76,
        0.98,
        2.36,
        0.15,
        1.97,
        0.07,
    }; // frequency table from a to z

    string key = "";
    unsigned int string_length = s.length();
    for (int i = 0; i < key_length; i++) {
        unsigned int offset = 0;
        for (int k = 0; k < 26; k++) {
            double X = 0.0f;
            int count[26] = {0};
            for (int j = 0; i + j * key_length < string_length; j++) {
                if ((s[i + j * key_length] <= 122 && s[i + j * key_length] >= 97) || (s[i + j * key_length] <= 90 && s[i + j * key_length] >= 65)) count[(int)(s[i + j * key_length] - 'a')]++;
            }
            int L = 0;
            for (int j = 0; j < 26; j++) {
                L += count[j];
            }
            for (int j = 0; j < 26; j++) {
                X += p[j] * count[(j + offset) % 26];
            }
            if (X / L > 5.5) {
                key.push_back('a' + offset);
                break;
            } else {
                offset++;
            }
        }
    }
    return key;
}

string decrypt(string s, string key) {
    // input a string and a key for decrypting it
    // output is a string decrypted
    string ret;
    s = to_lower(s, false);
    key = to_lower(key);
    cout << "Key = " << key << endl;
    unsigned int string_length = s.length();
    unsigned int key_length = key.length();
    for (int i = 0; i < string_length; i++) {
        if (s[i] <= 122 && s[i] >= 97)
            ret.push_back((s[i] - key[i % key_length] + 26) % 26 + 'a');
        else
            ret.push_back(s[i]);
    }
    return ret;
}

string decrypt(string s) {
    // input a string and decrypt it
    // output is a string decrypted
    string ret;
    s = to_lower(s, false);
    string key = get_key(s, key_length(s));
    cout << "Key = " << key << endl;
    unsigned int string_length = s.length();
    unsigned int key_length = key.length();
    for (int i = 0; i < string_length; i++) {
        if (s[i] <= 122 && s[i] >= 97)
            ret.push_back((s[i] - key[i % key_length] + 26) % 26 + 'a');
        else
            ret.push_back(s[i]);
    }
    return ret;
}

string encrypt(string s, string key = "a", bool uppercase = true, bool delete_space = false) {
    // input a string to be encrypted and a key_length
    // output a string encrypted with the key
    string ret;
    char a;
    if (uppercase)
        a = 'A';
    else
        a = 'a';

    s = to_lower(s, delete_space);
    key = to_lower(key);
    unsigned int string_length = s.length();
    unsigned int key_length = key.length();
    for (int i = 0; i < string_length; i++) {
        if (s[i] <= 122 && s[i] >= 97)
            ret.push_back((s[i] + key[i % key_length] - 2 * (int)a) % 26 + a);
        else
            ret.push_back(s[i]);
    }
    return ret;
}

int main() {
    cout << "Running main() ..." << endl;
    string s = "";
    string line;
    ifstream ifs;
    ofstream ofs;
#if !TEST

    ifs.open("The_Communist_Manifesto.txt");
    while (getline(ifs, line)) s += line + "\n";
    ifs.close();
    string key = "TheCommunistManifesto";
    string encrypted = encrypt(s, key, true, false);
    ofs.open("Encrypted_The_Communist_Manifesto.txt");
    ofs << encrypted;
    ofs.close();
    string decrypted = decrypt(encrypted);
    ofs.open("Decrypted_The_Communist_Manifesto.txt");
    ofs << decrypted;
    ofs.close();
    cout << "Complete!" << endl;

#endif
#if TEST

    ifs.open("Encrypted.txt");
    while (getline(ifs, line)) s += line;

    s = to_lower(s);

    unsigned int keyLength;
    keyLength = key_length(s);
    cout << endl
         << "key length:" << keyLength << endl;

    string key = get_key(s, keyLength);
    cout << "key: " << key << endl
         << endl;

    string decrypted = decrypt(s, key);

    cout << "input text:" << endl
         << s << endl
         << endl;
    cout << "decrypted:" << endl
         << decrypted << endl
         << endl;

    ofs.open("Decrypted.txt");
    ofs << decrypted;

    string reen = encrypt(decrypted, key);

    string out;
    if (s.compare(to_lower(reen))) // 0 if is equal
        out = "incorrect";
    else
        out = "correct";
    cout << out << endl
         << endl; 
    cout << reen << endl;

#endif

    return 0;
}
```
