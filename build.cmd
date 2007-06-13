@echo off

vcbuild /nologo /nohtmllog %* notifu.sln "Release|Win32"

doxygen notifu.doxygen
