#! /usr/bin/env bash
cconv-standalone --use-malloc=vsf_sysutil_malloc -dump-stats -alltypes --output-postfix=checked $(jq -r ".[] | .file" < compile_commands.json)
rename -f "s/.checked//" *.checked.c *.checked.h
