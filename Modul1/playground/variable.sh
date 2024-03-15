#!/bin/bash

str1="ini string"
str2='ini juga string'

echo $str1
echo $str2

num1=19
num2=12.5

echo $num1
echo $num2

arr1=('satu' 'dua' 'tiga')
arr2=(1 2 3)
arr3=('satu' 2 'tiga')

echo ${arr1[0]}
echo ${arr2[*]}
echo ${#arr3[*]}
