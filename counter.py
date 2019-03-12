# Script made by Alex Edwards
# 11 March 2019

import sys

finalCount={}
with open(sys.argv[1],'r') as f:
    for line in f:
        finalCount[''.join([i for i in line if i.isalpha()])] = finalCount.get(''.join([i for i in line if i.isalpha()]),0)+1
list = []
for key in finalCount:
    list.append((finalCount[key],key))
list.sort(reverse=True)
with open(sys.argv[2],'w+') as f:
    for (count,name) in list:
        f.write("{} {}\n".format(count,name))
