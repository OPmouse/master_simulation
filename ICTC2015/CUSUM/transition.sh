#!/bin/sh

#./cusum 1012 > 25per.txt
#echo "1"
#./cusum 1524 > 50per.txt
#echo "2"
#./cusum 2036 > 75per.txt
#echo "3"
#./cusum 1536 > 50per_v2.txt
gcc -o cusum cusum.c -lm
echo "compile OK."

./cusum  405 >> per.txt
echo "10%"
./cusum  610 >> per.txt
echo "20%"
./cusum  814 >> per.txt
echo "30%"
./cusum  1019 >> per.txt
echo "40%"
./cusum  1224 >> per.txt
echo "50%"
./cusum  1429 >> per.txt
echo "60%"
./cusum  1634 >> per.txt
echo "70%"
./cusum  1838 >> per.txt
echo "80%"
./cusum  2042 >> per.txt
echo "90%"
