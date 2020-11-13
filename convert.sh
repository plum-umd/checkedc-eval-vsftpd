#! /usr/bin/env bash
3c -dump-intermediate -dump-stats -alltypes --output-postfix=checked $(jq -r ".[] | .file" < compile_commands.json)
rename -f "s/.checked//" *.checked.c *.checked.h
