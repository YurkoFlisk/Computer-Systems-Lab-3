#!/bin/bash

ml icc
std="-std=c++14"
sources="$2"
inputFile="$3"
CC="$4"
# Since several script instances may ran simultaneously, each one should
# have different output files. Each script instance, though, can (and will)
# reuse its own names (e.g. each produced executable will have the same name given below).
exeFile="exe$1"
tmpFile="tmp$1.txt"
logFile="log$1.txt"
rm $logFile 2> /dev/null # avoid appending to a previous log
# Compiler flags
oFlags=(O0 O1 O2 O3 Ofast)
# Testing available CPU SIMD extensions
possibleExts=(sse2 sse3 ssse3 sse4.1 sse4.2 avx)
cpuExts=()
for possibleExt in ${possibleExts[@]}; do
	if lscpu | grep Flags | grep -qw $possibleExt; then
		cpuExts+=($possibleExt)
	fi
done
echo "Supported cpu extensions: ${cpuExts[*]}" >> $logFile

echo "Log index: $1" >> $logFile

calcResAndTime() {
	local lExecTime=$( ( time -p ./$exeFile < $inputFile > $tmpFile ) 2>&1 | grep real | awk '{print $2}' )
	local lExecRes=$( cat $tmpFile | awk '{sum=(sum+$1)%1000}END{print sum}' )
	echo "$lExecRes $lExecTime";
}

echo "Testing individual -O optimization flags" >> $logFile
echo "Flag Result Time" >> $logFile
for flag in ${oFlags[@]}; do
	$CC $std -$flag -o $exeFile $sources # here can be g++ or icc
	echo "$flag $(calcResAndTime)" >> $logFile
done
echo "Testing -x optimization flags (for some supported CPU extensions) alongside -Ofast." >> $logFile
echo "Extension Result Time" >> $logFile
for ext in ${cpuExts[@]}; do
	icc $std -Ofast -x$ext -o $exeFile $sources # here only icc
	echo "$ext $(calcResAndTime)" >> $logFile
done