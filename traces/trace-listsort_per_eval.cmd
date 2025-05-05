# Test the performance of listsort from Linux kernel
option fail 0
option malloc 0
new
ih RAND 800000
listsort
free
