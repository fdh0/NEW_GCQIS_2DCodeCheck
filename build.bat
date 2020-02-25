@rem ----------编译VS工程项目------------------------------------


SETLOCAL ENABLEDELAYEDEXPANSION
cd %~dp0
set pwd=%~dp0 
@echo off
@rem 1.设置解决方案的路径
for /f "delims=|" %%i in (*.sln) do ( set commitid=%%i)
set thesln=%commitid%
@rem 如果找不到文件，报错退出
if [%commitid%] == [] ( 
@echo Oop,Bad, Cannot find "*.sln" files
pause 
exit )

@rem set thesln=./dhtestdll.sln 
set debugOrRelease=Release
set baseOutPath=%debugOrRelease%
@rem 当前脚本所在路径
set build_root = %~dp0
@rem vcvarsall.bat所在的路径：
set vcvars="%VS100COMNTOOLS%../../VC/vcvarsall.bat"
set devenv="%VS100COMNTOOLS%../IDE/devenv.exe"

@echo Current Project:%thesln%

@echo on
@echo start compile.
@echo Compiling, please wait...
@%devenv% %thesln% /rebuild %baseOutPath% /out build.log
@endlocal

@echo off
for /f "delims=" %%i in (build.log) do (set a=%%i)
echo %a%

@rem @notepad build.log
@rem 删除编译生成的log文件

@rem 对是否编译生成进行判断，如果失败，即退出
@rem echo %a:~22,1%

for /F %%i in ('echo %a:~22,1% --short HEAD') do ( set out_flag=%%i)

if %out_flag%==1 (
	@echo Compile Successfully
) else (
	@echo Oop,Bad, Unsuccessfully
	pause
	exit
)

TIMEOUT /T 2

@rem 编译完成后进行清除-----------------------------------------

@echo Clear cache files after compile

@echo off
echo -------------------------------------------------------
cd /d %~dp0
del /S *.tmp
del /S *.positions
del /S *.obj 
del /S *.ilk 
del /S *.pdb 
del /S *.plg 
del /S *.bsc 
del /S *.trc 
del /S *.pch 
del /S *.idb 
del /S *.exp 
del /S *.sbr 
del /S *.ncb
del /S *.opt
del /S *.o
del /S *.sup
del /S *.aps 
del /S *.sdf
rd /S /q .\ipch\

@rem rd /S /q .\x64\
@rem rd /S /q .\Release\
@rem rd /S /q  .\Debug\

del /S build.log

@echo off
@rem 对于生成的dll文件，不进行删除
@rem for /r  %%i in (debug, release) do @if exist "%%i" @rd/s/q "%%i" 
echo Clean finished 
echo. & pause 