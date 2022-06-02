REM How to Use this bat file 
REM Follow step by step below
:: Install or Download MingGW and save it in a specific folder
:: Download Freeglut and config it below
::   -copy include and bin folder into MinGW
::   -go to "bin folder" and copy freeglut.dll in to System32 and SysWOW64
:: Extract the bat.(zip) and copy it into your folder
:: set your project name like this "main.c"
:: Run This.bat 
:: Done

del main.exe
del main.o

REM
echo "Running \m"
gcc -c -o main.o main.cpp -I\"D:\CodeBlocks\MinGW\x86_64-w64-mingw32\include"
cls
echo "Compiling Graphic"
gcc -o main.exe  main.o -L\"D:\CodeBlocks\MinGW\x86_64-w64-mingw32\lib -lopengl32 -lglu32 -lfreeglut"
cls
echo "App Running"
main.exe