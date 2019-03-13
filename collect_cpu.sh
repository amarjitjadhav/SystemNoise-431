#!/bin/bash

filepath=$1 #directory of collect_data_script_v2
filepath2=$2 #directory of collect_cpu
data_dir=$3 #the directory name of where to store the data (in filepath)
#filepath2="/home/mrh8/cs431/SystemNoise-431"
cd $filepath2

echo "Run cpu useage on nodes 01 - 17? (y/n)"
read response

servers="wyeast01 wyeast02 wyeast03 wyeast04 wyeast05 wyeast06 wyeast07 wyeast08 wyeast09 wyeast10 wyeast11 wyeast12 wyeast13 wyeast14 wyeast15 wyeast16 wyeast17"
#servers="wyeast01"

run=echo test

i=1
if [ "$response" = "y" ];
then
	rm freq.dat
	rm names.dat
	make
	for server in $servers; do
		echo "wyeast$i: ./cpu"
		ssh $server "bash -s" < ./run_cpu.sh
		#copy the .dat files for the single run on that cluster node
		cp avg.dat $filepath/$data_dir/$i/cpu
		cp child.dat $filepath/$data_dir/$i/cpu
		cp total.dat $filepath/$data_dir/$i/cpu
		((++i))
		#sleep 1s
	done
	wait
fi

#echo "Sleeping for 10 sec to wait for cpu code to finish..."
sleep 2s 

echo "Running python script to get frequency..." 
python3 counter.py names.dat freq.dat
echo "---Freq has been written to freq.dat---"


