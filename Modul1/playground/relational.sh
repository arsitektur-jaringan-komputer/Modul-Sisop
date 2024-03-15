#!/bin/bash

echo 'echo "$((10 > 2))"'
echo "$((10 > 2))"
echo '[[ 10 -gt 2 ]] && echo 1'
[[ 10 -gt 2 ]] && echo 1
