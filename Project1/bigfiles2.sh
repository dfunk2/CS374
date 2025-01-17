#!/bin/bash
DIRECTORY=${1}
#list Directory contents
ls -la "$DIRECTORY"
echo ====
#Sort by file size and in descending order
ls -la "$DIRECTORY' | tr -s " " | cut -d' ' -f 5,9 | sort -n -r   
echo ====
