@echo off

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

SET includes=
SET links=
SET defines=

echo "Building main..."

cl /EHsc %includes% %defines% src/main.cpp %links%