#!/bin/bash
echo "This is implementation of ques 1 of lab 1"
filepath="lab1text.txt"
lineCount=$(wc --lines<$filepath)
wordCount=$(wc --word<$filepath)
echo "number of lines =  $lineCount"
echo "number of words = $wordCount"
