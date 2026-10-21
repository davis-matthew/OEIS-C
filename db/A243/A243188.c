/*
 * A243188  Nonnegative numbers of the form x^2 + 6*x*y - 6*y^2.
 * https://oeis.org/A243188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 752, u32 n <= 9599, u64 n <= 9599,
 *            u128 n <= 9599, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th term of OEIS A243188.
   Nonnegative numbers of the form x^2 + 6*x*y - 6*y^2.
   To generate the sequence in ascending order we enumerate, for each
   nonnegative y, the values produced by increasing x, and merge all
   these sorted lists with a binary min‑heap.  Duplicates are
   discarded on the fly.  The implementation works for any seq_t
   type (unsigned integer or arbitrary‑precision integer class) and
   handles both C11 and C++17 builds.  */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Internal node type used in the heap.  The struct is POD in C and
   trivially copyable in C++ (seq_t is a basic integer or a
   multiprecision type with trivial copy semantics under the build). */
typedef struct {
    seq_t val;   /* current value x^2 + 6*x*y - 6*y^2 */
    idx_t x;     /* current x for this y */
    idx_t y;     /* fixed y for this sequence */
} heap_node;

/* Swap two heap nodes. */
static inline void heap_swap(heap_node *a, heap_node *b)
{
    heap_node tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Push a node into the binary min‑heap. */
static void heap_push(heap_node *heap, size_t *n, const heap_node *node)
{
    size_t i = *n;
    heap[i] = *node;
    ++(*n);
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val)
            break;
        heap_swap(&heap[parent], &heap[i]);
        i = parent;
    }
}

/* Pop the smallest node from the heap. */
static heap_node heap_pop(heap_node *heap, size_t *n)
{
    heap_node top = heap[0];
    --(*n);
    heap[0] = heap[*n];
    size_t i = 0;
    while (1) {
        size_t left = (i << 1) + 1;
        if (left >= *n) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < *n && heap[right].val < heap[left].val)
            smallest = right;
        if (heap[i].val <= heap[smallest].val) break;
        heap_swap(&heap[smallest], &heap[i]);
        i = smallest;
    }
    return top;
}

/* Return the n-th term of A243188 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1, but guard anyway */
        return 0;

    /* Choose the heap size as n+1; this guarantees enough sequences
       to cover the n-th smallest number. */
    size_t heap_cap = (size_t)n + 1;

    /* Allocate the heap array in a way compatible with both C and C++. */
#ifdef __cplusplus
    heap_node *heap = new heap_node[heap_cap];
#else
    heap_node *heap = (heap_node *)malloc(heap_cap * sizeof(heap_node));
#endif
    if (!heap) /* allocation failure */
        return 0;

    /* Initialise the heap with the smallest value from each y = 0 .. n. */
    size_t heap_size = 0;
    for (idx_t y = 0; y <= n; ++y) {
        heap_node node;
        node.y = y;
        node.x = y;                     /* minimal x for nonnegative value */
        node.val = (seq_t)y * (seq_t)y; /* y^2 when x = y */
        heap_push(heap, &heap_size, &node);
    }

    seq_t last_val = 0;
    bool first = true;
    idx_t count = 0;
    seq_t result = 0;

    while (count < n) {
        heap_node cur = heap_pop(heap, &heap_size);

        /* Count distinct values only. */
        if (first || cur.val != last_val) {
            ++count;
            last_val = cur.val;
            first = false;
            if (count == n) {
                result = cur.val;
                break;
            }
        }

        /* Generate the next value for this y (increment x). */
        idx_t next_x = cur.x + 1;
        seq_t xx = (seq_t)next_x;
        seq_t yy = (seq_t)cur.y;
        seq_t v = xx * xx + 6 * xx * yy - 6 * yy * yy;

        heap_node nxt;
        nxt.y = cur.y;
        nxt.x = next_x;
        nxt.val = v;
        heap_push(heap, &heap_size, &nxt);
    }

    /* Release the heap. */
#ifdef __cplusplus
    delete[] heap;
#else
    free(heap);
#endif

    return result;
}
