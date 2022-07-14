# Arithmetic Expression Compiler

这是一个算术表达式编译器练手项目。

## 功能介绍

支持的运算：
* 正（+）
* 负（-）
* 加（+）
* 减（-）
* 乘（*）
* 除（/）
* 模（%）
* 幂（^）
* 函数
  * 平方根 sqrt(x)
  * 对数 log(a, x)
  * 自然对数 ln(x) = log(e, x)
  * 常用对数（以10为底的对数） lg(x) = log(10, x)
  * 正弦 sin(x)
  * 余弦 cos(x)
  * 正切 tan(x)、tg(x)
  * 余切 cot(x)、ctg(x)

支持的常数写法：
* 整数 123
* 小数 1.23、0.123、.123
* 科学计数法 1e-6、1.23e4、1.24E+4
* 自然对数的底 e
* 圆周率 pi

## 项目结构

* lexer：词法分析
  * manual：手写的词法分析器
  * regexp：借助正则表达式半手写的词法分析器
  * flex：使用 lex 语法、用 flex 自动生成的词法分析器
* parser：语法分析
  * manual：手写的 LL 语法分析器
  * bison：使用 yacc 语法、用 bison 自动生成的 LR 语法分析器

## 构建方法

一、 VSCode + CMake Tools 插件（推荐）

1. Ctrl+Shift+P：CMake 配置（CMake Configure）
2. Ctrl+Shift+P：CMake 生成（CMake Build）
3. Ctrl+Shift+P：CMake 安装（CMake Install）

二、手工

```bash
mkdir build
cd build
cmake ..
cmake --build .
cmake --install .
```

然后便可运行 bin 目录下的各个可执行文件

## 输入输出说明

lexer_* 会接受一个算术表达式的输入，输出 Token 列表。
例如：

输入
```
1+2*3^4
```
输出
```
NUM 1
ADD
NUM 2
MUL
NUM 3
EXP
NUM 4
```

parser_* 会接受一个算术表达式的输入，输出：
1. 计算结果
2. 文本表示的语法树
3. graphviz 表示的语法树

例如：

输入
```
1+2*3^4
```
输出
```
163
```
```
ADD = 163
  1
  MUL = 162
    2
    EXP = 81
      3
      4
```
```dot
digraph {
  rankdir=TD;
  n0 [ label="ADD\n(=163)" shape=doublecircle ];
  n0->n1;
  n1 [ label="1" shape=circle ];
  n0->n2;
  n2 [ label="MUL\n(=162)" shape=doublecircle ];
  n2->n3;
  n3 [ label="2" shape=circle ];
  n2->n4;
  n4 [ label="EXP\n(=81)" shape=doublecircle ];
  n4->n5;
  n5 [ label="3" shape=circle ];
  n4->n6;
  n6 [ label="4" shape=circle ];
}
```