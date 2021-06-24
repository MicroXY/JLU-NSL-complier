@REM @Author: 远浪
@REM @Date:   2021-04-08 21:24:42
@REM @Last Modified by:   jlu
@REM Modified time: 2021-04-21 10:34:45
@echo off
setlocal enabledelayedexpansion
echo 请输入想要编译的程序:

set /p input=

"词法分析程序.exe" %input%

if %errorlevel%==0 (
	echo 词法分析失败,程序结束 
	goto:exceptionEnd
)

echo.&echo "========================================"
echo.&echo "========================================"
choice /c yn /m "请选择语法分析程序 (LL1)语法分析 按Y 递归下降语法 按N"
if %errorlevel%==1 (
	"语法分析LL(1) - 带语法树.exe"
	if !errorlevel!==0 (
		echo 文法分析失败,程序结束
	 	goto:exceptionEnd
	)
)
if %errorlevel%==2 (
	"递归下降.exe"
	if !errorlevel!==0 (
		echo 文法分析失败,程序结束
	 	goto:exceptionEnd
	)
)

"语义分析程序.exe"
if %errorlevel%==0 (
	echo 语义分析失败,程序结束
 	goto:exceptionEnd
)

echo.&echo "========================================"
echo.&echo "========================================"
choice /c yn /m "是否要生成目标代码 是请按Y 否请按N"
if %errorlevel%==1 ("SNLtransform.exe"
echo.&echo 目标代码生成成功
)

echo.&echo "========================================"
echo.&echo "========================================"
choice /c yn /m "是否要显示TOKEN 是请按Y 否请按N"
if %errorlevel%==1 type token.txt 

echo.&echo "========================================"
echo.&echo "========================================"
choice /c yn /m "是否显示语法树 是请按Y 否请按N"
if %errorlevel%==1 type "tree.txt"
echo.&echo "========================================"
echo.&echo "========================================"
echo.&echo 编译成功
pause

:exceptionEnd
pause