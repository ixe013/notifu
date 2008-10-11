rem @echo off                            

setlocal

SET PROJECT_NAME=notifu

echo Getting repository URL
SET TEMP_NAME=%RANDOM%-%RANDOM%.tmp
svn info | findstr URL | gawk '{print $2}' > %TEMP_NAME%

SET /P SVN_URL= < %TEMP_NAME%
del %TEMP_NAME%

echo Checking out files
svn co -q %SVN_URL% %TEMP_NAME%\%PROJECT_NAME%

pushd %TEMP_NAME%

echo Creating source zip
zip -rp -q ..\%PROJECT_NAME%-src.zip %PROJECT_NAME%\*

pushd %PROJECT_NAME%

echo Building release configuration
for %%i in (*.sln) do msbuild %%i /nologo /v:q /p:Configuration=Release /t:Rebuild
echo Building debug configuration
for %%i in (*.sln) do msbuild %%i /nologo /v:q /p:Configuration=Debug   /t:Rebuild

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
