# SystemNoise-431

If you plan on using this you have to modify the chdir command in calcCPU to be your username instead of the placeholder I have there.
Example:
    My username (aka user file for the given system) is kodavis, so I will do the following to correct the program,

        Before modification:
            chdir("/../home/<Your Username Here>/");

        After I change it to be correct for me:
<<<<<<< HEAD
            chdir("/../u/kodavis/CS431/Project/"); 

(You might have to change the full path if it is differrent then above ^^)
mrh8: 
chdir("/../home/mrh8/cs431/SystemNoise-431/"); // Change back to working directory
=======
            chdir("/../home/kodavis/"); 

The idea is to modify the call to chdir() to place you back in your working directory so that the program can begin writing data to the output files.
>>>>>>> 65047f33a90357f06ab31912a6e6491b8d5b582f
