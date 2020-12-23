@REM Copyright (c) 2019, Solutions Paralint inc.
@REM All rights reserved.
@REM
@REM Redistribution and use in source and binary forms, with or without
@REM modification, are permitted provided that the following conditions are met:
@REM     * Redistributions of source code must retain the above copyright
@REM       notice, this list of conditions and the following disclaimer.
@REM     * Redistributions in binary form must reproduce the above copyright
@REM       notice, this list of conditions and the following disclaimer in the
@REM       documentation and/or other materials provided with the distribution.
@REM     * Neither the name of the <organization> nor the
@REM       names of its contributors may be used to endorse or promote products
@REM       derived from this software without specific prior written permission.
@REM
@REM THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
@REM ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
@REM WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
@REM DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
@REM DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
@REM (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
@REM LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
@REM ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
@REM (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
@REM SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


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
if not exist %PROJECT_NAME%_build mkdir %PROJECT_NAME%_build

pushd %PROJECT_NAME%_build

unzip -qo ..\%PROJECT_NAME%-src.zip

echo.
findstr /s /n DebugBreak *.c *.cpp *.h 
if ERRORLEVEL 1 (
echo Building...
    msbuild notifu\notifu.vcxproj /nologo /v:q /p:Platform=Win32;Configuration=Debug
    msbuild notifu\notifu.vcxproj /nologo /v:q /p:Platform=Win32;Configuration=Release
    msbuild notifu\notifu.vcxproj /nologo /v:q /p:Platform=x64;Configuration=Debug
    msbuild notifu\notifu.vcxproj /nologo /v:q /p:Platform=x64;Configuration=Release
    msbuild cnotifu\cnotifu.vcxproj /nologo /v:q /p:Platform=Win32;Configuration=Debug
    msbuild cnotifu\cnotifu.vcxproj /nologo /v:q /p:Platform=Win32;Configuration=Release

    echo Creating binary zip
    zip -j -q ..\%PROJECT_NAME%.zip notifu\release\%PROJECT_NAME%.exe notifu\x64\release\%PROJECT_NAME%64.exe cnotifu\release\c%PROJECT_NAME%.exe notifu\release\%PROJECT_NAME%.pdb cnotifu\release\c%PROJECT_NAME%.pdb notifu\x64\release\%PROJECT_NAME%64.pdb Sample.bat Test.bat 
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
