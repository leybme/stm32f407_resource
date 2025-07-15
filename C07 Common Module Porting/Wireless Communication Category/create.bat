@echo off
setlocal enabledelayedexpansion

REM 设置代码页为65001（UTF-8）
chcp 65001 >nul

REM 定义配置文件的路径
set "configFile=config.txt"

REM 检查配置文件是否存在
if not exist "%configFile%" (
    echo 配置文件 %configFile% 不存在!
    exit /b 1
)

REM 逐行读取配置文件并创建文件夹
for /f "usebackq delims=" %%a in ("%configFile%") do (
    set "folderName=%%a"
    if not exist "!folderName!" (
        mkdir "!folderName!"
        echo 文件夹 "!folderName!" 已创建.
    ) else (
        echo 文件夹 "!folderName!" 已经存在.
    )
)

echo 所有文件夹已处理完成.
pause
