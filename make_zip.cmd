@echo off
del output\notifu_src.zip
del output\notifu.zip
zip -rp output/notifu_src.zip notifu
zip -j output/notifu.zip release/notifu.exe
