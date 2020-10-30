#! /usr/bin/env bash
3c --use-malloc=vsf_sysutil_malloc -dump-stats -alltypes --output-postfix=checked $(jq -r ".[] | .file" < compile_commands.json)
rename -f "s/.checked//" *.checked.c *.checked.h
