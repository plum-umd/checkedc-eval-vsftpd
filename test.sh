#!/bin/bash
set -e
cp sysstr.c .tmp.c
cp -r -n /home/cc/Downloads/vsftpd-3.0.3/* .
make -j3 sysstr.o || exit 1
make clean
cconv-standalone -alltypes --output-postfix=checked $(jq -r ".[] | .file" < compile_commands.json) 2> err.txt 1> /dev/null || true
rename -f "s/.checked//" *.checked.c *.checked.h
make -j3 sysstr.o 2> err.txt 1>/dev/null || true
grep -F "cast source bounds are too narrow for '_Ptr<char>'" err.txt || exit 1
mv .tmp.c sysstr.c
exit 0
