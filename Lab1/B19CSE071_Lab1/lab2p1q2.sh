#!/bin/bash
echo "Part 1 Q2"
file="commands.txt"
linescount=$(wc --lines<$file)
wordscount=$(wc --word<$file)
bytecount=$(wc --c<$file)
outputfile=out.txt

finalOut="num of lines : $linescount num of words : $wordscount num of bytes : $bytecount"
echo $finalOut>$outputfile
