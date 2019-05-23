# HeapManagementAPI
A system that exposes an API for managing heap memory.  Users can allocate pieces of virtual memory contiguously and then release them.  

# Usage
```bash
$ make project
gcc -c Info.c
gcc -c ListNode.c
gcc -c FreeList.c
gcc -c AllocList.c
gcc -c Heap.c
gcc -c myStringApp.c
gcc -o project Info.o ListNode.o FreeList.o AllocList.o Heap.o myStringApp.o
$ ./project -h <heapSize> -a [first|best|worst]
```

# Input
Input is read in from standard input. Each line must contain an allocation or deallocation instruction. 
For example "+ Hello" will allocate the string "Hello" and "- Hello" will deallocate the string.
Here is an example of this:
```bash
$ ./project -h 16000 -a best

+ Hello
Allocate: Hello
Address: 0x7fda60801000
- Hello
Deallocate: Hello
Address: 0x7fda60801000
```
Three example input files have been provided: traceSmall.txt, traceMedium.txt, traceLarge.txt
Here is an example of using an input file:
```bash
$ ./project -h 128000 -a best < traceLarge.txt 
```

# Output
Each of the provided input files deallocate all strings that has been allocated.  Each allocation and deallocation is printed to standard output.  If the heap runs out of space, the application will stop running and print out heap status. 
Here is an example of this:
```bash

Ratio of heap - Used : Free - 14313 : 1687
Number of Free Holes: 2
Average Size of Free Holes: 843
Number of Allocated Chunks: 384

$
```
Here is example heap status if the entire input file is processed successfully:
```bash

Ratio of heap - Used : Free - 0 : 128000
Number of Free Holes: 1
Average Size of Free Holes: 128000
Number of Allocated Chunks: 0

$
```
   
