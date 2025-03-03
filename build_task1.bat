@echo off
g++ task1.cpp Record.cpp Block.cpp Storage.cpp BPlusTree.cpp -o task1
if %errorlevel% == 0 (
    echo Compilation successful. Running the program...
    .\task1
) else (
    echo Compilation failed. Please check the errors.
)
pause