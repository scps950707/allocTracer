# Usage

```sh
make
LD_PRELOAD=./malloc.so [Command]
./parse.py < logfile.txt > paired

# Draw memory allocation distribution
./plot.py < paired

# Create header and do test
./header.py < paired > test.h

gcc test.c -o test
valgrind --leak-check=yes ./test
```

Example: Distribution of launching Xilinx SDK2017.4 under Ubuntu 16.04
![](https://i.imgur.com/VASOlyA.png)
