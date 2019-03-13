# SystemNoise-431

If you plan on using this you have to modify the chdir command in calcCPU to be your username instead of the placeholder I have there.
Example:
    My username (aka user file for the given system) is kodavis, so I will do the following to correct the program,

        Before modification:
            chdir("/../<PATH TO WORKING DIRECTORY>");

        After I change it to be correct for me:
            chdir("/../home/kodavis/"); 

The idea is to modify the call to chdir() to place you back in your working directory so that the program can begin writing data to the output files. 
