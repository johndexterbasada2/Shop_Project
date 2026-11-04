@echo off
chcp 65001 >nul
set PATH=C:\msys64\ucrt64\bin;%PATH%
echo Building BlitzShop...
set FILES=
for %%f in (BlitzShop\*.cpp) do call set FILES=%%FILES%% "%%f"
g++ -std=c++17 -Wall -Wextra -g %FILES% -o BlitzShop.exe
if %errorlevel% neq 0 (
    echo.
    echo Build failed. Read the error above.
    pause
    exit /b 1
)
echo Build successful. Running...
echo.
BlitzShop.exe
