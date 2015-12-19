@echo off
cls

del /S /Q project\*

premake5 --os=windows	vs2015
premake5 --os=macosx	gmake
premake5 --os=linux		gmake

pause