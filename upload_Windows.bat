echo off
mkdir scripts
cd scripts
del scripts/ftpcmd.dat
del scripts/generated_cmds_del.txt
set EXE=sheldon_position.elf

rem FTP uploading
cd scripts
echo open 192.168.1.1 >> ftpcmd.dat
echo user >> ftpcmd.dat
echo put %EXE% >> ftpcmd.dat
echo disconnect >> ftpcmd.dat
echo quit >> ftpcmd.dat
cd ../build
ftp -s:../scripts/ftpcmd.dat

rem TELNET script for program start
cd ../scripts
echo killall -9 %EXE% >> generated_cmds_del.txt
echo killall -9 program.elf >> generated_cmds_del.txt
echo cd data/video >> generated_cmds_del.txt
echo insmod modules/cdc-acm.ko >> generated_cmds_del.txt
echo chmod +x %EXE% >> generated_cmds_del.txt
echo ./%EXE% >> generated_cmds_del.txt
cd ..
"C:\Program Files\MATLAB\R2014a\toolbox\idelink\foundation\hostapps\plink.exe" -telnet -P 23 192.168.1.1 < scripts/generated_cmds_del.txt

rm -rf scripts