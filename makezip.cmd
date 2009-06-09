@echo off                            

setlocal

SET PROJECT_NAME=notifu

echo Getting repository URL
SET TEMP_NAME=.%RANDOM%_%RANDOM%.tmp
for /f "tokens=2" %%i IN ('svn info') DO @echo %%i | findstr /i /c:http >> %TEMP_NAME%


SET /P SVN_URL= < %TEMP_NAME%
del %TEMP_NAME%

echo Checking out files
svn co -q %SVN_URL% %TEMP_NAME%\%PROJECT_NAME%

pushd %TEMP_NAME%

echo Creating source zip
zip -rp -q ..\%PROJECT_NAME%-src.zip %PROJECT_NAME%\*

pushd %PROJECT_NAME%

rem Let's build all configurations
set VCBUILD_DEFAULT_CFG=
vcbuild /rebuild

echo Creating binary zip
zip -j -q ..\..\%PROJECT_NAME%.zip Sample.bat Test.bat release\%PROJECT_NAME%.exe

popd
popd

rd /s /q %TEMP_NAME%

dir *.zip | findstr zip
echo.
echo Done.
endlocal
echo.
