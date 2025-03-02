@echo off
g++ main.cpp Record.cpp Block.cpp Storage.cpp BPlusTree.cpp -o storage_test
if %errorlevel% == 0 (
    echo Compilation successful. Running the program...
    .\storage_test
) else (
    echo Compilation failed. Please check the errors.
)
pause