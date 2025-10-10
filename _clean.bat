@echo off

del *.bak /S

cd fpga
call _clean.bat
cd ..

cd blinky
call _clean.bat
cd ..

cd blinky-tcts
call _clean.bat
cd ..

cd coremark
call _clean.bat
cd ..

cd crypto
call _clean.bat
cd ..


cd dhrystone
call _clean.bat
cd ..

cd memtest
call _clean.bat
cd ..
