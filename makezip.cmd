@echo off                            

setlocal

SET PROJECT_NAME=notifu
SET VCBUILD_DEFAULT_CFG=

echo Zipping versioned project files
if exist %PROJECT_NAME%.zip del %PROJECT_NAME%.zip
if exist %PROJECT_NAME%-src.zip del %PROJECT_NAME%-src.zip
rem svn st -v | findstr /V /B "[\?CDIX\!\~]" | gawk "{ $0 = substr($0, 6); print $4 }" | zip %PROJECT_NAME%-src.zip -@ 
svn list -R | zip %PROJECT_NAME%-src.zip -@ 

echo.
echo Preparing for build
md %PROJECT_NAME%_build

pushd %PROJECT_NAME%_build

unzip -q ..\%PROJECT_NAME%-src.zip

echo.
findstr /s /n DebugBreak *.c *.cpp *.h 
if ERRORLEVEL 1 (
echo Building...
    dir /s /b *.vcxproj
    pause
    msbuild notifu\notifu.vcxproj /nologo /v:q /p:Platform=Win32;Configuration=Debug
    msbuild notifu\notifu.vcxproj /nologo /v:q /p:Platform=Win32;Configuration=Release
    msbuild notifu\notifu.vcxproj /nologo /v:q /p:Platform=Win64;Configuration=Debug
    msbuild notifu\notifu.vcxproj /nologo /v:q /p:Platform=Win64;Configuration=Release
    msbuild cnotifu\cnotifu.vcxproj /nologo /v:q /p:Platform=Win32;Configuration=Debug
    msbuild cnotifu\cnotifu.vcxproj /nologo /v:q /p:Platform=Win32;Configuration=Release

    echo Creating binary zip
    zip -j -q ..\%PROJECT_NAME%.zip release\%PROJECT_NAME%.exe x64\release\%PROJECT_NAME%64.exe release\c%PROJECT_NAME%.exe release\%PROJECT_NAME%.pdb release\c%PROJECT_NAME%.pdb x64\release\%PROJECT_NAME%64.pdb Sample.bat Test.bat 
) else (
    echo.
    echo Some files or binaries where not build. Fix it or die.
    if exist ..\%PROJECT_NAME%.zip del ..\%PROJECT_NAME%.zip
    if exist ..\%PROJECT_NAME%-src.zip del ..\%PROJECT_NAME%-src.zip
)

popd

::rd /s /q %PROJECT_NAME%_build 

echo.
dir *.zip | findstr zip
echo.
unzip -l %PROJECT_NAME%.zip *.exe *.pdb
if NOT ERRORLEVEL 0 (
echo.
echo Binary not found in distribution. Fix it or die.
if exist ..\%PROJECT_NAME%.zip del ..\%PROJECT_NAME%.zip
if exist ..\%PROJECT_NAME%-src.zip del ..\%PROJECT_NAME%-src.zip
)
echo.
echo Done.
endlocal
echo.
