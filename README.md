## 分形是啥
> 取自百度百科

分形，具有以非整数维形式充填空间的形态特征。通常被定义为“一个粗糙或零碎的几何形状，可以分成数个部分，且每一部分都（至少近似地）是整体缩小后的形状”，即具有自相似的性质。分形（Fractal）一词，是芒德勃罗创造出来的，其原意具有不规则、支离破碎等意义。1973年，芒德勃罗（B.B.Mandelbrot）在法兰西学院讲课时，首次提出了分维和分形的设想。

## 工程依赖
1. OpenGL
2. SDL  2.0+
3. Eigen    3.1+
4. Win系统下测试可正常运行

## 项目介绍
一共分成三个部分，(a)谢尔宾斯基三角(Sierpinski)二维及三维实现，(b)门格海绵(Menger)二维及三维实现，(c)“花椰菜”(Broccoli)三维实现。

其中二维的两个成像都是有参考书籍和他人博客，三维部分均为自己实现。“花椰菜”的实现参考论文：Procedural Flower Generation Suryaveer Singh Lodha and Jeremy Williams University of California - Berkeley

## 程序结果及3D打印结果
#### 谢尔宾斯基三角-三维成像
<div align="center"> <img src="/image/三角.png" width="500"/> </div><br>
<div align="center"> <img src="/image/三角2.jpg" width="500"/> </div><br>

#### 门格海绵-三维成像
<div align="center"> <img src="/image/方格.png" width="500"/> </div><br>
<div align="center"> <img src="/image/方格2.jpg" width="500"/> </div><br>

#### 花椰菜-三维成像
高迭代次数结果：
<div align="center"> <img src="/image/花椰菜.png" width="500"/> </div><br>
低迭代次数结果:
<div align="center"> <img src="/image/花椰菜低迭代次数.png" width="500"/> </div><br>
论文中用Maya实现和实物对比结果:
<div align="center"> <img src="/image/理想结果.png"/> </div><br>

## TODO
1.花椰菜的实现相对来说还存在一定问题，一方面是迭代次数上去之后机子会炸，所以头部的花椰菜没有绘制。可能可以用其他近似方法去逼近。
2.光影等等着色和渲染方面很糟糕，目前只是简单的用红绿去区别，shader这部分没有去做。
