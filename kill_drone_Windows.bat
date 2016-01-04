echo off
mkdir scripts
cd scripts
del kill_all_programs_on_drone.txt
cd ..
set EXE=sheldon_position.elf

rem TELNET script for program kill
echo killall -9 %EXE% >> kill_all_programs_on_drone.txt
"C:\Program Files\MATLAB\R2014a\toolbox\idelink\foundation\hostapps\plink.exe" -telnet -P 23 192.168.1.1 < kill_all_programs_on_drone.txt