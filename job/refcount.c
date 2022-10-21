#include <stdlib.h>
#include <stdio.h>
#include "refcount.h"

/*
 * Reference Counted Objects Look like this
 *     bytes 0..3:         reference count value
 *     bytes 4..7:         filler to maintain alignment of client payload
 *     bytes 8..nbytes+7:  client payload; client's reference points here
 */

/**
 * @brief allocated reference-counted object
 * 
 * @param nbytes size of client payload
 * @return void* pointer to payload portion of allocation
 */

void* rc_malloc (int nbytes) {
    int* ref_count = malloc(nbytes + 8);   // allocate nbytes plus room for count, retaining alignment
    *ref_count = 1;                        // initialize ref count to 1;
    return ((void*) ref_count) + 8;        // return pointer to client portion of allocation
}

/**
 * @brief increment reference count
 * 
 * @param p pointer to payload portion of allocation
 */

void  rc_keep_ref (void* p) {
    int *ref_count = p - 8;                // get pointer to ref count from client pointer
    (*ref_count)++;                        // increment ref count
}


/**
 * @brief decrement reference count; free if zero
 * 
 * @param p pointer to payload portion of allocation
 */
void  rc_free_ref (void* p) {
    int *ref_count = p - 8;                // get pointer to ref count from client pointer
    (*ref_count)--;                        // decrement ref count
    if (*ref_count == 0)                   // free allocation when ref count is 0
        free(ref_count);                   // note that ref_count points to the entire allocation
}
