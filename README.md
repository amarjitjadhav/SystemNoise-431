# SystemNoise-431

If you plan on using this you have to modify the chdir command in calcCPU to be your username instead of the placeholder I have there.
Example:
    My username (aka user file for the given system) is kodavis, so I will do the following to correct the program,

        Before modification:
            chdir("/../u/<Your Username Here>/CS431/Project/");

        After I change it to be correct for me:
            chdir("/../u/kodavis/CS431/Project/"); 

(You might have to change the full path if it is differrent then above ^^)
mrh8: 
chdir("/../home/mrh8/cs431/SystemNoise-431/"); // Change back to working directory
