@echo off
g++ storage.cpp main.cpp -o main
if %errorlevel% == 0 (
    echo Compilation successful. Running the program...
    .\main
) else (
    echo Compilation failed. Please check the errors.
)
pause