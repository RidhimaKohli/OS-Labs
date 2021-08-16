#!/bin/bash
awk  "BEGIN{srand();


for(i=0;i<10;i++){

num = rand();

print(num%101);

}}"
