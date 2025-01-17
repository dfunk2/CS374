#!/bin/bash
DIRECTORY=${1}
ls -a "$DIRECTORY" | grep -i '[aeiou]$'