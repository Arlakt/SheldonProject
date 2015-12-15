cd build

echo open 192.168.1.1>> ftpcmd.dat
echo user>> ftpcmd.dat
echo put sheldon_position.elf>> ftpcmd.dat
echo disconnect >>ftpcmd.dat
echo quit>> ftpcmd.dat

ftp -s:ftpcmd.dat
del ftpcmd.dat
cd ..

del generated_cmds_del.txt
rem insmod data/video/modules/cdc-acm.ko
echo cd data/video>>generated_cmds_del.txt
echo insmod modules/cdc-acm.ko>>generated_cmds_del.txt
echo ./sheldon_position.elf>>generated_cmds_del.txt
"C:\Program Files\MATLAB\R2014a\toolbox\idelink\foundation\hostapps\plink.exe" -telnet -P 23 192.168.1.1 < generated_cmds_del.txt