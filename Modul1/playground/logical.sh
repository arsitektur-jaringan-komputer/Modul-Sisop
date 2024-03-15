#!/bin/bash

echo '[[ 10 -gt 2 ]] && echo 1'
[[ 10 -gt 2 ]] && echo 1
echo '[[ ! 10 -gt 2 ]] || echo 1'
[[ ! 10 -gt 2 ]] || echo 1
