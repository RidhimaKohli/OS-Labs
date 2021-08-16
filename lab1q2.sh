#!/bin/bash
echo "This is implementation of lab1 ques 2"
currDate=$(date)
echo "Current Date is : $currDate"
hour=$(date +%-H)
min=$(date +%-M)
sec=$(date +%-S)
day=$(date +%-d)
month=$(date +%B)
year=$(date +%Y)
numNameArray=(zero one two three four five six seven eight nine ten eleven twelve thirteen fourteen fifteen sixteen seventeen eighteen nineteen )
tenArray=(zero ten twenty thirty forty fifty sixty seventy eighty ninety)

if [ $hour -lt 12 ]
 then
	am=A.M.
	hourword=${numNameArray[hour]}
else
 am=P.M.
 hour=$hour-12
 hourword=${numNameArray[hour]}
fi

function getName(){
if [ "$1" -lt 20 ]
then 
 numName=${numNameArray[$1]}
else
let  f=$1/10
let  s=$1%10
 fir=${tenArray[f]}
 secon=${numNameArray[s]}
 if [ $s -eq 0 ]
 then
	numName=${fir}
 else
 	numName="${fir} ${secon}"
 fi
fi
echo $numName
}

minword="$(getName "$min")"
secword="$(getName "$sec")"
dateword="$(getName "$day")"
let y=$year-2000
yearword="$(getName "$y")"
echo "Current time : $hourword hours $minword minutes $secword seconds $am" 
echo "Date: $dateword $month two thousand $yearword"
