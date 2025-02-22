@echo off
rem 编译 storage.cpp 和 main.cpp
g++ storage.cpp main.cpp -o main

rem 如果编译成功，运行程序
if %errorlevel% == 0 (
    echo Compilation successful. Running the program...
    .\main
) else (
    echo Compilation failed. Please check the errors.
)

rem 暂停，方便查看输出
pause