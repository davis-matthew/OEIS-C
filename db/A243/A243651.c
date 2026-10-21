/*
 * A243651  Nonnegative integers of the form x^2+11y^2.
 * https://oeis.org/A243651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Nonnegative integers representable as x^2 + 11*y^2
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation generates the sequence by merging, for each y ≥ 0, the
   infinite list {x^2 + 11*y^2 : x = 0,1,2,…} using a min‑heap.  Duplicate
   values from different y are discarded.  The routine keeps a static cache
   of previously computed terms so that calls may arrive in any order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"

typedef struct {
    seq_t val;   /* current value x^2 + 11*y^2 */
    idx_t y;     /* fixed y for this list */
    idx_t x;     /* current x (starts at 0) */
} HeapNode;

/* --------- heap utilities --------- */
static void sift_down(HeapNode *h, size_t size, size_t idx) {
    while (1) {
        size_t left = 2 * idx + 1;
        if (left >= size) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < size && h[right].val < h[left].val) smallest = right;
        if (h[idx].val <= h[smallest].val) break;
        HeapNode tmp = h[idx];
        h[idx] = h[smallest];
        h[smallest] = tmp;
        idx = smallest;
    }
}

/* --------- static state --------- */
static HeapNode *heap = NULL;
static size_t heap_size = 0;
static seq_t *cache = NULL;      /* cached values of the sequence */
static size_t cache_len = 0;     /* number of values stored in cache */
static size_t cache_cap = 0;     /* current capacity of cache */
static bool initialized = false;
#define Y_MAX 5000   /* covers up to ≈ 10000 terms for this quadratic form */

/* --------- sequence initialization --------- */
static void init_sequence(void) {
    /* construct heap of Y_MAX+1 lists */
    heap_size = Y_MAX + 1;
    heap = (HeapNode *)malloc(heap_size * sizeof(HeapNode));
    for (idx_t i = 0; i < (idx_t)heap_size; ++i) {
        heap[i].y = i;
        heap[i].x = 0;
        seq_t base = (seq_t)11 * (seq_t)i * (seq_t)i;   /* 11*y^2 */
        heap[i].val = base;
    }
    /* heapify */
    for (size_t i = heap_size / 2; i > 0; --i)
        sift_down(heap, heap_size, i - 1);

    /* initialise cache */
    cache_cap = 128;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache_len = 0;
    initialized = true;
}

/* --------- main sequence accessor --------- */
seq_t a(idx_t n) {
    if (!initialized) init_sequence();

    /* produce terms until we reach the needed index */
    while (cache_len <= n) {
        HeapNode *cur = &heap[0];
        seq_t val = cur->val;

        /* store unique value */
        if (cache_len == 0 || val != cache[cache_len - 1]) {
            if (cache_len == cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = val;
        }

        /* advance the node for this y: (x, val) → (x+1, val + 2*x + 1) */
        idx_t oldx = cur->x;
        cur->x = oldx + 1;
        seq_t inc = (seq_t)((idx_t)2 * oldx + 1);
        cur->val = val + inc;

        /* restore min‑heap property */
        sift_down(heap, heap_size, 0);
    }

    return cache[n];
}
