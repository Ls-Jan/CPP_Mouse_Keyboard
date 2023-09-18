
chcp 936
@echo off


@echo 3秒后开始运行
timeout /t 3 >nul
@echo 开始

REM 因为没有While(True)这种死循环语句，使用goto语句来代替
REM 毫秒级休眠需要外部工具，至少cmd命令里timeout命令只能支持秒为单位的休眠
: Start
timeout /t 1 >nul
Mouse_Keyboard /C L
@echo Click
goto Start 




