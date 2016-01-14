#!/bin/sh
# Supposed to work on Cygwin

echo "binary
put ${1:-main.elf}" | ftp -nv 192.168.1.1
telnet 192.168.1.1
