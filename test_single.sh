clang -cc1 pre.c || exit 1
cconv-standalone -alltypes pre.c > pre.checked.c || exit 1
clang -o /dev/null -c pre.checked.c 2> err.txt  > /dev/null
grep -F "cast source bounds are too narrow for '_Ptr<char>'" err.txt || exit 1
#grep -F "error:" err.txt && exit 1
exit 0
