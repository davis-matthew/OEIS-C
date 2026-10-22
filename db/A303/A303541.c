/*
 * A303541  Numbers of the form k^2 + binomial(2*m,m) with k and m nonnegative
 *          integers.
 * https://oeis.org/A303541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers of the form k^2 + binomial(2*m,m).
   The function a(n) generates the sequence incrementally using a
   simple min-heap over the different values of m.  Duplicate values
   are discarded so that the resulting list is the sorted set of all
   numbers representable in that form.  The cache is extended only
   as far as needed, so subsequent calls with any n in any order
   behave correctly.  The implementation is fully portable to C11 and
   C++17.  */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) */

typedef struct {
    seq_t value;    /* current k^2 + binom(2*m,m) */
    idx_t m;        /* fixed m for this branch */
    idx_t k;        /* current k value (starting at 0) */
} node_t;

#define MAX_M 35   /* ensures binomial fits in 64‑bit when seq_t is 64‑bit */

static bool initialized = false;
static seq_t *cache      = NULL;    /* dynamic array of sequence values */
static size_t cache_len  = 0;        /* number of elements stored */
static size_t cache_cap  = 0;        /* allocated capacity */

static node_t heap[MAX_M + 1];
static idx_t  heap_size = 0;         /* number of nodes in the heap */

static seq_t  binom[MAX_M + 1];      /* binom(2*m,m) for m<=MAX_M */
static seq_t  last_val = 0;          /* last value inserted into cache */

/*--- auxiliary heap operations ------------------------------------------*/
static void heap_init(void) {
    idx_t i;
    for (i = 0; i <= MAX_M; ++i) {
        binom[i] = binomial((idx_t)(2 * i), (idx_t)i);
        heap[i].m = i;
        heap[i].k = 0;
        heap[i].value = binom[i];    /* k = 0 -> value = binom */
    }
    heap_size = (idx_t)(MAX_M + 1);
} 

static idx_t heap_find_min(void) {
    idx_t i, min = 0;
    for (i = 1; i < heap_size; ++i) {
        if (heap[i].value < heap[min].value)
            min = i;
    }
    return min;
}

static void heap_replace_idx(idx_t idx, node_t new_node) {
    heap[idx] = new_node;
    heap_size             = heap_size;  /* unchanged */
}

static void heap_push(node_t node) {
    heap[heap_size++] = node;
}

static node_t heap_pop_min(void) {
    idx_t i = heap_find_min();
    node_t node = heap[i];
    heap[i] = heap[--heap_size];
    return node;
}

/*--- cache handling ------------------------------------------------------*/
static void cache_ensure_capacity(size_t req) {
    if (req <= cache_cap) return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (new_cap < req) new_cap <<= 1;
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf) {
        /* allocation failed – for safety abort; in contest environment
           we can treat this as unrecoverable */
        abort();
    }
    cache      = new_buf;
    cache_cap  = new_cap;
}

static void extend_cache_up_to(idx_t n) {
    if (!initialized) {
        /* one‑time initialisation */
        cache_ensure_capacity(1000);
        heap_init();
        initialized = true;
    }

    while (cache_len < n) {
        node_t cur = heap_pop_min();

        seq_t val = cur.value;
        if (val != last_val) {          /* new distinct value */
            cache_ensure_capacity(cache_len + 1);
            cache[cache_len++] = val;
            last_val = val;
        }

        /* advance this m‑branch to next k */
        cur.k += 1;
        /* new value = binom + k^2 */
        seq_t k_sq = (seq_t)(cur.k) * (seq_t)(cur.k);
        cur.value = binom[cur.m] + k_sq;
        heap_push(cur);
    }
}

/*--- public function -----------------------------------------------------*/
seq_t a(idx_t n)
/* Return the n-th term (1‑based) of A303541 */
{
    if (n == 0) return 0;                 /* undefined, but harmless */
    extend_cache_up_to(n);
    return cache[n - 1];
}
