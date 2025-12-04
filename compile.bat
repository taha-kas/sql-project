@echo off
echo Compiling SQLite program...
gcc -o main.exe src/main.c src/sqlite3.c -I include -Wl,--subsystem,console
if %errorlevel% == 0 (
    echo Compilation successful!
    echo Running program...
    main.exe
) else (
    echo Compilation failed!
    pause
)
