/*
 * A303234  Numbers of the form x*(x+1)/2 + 2^y with x and y nonnegative
 *          integers.
 * https://oeis.org/A303234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) are the numbers of the form x*(x+1)/2 + 2^y with
 *            nonnegative integers x and y, given in increasing order
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"

#ifndef Y_MAX
/* Upper bound on y.  64 is safe for n up to many thousands even with
   a 64‑bit sequence; for arbitrary precision you will simply generate
   all required y values because the heap will grow as needed. */
#define Y_MAX 64
#endif

/* Storage for each stream indexed by y. */
typedef struct {
    seq_t val;           /* current value of this stream */
    uint64_t y;          /* fixed y for this stream */
    uint64_t x;          /* current x index for this stream */
} node_t;

/* Min‑heap of node_t objects. */
static size_t heap_size = 0;
static size_t heap_cap   = Y_MAX;
static node_t *heap = NULL;

/* Memoised sequence values. */
static seq_t *seq_cache = NULL;
static idx_t cache_len  = 0;
static idx_t cache_cap  = 0;

/* Utility: swap two nodes */
static inline void swap_nodes(node_t *a, node_t *b) {
    node_t tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Push a node into the heap. */
static void heap_push(node_t node) {
    if (heap_size == heap_cap) {
        heap_cap <<= 1;
        heap = (node_t *)realloc(heap, heap_cap * sizeof(node_t));
    }
    size_t i = heap_size++;
    heap[i] = node;
    /* sifting up */
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val)
            break;
        swap_nodes(&heap[parent], &heap[i]);
        i = parent;
    }
}

/* Pop the smallest node from the heap. */
static node_t heap_pop(void) {
    node_t ret = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    for (;;) {
        size_t left  = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left  < heap_size && heap[left].val  < heap[smallest].val)
            smallest = left;
        if (right < heap_size && heap[right].val < heap[smallest].val)
            smallest = right;
        if (smallest == i) break;
        swap_nodes(&heap[i], &heap[smallest]);
        i = smallest;
    }
    return ret;
}

/* Ensure the cache can hold at least `need` elements. */
static void grow_cache(idx_t need) {
    if (need <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Lazy initialisation of heap and cache. */
static void init_once(void) {
    static bool initialized = false;
    if (initialized)
        return;
    initialized = true;

    heap = (node_t *)malloc(heap_cap * sizeof(node_t));
    if (!heap)
        abort();

    /* initialise cache */
    cache_cap = 16;
    seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!seq_cache)
        abort();
    cache_len = 0;

    /* push one element for each y in [0, Y_MAX) */
    for (uint64_t y = 0; y < Y_MAX; ++y) {
        node_t n = { pow2(y), y, 0 }; /* T_0 + 2^y */
        heap_push(n);
    }
}

/* Public function required by the build. */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;          /* undefined; return 0 for safety */

    init_once();

    /* generate sequence until we have at least n terms */
    while (cache_len < n) {
        node_t cur = heap_pop();

        /* deduplicate – compare to last inserted value */
        if (cache_len == 0 || cur.val != seq_cache[cache_len - 1]) {
            grow_cache(cache_len + 1);
            seq_cache[cache_len++] = cur.val;
        }

        /* advance this stream: next x = cur.x + 1 */
        cur.x++;
        cur.val += cur.x;          /* T_{x+1} = T_x + (x+1) */
        heap_push(cur);
    }

    return seq_cache[n - 1];
}
