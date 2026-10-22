/*
 * A373214  Signature sequence of log(3).
 * https://oeis.org/A373214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 127, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the i‑value of the nth smallest element of the list
 *           { i + j*log(3) : i,j >= 1 } when the list is sorted in increasing order.
 * The program uses a min‑heap over j to extract the values in order.
 */

#include <stdint.h>
#include <stdlib.h>

/* Scaling constants for a rational approximation of log(3). */
static const uint64_t SCALE = 1000000000000000000ULL; /* 1e18 */
static const uint64_t NUM   = 1098612288668109800ULL; /* floor(log(3)*SCALE) */

/* Heap node containing an index pair (i,j) and the key i*SCALE + j*NUM. */
typedef struct {
    uint64_t j;
    uint64_t i;
    unsigned __int128 key;   /* sufficient to order values */
} heap_node;

/* Sift down a node at position idx within a heap of size sz. */
static void heapify_down(heap_node *heap, size_t idx, size_t sz)
{
    while (1) {
        size_t left  = idx << 1;
        size_t right = left + 1;
        size_t smallest = idx;

        if (left <= sz && heap[left].key < heap[smallest].key)
            smallest = left;
        if (right <= sz && heap[right].key < heap[smallest].key)
            smallest = right;

        if (smallest == idx)
            break;

        heap_node tmp = heap[idx];
        heap[idx] = heap[smallest];
        heap[smallest] = tmp;
        idx = smallest;
    }
}

/* Sift up a node at position idx within a heap of size sz. */
static void heapify_up(heap_node *heap, size_t idx, size_t sz)
{
    while (idx > 1) {
        size_t parent = idx >> 1;
        if (heap[parent].key <= heap[idx].key)
            break;
        heap_node tmp = heap[parent];
        heap[parent] = heap[idx];
        heap[idx] = tmp;
        idx = parent;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n==0; return 0 */

    /* Use a heap of size n: enough to generate the n smallest sums. */
    size_t j_max = (size_t)n;
    heap_node *heap = (heap_node *)malloc((j_max + 1) * sizeof(heap_node));
    if (!heap)
        return (seq_t)0;           /* allocation failure */

    /* Initialise heap roots for j = 1..j_max with i = 1. */
    for (size_t j = 1; j <= j_max; ++j) {
        heap[j].j   = j;
        heap[j].i   = 1;
        heap[j].key = ((unsigned __int128)heap[j].i) * (unsigned __int128)SCALE
                    + ((unsigned __int128)heap[j].j) * (unsigned __int128)NUM;
    }

    /* Build a min‑heap. */
    for (size_t i = j_max >> 1; i > 0; --i)
        heapify_down(heap, i, j_max);

    size_t heap_size = j_max;
    seq_t result = 0;
    size_t n_sz = (size_t)n;

    for (size_t t = 1; t <= n_sz; ++t) {
        /* Extract the smallest (i,j). */
        heap_node minNode = heap[1];
        result = (seq_t)minNode.i;              /* value to return */

        /* Replace root with last element. */
        heap[1] = heap[heap_size];
        --heap_size;
        heapify_down(heap, 1, heap_size);

        /* Advance i for this j, compute new key, and re‑insert. */
        ++minNode.i;
        minNode.key = ((unsigned __int128)minNode.i) * (unsigned __int128)SCALE
                    + ((unsigned __int128)minNode.j) * (unsigned __int128)NUM;
        ++heap_size;
        heap[heap_size] = minNode;
        heapify_up(heap, heap_size, heap_size);
    }

    free(heap);
    return result;
}
