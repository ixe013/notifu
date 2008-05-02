@echo off

vcbuild /nologo /nohtmllog %* notifu.sln "Release|Win32"

echo ------------------------
echo Generating documentation
echo ------------------------
rd /s /q html
doxygen doxygen\notifu.doxygen
