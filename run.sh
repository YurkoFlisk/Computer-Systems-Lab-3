#!/bin/bash

ml icc
std="-std=c++14"
sources="main.cpp"
oFlags=(O0 O1 O2 O3 Ox Ofast)
cpuExts=(sse sse2)
# Since several script instances may ran simultaneously, each one should
# have different output files. Each script instance, though, can (and will)
# reuse its own names (e.g. each produced executable will have the same name given below).
exeFile="exe$1"
tmpFile="tmp$1.txt"
logFile="log$1.txt"
rm logFile
timeCommand="(time ./$exeFile < $inputFile > $tmpFile) 2>&1 | grep real | awk '{print \$2}'"
resCommand="cat $tmpFile | awk '{print \$1}'"
echo "Testing individual -O optimization flags" >> logFile
echo "Flag Time" >> logFile
for flag in ${oFlags[@]}; do
	icc $std -$flag -o $exeFile $sources
	execTime=`$timeCommand`
	execRes=`$resCommand`
	echo "$flag $execRes $execTime" >> logFile
done
echo "Testing -x optimization flags (for some supported CPU extensions) alongside -O1." >> logFile
echo "Extension Time" >> logFile
for ext in ${cpuExts[@]}; do
	icc $std -x$ext -o $exeFile $sources
	execTime=`$timeCommand`
	execRes=`$resCommand`
	echo "$flag $execRes $execTime" >> logFile
done