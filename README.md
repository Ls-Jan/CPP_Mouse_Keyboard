# CPP_Mouse_Keyboard

本项目功能如图所示：其中“MK”为本项目的exe打包程序(38kb左右)
![工具运行截图](https://github.com/Ls-Jan/CPP_Mouse_Keyboard/blob/main/%E7%A8%8B%E5%BA%8F%E8%BF%90%E8%A1%8C%E6%88%AA%E5%9B%BE.png)

通过该工具可以以命令行或者bat脚本的方式对键鼠进行操作，从而满足一些另类需求(像是对没有Python环境或者一些键鼠控制软件的电脑进行键鼠控制啥的

<br>

***

下面的bat脚本代码，实现功能是每秒左键点击<br>
毫秒级睡眠需要外部命令程序实现，cmd原生命令里的timeout只支持秒级的<br>
只不过如果调用外部程序进行毫秒sleep的话估计精度会很差，像是sleep(10)有可能会休眠20ms之类的，当然这是后话了



```bat
chcp 936
@echo off


@echo 3秒后开始运行
timeout /t 3 >nul
@echo 开始

REM 因为没有While(True)这种死循环语句，使用goto语句来代替
REM 毫秒级休眠需要外部工具，至少cmd命令里timeout命令只能支持秒为单位的休眠
: Start
timeout /t 1 >nul
MK /C L
@echo Click
goto Start 
```

<br>
<br>

下面的bat脚本代码，则是在3秒后键入“ABCDE一二三#”以及一个回车。<br>

```bat

@echo off

timeout /t 3 >nul
Mouse_Keyboard /K ABC一二三#
Mouse_Keyboard /Ks [ENTER]

@echo on

```

<br>
<br>

