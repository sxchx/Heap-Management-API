all: project

project: Info.o ListNode.o FreeList.o AllocList.o Heap.o myStringApp.o
	gcc -o project Info.o ListNode.o FreeList.o AllocList.o Heap.o myStringApp.o

ListNode.o: ListNode.c
	gcc -c ListNode.c

Info.o: Info.c
	gcc -c Info.c
	
FreeList.o: FreeList.c
	gcc -c FreeList.c
	
AllocList.o: AllocList.c
	gcc -c AllocList.c
	
Heap.o: Heap.c
	gcc -c Heap.c
	
myStringApp.o: myStringApp.c
	gcc -c myStringApp.c

clean:
	rm -rf *\.o project
