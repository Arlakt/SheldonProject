#!/bin/sh
# Supposed to work on Cygwin

echo "put main.elf" | ftp -nv 192.168.1.1
telnet 192.168.1.1

