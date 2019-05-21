@echo off
title Information Board unit test
ECHO Compiling tests...
cd tests
for %%i in (*.cpp) do (
    g++ %%i -o ../output/%%i.exe
    ECHO %%i done
)
ECHO Compilation complete!
ECHO Running tests...
cd ../output
for %%i in (*.exe) do (
    ECHO Testing %%i
    %%i
)
ECHO Press any key to exit...
pause>nul