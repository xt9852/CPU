@echo off

:: 默认x86,设置nmake的目录
set MSVC="D:\4.backup\VC\Tools\MSVC\14.12.25827\bin"

if "%1%" == "x64" (
    path %MSVC%\Hostx64\x64\
) else (
    path %MSVC%\Hostx86\x86\
)

nmake /f .\makefile.txt
