#!/bin/bash

#Source used to help write concurrent bash script on multiple servers:
#https://www.tompurl.com/2015-08-06-running-a-job-on-multiple-servers.html

echo "Enter username:"
read username
cd "/home/$username/cs431/SystemNoise-431/ftqV110/ftq"

echo "In directory: /home/$username/cs431/SystemNoise-431/ftqV110/ftq"

echo "Is this correct? (y/n)"
read response
if [ "$response" = "n" ]
then
	echo "Enter path to collect_data_script_v2.sh: "
	read filepath
	cd "$filepath"
fi

echo "Enter directory name to store data:"
read data_dir

#create directory to store data in as well as the all folder
mkdir $data_dir
mkdir "$data_dir/all"
mkdir "$data_dir/all/ftq"
mkdir "$data_dir/all/fwq"
mkdir "$data_dir/all/t_ftq"
mkdir "$data_dir/all/t_fwq"

echo "Data directory created ($data_dir)."

echo "Enter number of threads per node (recommended 4)(1..):"
read num_threads

filename="wyeast$node_num"

echo "Run benchmarks on nodes 01 - 17? (y/n)"
read response

i=1
if [ "$response" = "y" ];
then
	echo "in if statement..."
	servers="wyeast01 wyeast02 wyeast03 wyeast04 wyeast05 wyeast06 wyeast07 wyeast08 wyeast09 wyeast10 wyeast11 wyeast12 wyeast13 wyeast14 wyeast15 wyeast16 wyeast17"
	#servers="wyeast01 " 
	for server in $servers; do
		echo "./collect_data_script_v2.sh $username $i $num_threads $data_dir"
		ssh $server "bash -s" < ./collect_data_script_v2.sh $username $i $num_threads $data_dir &
		((++i))
	done
	wait
fi
