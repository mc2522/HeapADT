/*
 * git
 * File: HeapADT.c.c
 * date: 3/13/19
 * @author Mike Cao
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "HeapADT.h" 

#define ALLOC_UNIT 5

struct Heap_S {
	size_t capacity;
	int(*compFun)(const void * lhs, const void * rhs);
	void(*dumpEntry)(const void * item, FILE * outfp);
	void **array;
	size_t number;
};

/*
 * parent returns the parent index of the node at the index
 * @param index the current index of the node
 */
static size_t parent(size_t index) {
	return (index - 1) / 2;
}

/*
 * lChild returns the index of the left child of the node at the index
 */
static size_t lChild(size_t index) {
	return 2 * index + 1;
}

/*
 * rChild returns the index of the right child of the node at the index
 */
static size_t rChild(size_t index) {
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
	Heap newHeap = (Heap)malloc(sizeof(struct Heap_S));
	// allocate memory to array
	void **array = (void**)calloc(capacity, sizeof(void*));
	// initialize array elements
	for(size_t i = 0; i < capacity; i++)
		array[i] = NULL;
	// check if dumpEntry and compFun is not NULL, otherwise assert failure
        // and abort
	assert(dumpEntry != NULL && compFun != NULL);
	// if newHeap is not null...
	if(newHeap != NULL) {
		newHeap->capacity = capacity;
		newHeap->compFun = compFun;
		newHeap->dumpEntry = dumpEntry;
		newHeap->array = array;
		newHeap->number = 0;
	}
	// return the heap
	return newHeap;
}

/*
 * heapFree deallocates everything inside aHeap but not aHeap itself
 * @param aHeap the subject heap
 */
static void helpFree(Heap aHeap) {
	if(aHeap->number != 0) {
		// deallocate everything in array and set them to null
		for(size_t i = 0; i < aHeap->capacity; i++) {
			if(aHeap->array[i] != NULL) {
				free(aHeap->array[i]);
				aHeap->array[i] = NULL;
			} 
		}			
	}
	//free the array
	free(aHeap->array);
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
	// check if aHeap is empty first
	assert(aHeap != NULL);
	// free everything in heap first
	helpFree(aHeap);
	// free the heap
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
 * siftUp moves the value at index to its proper spot in the heap
 * @param aHeap the heap
 * @param index the index of the current node
 */
static void siftUp(Heap aHeap, size_t index) {
	// point array to aHeap->array
	void **array = aHeap->array;
	// while compFun does not return 0
	while(index > 0 && 
			!(aHeap->compFun(array[parent(index)], array[index]))) {
		// swap the items
		void* temp = array[parent(index)];
		array[parent(index)] = array[index];
		array[index] = temp;
		// continue to check the parent and so on...
		index = parent(index);
	}
}

/*
 * looks that ihe index, its left and right child and returns the index of the
 * value that should come first
 * @param aHeap the heap
 * @param index the index of the current node
 */
static size_t first_of_three(Heap aHeap, size_t index) {
	// get the indexes of the left and right child
	size_t lt = lChild(index);
	size_t rt = rChild(index);
	void* value = aHeap->array[index];
	// if the right child exists...
	if(rt < aHeap->number) {	
		void* lVal = aHeap->array[lt];
		void* rVal = aHeap->array[rt];
		// if the left child or right child is greater than the value..
		if(aHeap->compFun(lVal, value) || 
				aHeap->compFun(rVal, value)) {
			// check whether the left or right child should sift up
			if(aHeap->compFun(lVal, rVal)) {
				return lt;
			} else {
				return rt;
			}
		} else {
			// return the original index
			return index;
		}
	// check if the left child exists...
	} else if(lt < aHeap->number) {
		void* lVal = aHeap->array[lt];
		// return the left child index if the value is greater
		if(aHeap->compFun(lVal, value)) {
			return lt;
		} else {
			// return the original index
			return index;
		}
	} else {
		// return the original index
		return index;
	}
}

/*
 * siftDwon moves the value at index down to its proper spot
 * @param aHeap the heap
 * @param index the index of the current node
 */
static void siftDown(Heap aHeap, size_t index) {
	size_t curr = index;
	void **array = aHeap->array;
	// check the item of index curr and compare it with its children
	size_t swapIndex = first_of_three(aHeap, curr);
	// while the new index is not the provided index
	while(swapIndex != curr) {
		// swap the items
		void* temp = array[swapIndex];
		array[swapIndex] = array[curr];
		array[curr] = temp;
		curr = swapIndex;
		// check the values
		swapIndex = first_of_three(aHeap, curr);
	}
}

/*
 * removeTop removes the topmost item from the heap.
 * @param aHeap the subject heap
 * @return a pointer to the removed item.
 * @pre aHeap is a non-empty, valid heap data type instance.
 * @post client assumes ownership of the item removed.
 */
void * removeTopHeap(Heap aHeap) {
	// remove the first item and sift down after bringing the last item to
	// the top
        void *topHead = aHeap->array[0];
        aHeap->array[0] = aHeap->array[aHeap->number - 1];
        aHeap->number--;
        siftDown(aHeap, 0);
        return topHead;
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
	// check if the size of the heap is at max capacity...
	if(aHeap->number == aHeap->capacity) {
		// increase the capacity
		aHeap->capacity = aHeap->capacity + ALLOC_UNIT;
		// reallocate memort to the array with its increased capacity
		aHeap->array= (void**)realloc(aHeap->array, 
				sizeof(void*) * aHeap->capacity);
	}
	// add the item to the end of the array
	aHeap->array[aHeap->number] = (void*)item;
	// sift up the item if necessary
	siftUp(aHeap, aHeap->number);
	// increase number by one since we're inserting one item 
	aHeap->number++;	
}

/* 
 * dumpHeap prints the heap content to the specified output file stream.
 * @param aHeap the subject heap
 * @param outfp the output file stream
 * @pre aHeap is a valid heap data type instance.
 * @post the internal state of aHeap has not changed.
 */
void dumpHeap(Heap aHeap, FILE * outfp) {
	// dump the entries to the provided file
	for(size_t i = 0; i < aHeap->number; i++) {
		aHeap->dumpEntry(aHeap->array[i], outfp);
	}
}
