/*
 * git
 * File: HeapADT.c.c
 * date: 3/13/19
 * @author Mike Cao
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <assert.h>
#include "HeapADT.h"

struct Heap_S {
	size_t capacity;
	int(*compFun)(const void *, const void *);
	void(*dumpEntry)(const void *, FILE *);
	/*const*/ void **array;
	size_t number;
};

/*
 * parent returns the parent index of the node at the index
 * @param index the current index of the node
 */
size_t parent(size_t index) {
	return (index - 1) / 2;
}

/*
 * lChild returns the index of the left child of the node at the index
 */
size_t lChild(size_t index) {
	return 2 * index + 1;
}

/*
 * rChild returns the index of the right child of the node at the index
 */
size_t rChild(size_t index) {
	return 2 * index + 2;
}

/* 
 * createHeap constructs an empty heap.
 * @return a heap object (pointer)
 * @pre compFun and dumpEntry are not NULL.
 * @post the heap is not NULL (i.e. there was enough storage).
 */
Heap createHeap(size_t capacity, 
		int(*compFun)(const void * lhs, const void * rhs), 
		void(*dumpEntry)(const void * item, FILE * outfp)) {
	Heap newHeap;
       	newHeap = (Heap)malloc(sizeof(struct Heap_S));
	//TODO check and redo if needed
	//void **array = (void**)calloc(capacity, sizeof(void*));
	// check if dumpEntry and compFun is not NULL, otherwise assert failure
	// and abort
	assert(dumpEntry != NULL && compFun != NULL);
	// if newHeap is not null...
	if(newHeap != NULL) {
		newHeap->capacity = capacity;
		newHeap->compFun = compFun;
		newHeap->dumpEntry = dumpEntry;
		newHeap->array = NULL;
		newHeap->number = 0;
	}
	return newHeap;
}

/*
 * heapFree deallocates everything inside aHeap but not aHeap itself
 * @param aHeap the subject heap
 */
void helpFree(Heap aHeap) {
	if(aHeap->number != 0) {
		for(size_t i = 0; i < aHeap->capacity; i++) {
			if(aHeap->array[i] != NULL) {
				free(aHeap->array[i]);
				aHeap->array[i] = NULL;
			} 
		}
		free(aHeap->array);
		aHeap->array = NULL;
	}
	// reset aHeap's capacity and number
	aHeap->capacity = 0;
	aHeap->number = 0;
}

/* 
 * destroyHeap deletes all dynamic heap storage.
 * @param aHeap the subject heap
 * @pre aHeap is a valid heap data type reference.
 * @pre client previously removed and deleted all entries that
 * involved dynamic memory allocation.
 * @post the aHeap reference is no longer valid.
 */
void destroyHeap(Heap aHeap) {
	assert(aHeap != NULL);
	helpFree(aHeap);
	free(aHeap);
}

/* 
 * @param aHeap the subject heap
 * @return the current number of active entries in the heap.
 * @pre aHeap is a valid heap data type reference.
 */
size_t sizeHeap(Heap aHeap) {
	return aHeap->number;
}

/* 
 * topHeap returns an immutable pointer to the topmost item in the heap.
 * @param aHeap the subject heap
 * @return an immutable pointer to the topmost item.
 * @pre aHeap is a non-empty, valid heap data type instance.
 * @post the internal state of aHeap has not changed.
 */
const void * topHeap(const Heap aHeap) {
	const void *topHead = aHeap->array[0];
	return topHead;
}

/* 
 * removeTop removes the topmost item from the heap.
 * @param aHeap the subject heap
 * @return a pointer to the removed item.
 * @pre aHeap is a non-empty, valid heap data type instance.
 * @post client assumes ownership of the item removed.
 */
void * removeTopHeap(Heap aHeap) {
	void *topHead = aHeap->array[0];
	// manipulate	
	return topHead;
}

/*
 * siftUp moves the value at index to its proper spot in the heap
 * @param aHeap the heap
 * @param index the index of the current node
 */
void siftUp(Heap aHeap, size_t index) {
	void **array = (void**)calloc(aHeap->capacity, sizeof(void*));
	array = aHeap->array;
	while(index > 0 && 
			(aHeap->compFun(array[parent(index)], array[index]))) {
		void* temp = (void*)malloc(sizeof(void*));
		/*temp = array[parent(index)];
		array[parent(index)] = array[index];
		array[index] = temp;
		index = parent(index);*/
	}
}

/*
 * looks that ihe index, its left and right child and returns the index of the
 * value that should come first
 * @param aHeap the heap
 * @param index the index of the current node
 */
size_t first_of_three(Heap aHeap, size_t index) {
	size_t lt = lChild(index);
	size_t rt = rChild(index);
	void* value = aHeap->array[index];
	if(rt < aHeap->number) {
		void* lVal = aHeap->array[lt];
		void* rVal = aHeap->array[rt];
		if(aHeap->compFun(lVal, value) || aHeap->compFun(rVal, value)) {
			if(aHeap->compFun(lVal, rVal)) {
				return lt;
			} else {
				return rt;
			}
		} else {
			return index;
		}
	} else if(lt < aHeap->number) {
		void* lVal = aHeap->array[lt];
		if(aHeap->compFun(lVal, value)) {
			return lt;
		} else {
			return index;
		}
	} else {
		return index;
	}
}

/*
 * siftDwon moves the value at index down to its proper spot
 * @param aHeap the heap
 * @param index the index of the current node
 */
void siftDown(Heap aHeap, size_t index) {
	size_t curr = index;
	void **array = aHeap->array;
	size_t swapIndex = first_of_three(aHeap, curr);
	while(swapIndex != curr) {
		void* temp = array[swapIndex];
		array[swapIndex] = array[curr];
		array[curr] = temp;
		curr = swapIndex;
		swapIndex = first_of_three(aHeap, curr);
	}
}

/* 
 * insertHeapItem inserts an item into the heap.
 * @param aHeap the subject heap
 * @param item the item to insert into the heap
 * @pre aHeap is a valid heap data type instance.
 * @post heap assumes (temporary) ownership/custody of the item, and
 * the client must not delete the memory while heap owns it.
 */
void insertHeapItem(Heap aHeap, const void * item) {
	aHeap->number++;
	//if(aHeap->number == ??? check py fle
	// TODO memory leak
	//void* temp = (void*)malloc(sizeof(item));
	/*temp =*///aHeap->array[aHeap->number] = item;
	//temp = item;
	siftUp(aHeap, aHeap->number);
	// increase number by one since we're inserting one item 
	//aHeap->number++;	
}

/* 
 * dumpHeap prints the heap content to the specified output file stream.
 * @param aHeap the subject heap
 * @param outfp the output file stream
 * @pre aHeap is a valid heap data type instance.
 * @post the internal state of aHeap has not changed.
 */
void dumpHeap(Heap aHeap, FILE * outfp) {
	// TODO prints the entries to outfp
	for(size_t i = 0; i < aHeap->number; i++) {
		aHeap->dumpEntry(aHeap->array[i], outfp);
	}
}
