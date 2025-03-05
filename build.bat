@echo off
g++ main.cpp Record.cpp Block.cpp Storage.cpp BPlusTree.cpp -o storage_test -Wl,--stack,4294967296

pause