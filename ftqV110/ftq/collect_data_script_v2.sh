# Matthew R. Hubbard
# 3/11/2019
# CS431 Karen Karavanic

#ex.
#./collect_data_script_v2.sh <username> <cluster num> <number of threads>

#This script will run 4 noise benchmarks (ftq, t_ftq, fwq, t_fwq).
#It will run fixed time quanta and fixed work quanta for both single and threaded.
#All data will be saved in a data folder in the same directory as the benchmark code.
#In that data folder, a folder will be made that is a number corresponding to the cluster node
#as specified by user input.
#Inside that folder there are 4 folders each corresponding to the 4 different benchmarks.
#It will also write data to the data/all/ directory which also has 4 respective directories.

# ex. of file structure:
#			data
#		/		\
# 		01		all
#		|		| 
#(ftq, t_ftq, fwq, t_fwq) (ftq, t_ftq, fwq, t_fwq)

#!/bin/bash

num_samples=30000 #(-n) number of samples the benchmark will record
num_threads=4 #(-t) number of threads for threaded benchmark
bits_i=20 #(-i) Bits in sampling interval limits (FTQ only). Number of loop iterations is 2^i.
bits_w=20 #(-w) Bits in work amount (FWQ only). Number of loop iterations is 2^w.
node_num=9999 #default cluster node number if none entered (used for filename)
username=$1 #argument 1 is the username
node_num=$2 #argument 2 is the node number
num_threads=$3 #argument 3 is the number of threads
data_dir=$4 #name of the directory to store data in

#echo "Enter username:"
#read username
cd "/home/$username/cs431/SystemNoise-431/ftqV110/ftq"

#echo "Directory: /home/$username/cs431/SystemNoise-431/ftqV110/ftq"

#echo "Enter node number (01-17):"
#read node_num

#echo "Enter number of threads (1..):"
#read num_threads

filename="wyeast$node_num"

#make directory for node data in data folder
rm -r $data_dir/$node_num
mkdir $data_dir/$node_num

echo "Filename: $filename"
echo "Number of samples: $num_samples"
echo "Number of Threads: $num_threads"
echo "Bits in sampling interval limits (-i): $bits_i"
echo "Bits in work amount (-w): $bits_w"

echo "Running make clean..."
make clean
echo "Running make..."
make

#----ftq single----#
echo "----ftq single----"
echo "Executing ./ftq -o $filename -i $bits_i -n $num_samples..."
./ftq -o $filename -i $bits_i -n $num_samples
echo "Creating folder $data_dir/$node_num/ftq..."
#make ftq dir to store results
mkdir $data_dir/$node_num/ftq
echo "Copying wyeast$node_num*.dat to $data_dir/all "
cp wyeast$node_num*.dat $data_dir/all/ftq
echo "Moving wyeast$node_num*.dat to $data_dir/$node_num/ftq..."
mv wyeast$node_num*.dat $data_dir/$node_num/ftq

#----ftq threaded----#
echo "----ftq threaded----"
echo "Executing ./t_ftq -o $filename -i $bits_i -n $num_samples -t $num_threads..."
./t_ftq -o $filename -i $bits_i -n $num_samples -t $num_threads
echo "Creating folder $data_dir/$node_num/t_ftq..."
#make t_ftq dir to store results
mkdir $data_dir/$node_num/t_ftq
echo "Copying wyeast$node_num*.dat to $data_dir/all "
cp wyeast$node_num*.dat $data_dir/all/t_ftq
echo "Moving wyeast$node_num*.dat to $data_dir/$node_num/t_ftq..."
mv wyeast$node_num*.dat $data_dir/$node_num/t_ftq

#----fwq single----#
echo "----fwq single----"
echo "Executing ./fwq -o $filename -w $bits_w -n $num_samples..."
./fwq -o $filename -w $bits_w -n $num_samples
echo "Creating folder $data_dir/$node_num/fwq..."
#make fwq dir to store results
mkdir $data_dir/$node_num/fwq
echo "Copying wyeast$node_num*.dat to $data_dir/all "
cp wyeast$node_num*.dat $data_dir/all/fwq
echo "Moving wyeast$node_num*.dat to $data_dir/$node_num/fwq..."
mv wyeast$node_num*.dat $data_dir/$node_num/fwq

#----fwq threaded----#
echo "----fwq threaded----"
echo "Executing ./t_fwq -o $filename -w $bits_w -n $num_samples -t $num_threads..."
./t_fwq -o $filename -w $bits_w -n $num_samples -t $num_threads
echo "Creating folder $data_dir/$node_num/t_fwq..."
#make t_fwq dir to store results
mkdir $data_dir/$node_num/t_fwq
echo "Copying wyeast$node_num*.dat to $data_dir/all "
cp wyeast$node_num*.dat $data_dir/all/t_fwq
echo "Moving wyeast$node_num*.dat to $data_dir/$node_num/t_fwq..."
mv wyeast$node_num*.dat $data_dir/$node_num/t_fwq

echo "Data has been written to $data_dir/all and $data_dir/$node_num"
echo "Done."
