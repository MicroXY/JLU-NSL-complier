@REM @Author: Զ��
@REM @Date:   2021-04-08 21:24:42
@REM @Last Modified by:   jlu
@REM Modified time: 2021-04-21 10:34:45
@echo off
setlocal enabledelayedexpansion
echo ��������Ҫ����ĳ���:

set /p input=

"�ʷ���������.exe" %input%

if %errorlevel%==0 (
	echo �ʷ�����ʧ��,������� 
	goto:exceptionEnd
)

echo.&echo "========================================"
echo.&echo "========================================"
choice /c yn /m "��ѡ���﷨�������� (LL1)�﷨���� ��Y �ݹ��½��﷨ ��N"
if %errorlevel%==1 (
	"�﷨����LL(1) - ���﷨��.exe"
	if !errorlevel!==0 (
		echo �ķ�����ʧ��,�������
	 	goto:exceptionEnd
	)
)
if %errorlevel%==2 (
	"�ݹ��½�.exe"
	if !errorlevel!==0 (
		echo �ķ�����ʧ��,�������
	 	goto:exceptionEnd
	)
)

"�����������.exe"
if %errorlevel%==0 (
	echo �������ʧ��,�������
 	goto:exceptionEnd
)

echo.&echo "========================================"
echo.&echo "========================================"
choice /c yn /m "�Ƿ�Ҫ����Ŀ����� ���밴Y ���밴N"
if %errorlevel%==1 ("SNLtransform.exe"
echo.&echo Ŀ��������ɳɹ�
)

echo.&echo "========================================"
echo.&echo "========================================"
choice /c yn /m "�Ƿ�Ҫ��ʾTOKEN ���밴Y ���밴N"
if %errorlevel%==1 type token.txt 

echo.&echo "========================================"
echo.&echo "========================================"
choice /c yn /m "�Ƿ���ʾ�﷨�� ���밴Y ���밴N"
if %errorlevel%==1 type "tree.txt"
echo.&echo "========================================"
echo.&echo "========================================"
echo.&echo ����ɹ�
pause

:exceptionEnd
pause