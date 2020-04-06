
@echo off
:: 默认x86

if "%1%" == "x64" (
    path D:\4.backup\VC\Tools\MSVC\14.12.25827\bin\Hostx64\x64\
) else (
    path D:\4.backup\VC\Tools\MSVC\14.12.25827\bin\Hostx86\x86\
)

nmake /f .\makefile.txt
