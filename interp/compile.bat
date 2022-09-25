@echo off

cd bin

set files=../src/main.cpp

CL /Zi -Fmmygame.map -W4 -GR- -EHa- -FC /std:c++20 /I include %files% /link %libs% /OUT:../bin/main.exe
main
 cd ..
pause