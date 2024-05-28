#!/usr/bin/env bash

echo one two | awk '{print $1}'
id=$(echo one two | awk '{print $1}')
echo $id

array=()
array+=($id)
echo array
echo $array

id2=$(echo one two | awk '{print $2}')
array+=($id2)
echo array2
echo ${array[@]}

echo one two | awk '{print $2}'

function_name () {
   echo "Parameter #1 is $1"  # arguments in bash are referred to using their "index"
}
arg1="ew bash"
function_name "$arg1"

sample() {
    str=$(IFS=,; echo "$*")  # $* is all arguments
    echo $str
}
sample "${array[@]}" # i'm trying to pass this array into sample(), which converts it to a string