/*
 * A365801  Numbers k such that A163511(k) is a cube.
 * https://oeis.org/A365801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sequence A365801 – start with 0 and 4, then for every non‑zero *
 *            term t add 2*t + 1 and 8*t to the set. The function below builds
 *            * the sequence on demand, keeping all produced numbers sorted. *
 *            The implementation uses a min‑heap and memoises the sequence
 * Range:     u8 n <= 5, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------ */
/* Grow an array of type seq_t.  The cast ensures the code
   compiles both in C (where the cast is optional) and in C++.
   ------------------------------------------------------------ */
static void grow_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (needed <= *cap)
        return;
    size_t new_cap = (*cap == 0) ? 4 : *cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    void *tmp = realloc(*arr, new_cap * sizeof(seq_t));
    *arr = (seq_t *)tmp;    /* cast needed for C++ */
    *cap = new_cap;
}

/* ------------------------------------------------------------ */
/* Main function as required by the specification                */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static state keeps the sequence and heap between calls */
    static seq_t *seq     = NULL;   /* generated sequence              */
    static size_t  seq_len = 0;     /* number of terms computed         */
    static size_t  seq_cap = 0;     /* allocated capacity               */

    static seq_t *heap     = NULL;  /* min‑heap of pending values      */
    static size_t  heap_size = 0;   /* number of elements in heap      */
    static size_t  heap_cap  = 0;   /* allocated capacity of heap      */

    /* Initialise first term (0) and the heap with seed 4 */
    if (seq_len == 0) {
        grow_array(&seq, &seq_cap, 1);
        seq[0] = (seq_t)0;
        seq_len = 1;
    }
    if (heap_size == 0 && heap_cap == 0) {
        grow_array(&heap, &heap_cap, 1);
        heap[0] = (seq_t)4;   /* seed value */
        heap_size = 1;
    }

    /* Generate terms until we have at least n elements */
    while (seq_len < n) {
        /* Pop smallest element from the heap */
        seq_t x = heap[0];
        heap_size--;
        if (heap_size > 0) {
            heap[0] = heap[heap_size];
            /* Sift down */
            size_t i = 0;
            while (1) {
                size_t left  = i * 2 + 1;
                size_t right = left + 1;
                size_t smallest = i;
                if (left < heap_size && heap[left] < heap[smallest])
                    smallest = left;
                if (right < heap_size && heap[right] < heap[smallest])
                    smallest = right;
                if (smallest == i)
                    break;
                seq_t tmp = heap[i];
                heap[i] = heap[smallest];
                heap[smallest] = tmp;
                i = smallest;
            }
        }

        /* Append the popped value to the sequence */
        grow_array(&seq, &seq_cap, seq_len + 1);
        seq[seq_len++] = x;

        /* Generate the two children of x */
        grow_array(&heap, &heap_cap, heap_size + 2);

        /* child 1 = 2*x + 1 */
        seq_t child1 = (seq_t)(((seq_t)2 * x) + (seq_t)1);
        heap[heap_size] = child1;
        size_t i = heap_size++;
        /* Sift up */
        while (i > 0) {
            size_t parent = (i - 1) / 2;
            if (heap[parent] <= heap[i])
                break;
            seq_t tmp = heap[parent];
            heap[parent] = heap[i];
            heap[i] = tmp;
            i = parent;
        }

        /* child 2 = 8*x */
        seq_t child2 = (seq_t)(((seq_t)8 * x));
        heap[heap_size] = child2;
        i = heap_size++;
        while (i > 0) {
            size_t parent = (i - 1) / 2;
            if (heap[parent] <= heap[i])
                break;
            seq_t tmp = heap[parent];
            heap[parent] = heap[i];
            heap[i] = tmp;
            i = parent;
        }
    }

    /* Return the requested term (1‑based index) */
    return seq[n - 1];
}
