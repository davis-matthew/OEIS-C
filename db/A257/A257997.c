/*
 * A257997  Numbers of the form (2^i)*(3^j) or (2^i)*(5^j) or (3^i)*(5^j).
 * https://oeis.org/A257997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 172, u32 n <= 684, u64 n <= 2731,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_overflows
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A257997: numbers of the form 2^i*3^j, 2^i*5^j or 3^i*5^j.
   Offset: 1 (a(1) = 1) */
/* The function a(idx_t n) returns the n-th term of the sequence. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mul_overflows.h"

/* Node for the priority queue */
struct Node {
    seq_t val;
    unsigned type;  /* 0: 2^i 3^j, 1: 2^i 5^j, 2: 3^i 5^j */
    unsigned i;
    unsigned j;
};

/* Pop the minimum element from a binary min-heap. */
static inline struct Node heap_pop(struct Node *heap, size_t *size)
{
    struct Node ret = heap[0];
    heap[0] = heap[--(*size)];
    size_t i = 0;
    while (1) {
        size_t left = 2 * i + 1;
        if (left >= *size)
            break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < *size && heap[right].val < heap[left].val)
            smallest = right;
        if (heap[smallest].val >= heap[i].val)
            break;
        struct Node tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
    return ret;
}

/* Push an element into a binary min-heap. The heap capacity is assumed
   sufficient for the expected number of pushes. */
static inline void heap_push(struct Node *heap, size_t *size,
                             const struct Node *v)
{
    size_t i = *size;
    heap[i] = *v;
    (*size)++;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val)
            break;
        struct Node tmp = heap[i];
        heap[i] = heap[parent];
        heap[parent] = tmp;
        i = parent;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but we return 0 for safety */

    /* Limits for exponent storage: 128 bits are more than enough for
       n <= 10000. */
    static unsigned char visited[3][128][128];
    memset(visited, 0, sizeof(visited));

    /* Prime pairs for the three types. */
    const unsigned primes[3][2] = { {2, 3}, {2, 5}, {3, 5} };

    /* Priority queue with a capacity that covers all pushes. */
    size_t heap_cap = 2 * n + 4; /* at most 2 pushes per pop */
    struct Node *heap = (struct Node *)malloc(heap_cap * sizeof(struct Node));
    size_t heap_size = 0;

    /* Seed the heap with the root nodes (1 for each type). */
    for (unsigned t = 0; t < 3; ++t) {
        visited[t][0][0] = 1;
        struct Node n0 = {1, t, 0, 0};
        heap_push(heap, &heap_size, &n0);
    }

    seq_t result = 0;
    seq_t last_val = 0;
    bool first = true;

    while (true) {
        struct Node cur = heap_pop(heap, &heap_size);

        /* Count distinct values. */
        if (first || cur.val != last_val) {
            --n;
            if (n == 0) {
                result = cur.val;
                break;
            }
            last_val = cur.val;
            first = false;
        }

        /* Generate successors for the current node. */
        for (int k = 0; k < 2; ++k) {
            unsigned p = primes[cur.type][k];
            unsigned ni = cur.i;
            unsigned nj = cur.j;
            if (k == 0)
                ++ni; /* multiply by the first prime */
            else
                ++nj; /* multiply by the second prime */

            if (ni >= 128 || nj >= 128)
                continue;  /* beyond our visited buffer */

            if (visited[cur.type][ni][nj])
                continue;  /* already queued */

            seq_t new_val;
            if (mul_overflows(cur.val, p, &new_val))
                continue;  /* result does not fit in seq_t */

            visited[cur.type][ni][nj] = 1;
            struct Node child = { new_val, cur.type, ni, nj };
            heap_push(heap, &heap_size, &child);
        }
    }

    free(heap);
    return result;
}
