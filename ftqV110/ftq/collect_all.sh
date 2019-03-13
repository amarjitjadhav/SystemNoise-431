#!/bin/bash

#Source used to help write concurrent bash script on multiple servers:
#https://www.tompurl.com/2015-08-06-running-a-job-on-multiple-servers.html

echo "Enter username:"
read username
filepath="/home/$username/cs431/SystemNoise-431/ftqV110/ftq"
filepath2="/home/mrh8/cs431/SystemNoise-431"

cd "$filepath"

echo "Directory with collect_data_script_v2.sh: $filepath"

echo "Is this correct? (y/n)"
read response
if [ "$response" = "n" ]
then
	echo "Enter path to collect_data_script_v2.sh: "
	read filepath
	cd "$filepath"
fi

echo "Directory with collect_cpu.sh and run_cpu.sh: $filepath2"

echo "Is this correct? (y/n)"
read response
if [ "$response" = "n" ]
then
	echo "Enter path to collect_cpu.sh and run_cpu.sh: "
	read filepath2
fi

echo "Enter unique directory name to store data:"
read data_dir

echo "Try to remove dir $data_dir before creating? (y/n)"
read response
if [ "$response" = "y" ]
then
	rm -r $data_dir
fi

#create directory to store data in as well as the all folder
mkdir $data_dir
mkdir "$data_dir/all"
mkdir "$data_dir/all/ftq"
mkdir "$data_dir/all/fwq"
mkdir "$data_dir/all/t_ftq"
mkdir "$data_dir/all/t_fwq"

echo "Data directory created ($data_dir)."

for i in {1..17}
do
	mkdir -p $data_dir/$i
	mkdir -p $data_dir/$i/ftq
	mkdir -p $data_dir/$i/fwq
	mkdir -p $data_dir/$i/t_ftq
	mkdir -p $data_dir/$i/t_fwq
	mkdir -p $data_dir/$i/cpu
done

echo "Created all folders for the nodes 01-17"

echo "Enter number of threads per node (recommended 4)(1..):"
read num_threads

filename="wyeast$node_num"

servers="wyeast01 wyeast02 wyeast03 wyeast04 wyeast05 wyeast06 wyeast07 wyeast08 wyeast09 wyeast10 wyeast11 wyeast12 wyeast13 wyeast14 wyeast15 wyeast16 wyeast17"
#servers="wyeast01 " 


#-------RUN CPU USEAGE CODE ON EVERY NODE-------#
cd "$filepath2"
./collect_cpu.sh $filepath $filepath2 $data_dir

#echo "Sleeping for 20 sec to wait for cpu code to finish..."
#sleep 20s #wait to make sure it's completed before running benchmarks

echo "Running python script to get frequency..." 
python3 counter.py names.dat freq.dat
echo "---Freq has been written to freq.dat---"

cd "$filepath"
mkdir "$data_dir/cpu"
echo "Putting copy of cpu usage data into $data_dir/cpu/..."
cp ../../*.dat $data_dir/cpu

#-------RUN BENCHMARKSS ON EVERY NODE-------#
echo "Run benchmarks on nodes 01 - 17? (y/n)"
read response

i=1
if [ "$response" = "y" ];
then
	cd "$filepath"
	for server in $servers; do
		echo "./collect_data_script_v2.sh $username $i $num_threads $data_dir"
		ssh $server "bash -s" < ./collect_data_script_v2.sh $username $i $num_threads $data_dir &
		((++i))
	done
	wait
fi
