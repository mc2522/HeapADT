// RCS: $Id$ 
// File: test_HeapADT.c 
// 
// test_HeapADT.c tests a heap implementation.
// 
// @author bks: ben k steele
// // // // // // // // // // // // // // // // // // // // // // // // 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "HeapADT.h"

/// less-than-or-equal compares entries as pointers to int values.
/// @param item1 pointer to the left hand side of the comparison
/// @param item2 pointer to the right hand side of the comparison
/// @return non-zero if *item1 <= *item2
/// @pre values are pointers to integer values.
///
static int cmpintmin( const void * item1, const void * item2 ) {
    int v1 = *(int*)item1;
    int v2 = *(int*)item2;
    return v1 <= v2;
}

/// greater-than-or-equal compares entries as pointers to int values.
/// @param item1 pointer to the left hand side of the comparison
/// @param item2 pointer to the right hand side of the comparison
/// @return non-zero if *item1 >= *item2
/// @pre values are pointers to integer values.
///
static int cmpintmax( const void * item1, const void * item2 ) {
    int v1 = *(int*)item1;
    int v2 = *(int*)item2;
    return v1 >= v2;
}

/// prints entry as an int value.
/// @param item1 pointer to the value to print
/// @param outfp pointer to output file stream
/// @pre values are pointers to integer values.
///
static void dumpint( const void * item1, FILE * outfp ) {
    if ( ! item1 ) {
        printf( "WARNING: null entry\n" );
        return ;
    }
    int v1 = *(int*)item1;
    fprintf( outfp, "%d\n", v1 );
    return ;
}


/// less-than-or-equal compares entries as pointers to char values.
/// @param item1 pointer to the left hand side of the comparison
/// @param item2 pointer to the right hand side of the comparison
/// @return non-zero if *item1 <= *item2
/// @pre values are pointers to char values.
///
static int cmpcharmin( const void * item1, const void * item2 ) {
    char v1 = *(char*)item1;
    char v2 = *(char*)item2;
    return v1 <= v2;
}

/// greater-than-or-equal compares entries as pointers to char values.
/// @param item1 pointer to the left hand side of the comparison
/// @param item2 pointer to the right hand side of the comparison
/// @return non-zero if *item1 >= *item2
/// @pre values are pointers to char values.
///
static int cmpcharmax( const void * item1, const void * item2 ) {
    char v1 = *(char*)item1;
    char v2 = *(char*)item2;
    return v1 >= v2;
}

/// prints entry as a single char value.
/// @param item1 pointer to the value to print
/// @param outfp pointer to output file stream
/// @pre values are pointers to char values.
///
static void dumpchar( const void * item1, FILE * outfp ) {
    if ( ! item1 ) {
        printf( "\n\tWARNING: null entry\n" );
        return ;
    }
    char v1 = *(char*)item1;
    fprintf( outfp, "%c\n", v1 );
    return ;
}


/// test_int_heap tests an integer heap.
/// @param maxFlag is true if heap is a max-heap.
///
void test_int_heap( bool maxFlag ) {

    printf( "test_int_heap using %s ordering\n"
          , (maxFlag ? "max" : "min") );

    int testData[] = { 1, 3, 8, 5, 7, 10, 9, 6, 4, 2, -1 };

    printf( "test data: [ " );
    for ( size_t i = 0; i < sizeof( testData ) / sizeof( int ); i++ ) {
        printf( "%d ", testData[i] );
    }
    printf( "]\n" );

    Heap theheap = NULL;
    if ( maxFlag ) {
        theheap = createHeap( 10, cmpintmax, dumpint );
    } else {
        theheap = createHeap( 10, cmpintmin, dumpint );
    }

    for ( size_t i = 0; i < sizeof( testData ) / sizeof( int ); i++ ) {
        insertHeapItem( theheap, (void*)&testData[i] );
    }

    printf( "Heap size is now %zu\n", sizeHeap( theheap ) );
    printf( "Dump:\n" );
    dumpHeap( theheap, stdout ); 
    printf( "Emptying the heap: " );
    while ( sizeHeap( theheap ) ) {
	int value = *(int*)removeTopHeap( theheap );
        printf( "%d ", value );
    }
    printf( "\n" );

    destroyHeap( theheap );

    return ;
}

/// test_char_heap tests a character heap.
/// @param maxFlag is true if heap is a max-heap.
///
void test_char_heap( bool maxFlag ) {

    printf( "test_char_heap using %s ordering\n"
          , (maxFlag ? "max" : "min") );

    char testData[] = { 'a', 'c', 'g', 'e', 'm', 'u', 'i', 'e', 'o', 'y', 'A' };

    printf( "test data: [ " );
    for ( size_t i = 0; i < sizeof( testData ) / sizeof( char ); i++ ) {
        printf( "%c ", testData[i] );
    }
    printf( "]\n" );

    Heap theheap = NULL;
    if ( maxFlag ) {
        theheap = createHeap( 10, cmpcharmax, dumpchar );
    } else {
        theheap = createHeap( 10, cmpcharmin, dumpchar );
    }

    for ( size_t i = 0; i < sizeof( testData ) / sizeof( char ); i++ ) {
        insertHeapItem( theheap, (void*)&testData[i] );
    }

    printf( "Heap size is now %zu\n", sizeHeap( theheap ) );
    printf( "Dump:\n" );
    dumpHeap( theheap, stdout );

    printf( "Emptying the heap: " );
    while ( sizeHeap( theheap ) ) {
	char value = *(char*)removeTopHeap( theheap );
        printf( "%c ", value );
    }
    printf( "\n" );

    destroyHeap( theheap );

    return ;
}


// // // // // // // // // // // // // // // // // // // // // // // // 
/// main function runs these tests:
/// Create a min heap, fill it with test data, and display it.
///
/// @returns errorCode  0 for no error
///
int main( void ) {


    test_int_heap( false );
    test_int_heap( true );

    test_char_heap( false );
    test_char_heap( true );

    return EXIT_SUCCESS ;
}

// RCS Revisions: $Log$ 
//
 
