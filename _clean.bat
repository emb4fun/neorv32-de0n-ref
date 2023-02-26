@echo off

del *.bak /S
cd hw
call _clean.bat
cd ..

cd sw\coremark
call _clean.bat
cd ..\..

cd sw\dhrystone
call _clean.bat
cd ..\..

cd sw\crypto
call _clean.bat
cd ..\..
