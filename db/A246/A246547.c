/*
 * A246547  Prime powers p^e where p is a prime and e >= 2 (prime powers
 *          without the primes or 1).
 * https://oeis.org/A246547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 57, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A246547 – prime powers p^e, p prime, e ≥ 2 (offset 1).
   This implementation generates terms by merging the infinite lists
   {p², p³, p⁴, …} for every prime p, using a min‑heap.  The smallest
   unproduced power is extracted each time the cache needs a new term. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

/* Node in the min‑heap: current power, its prime, and exponent. */
typedef struct {
    seq_t   val;          /* current prime power p^e   */
    uint64_t prime;       /* the underlying prime p   */
    unsigned char exp;    /* current exponent e (≥ 2) */
} node_t;

/* Heap for extracting the next smallest prime power. */
static node_t *heap      = NULL; /* dynamic array of nodes          */
static size_t heap_size  = 0;     /* number of valid nodes in heap   */
static size_t heap_cap   = 0;     /* allocated capacity of heap      */

/* Cache of already produced prime powers. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;     /* number of cached terms          */
static size_t cache_cap  = 0;     /* allocated capacity of cache     */

/* List of primes up to a fixed bound (used to seed the heap). */
static uint64_t *prime_list = NULL;
static size_t   prime_cnt   = 0;

/* Has one‑time initialisation been performed? */
static bool initialized = false;

/* Bound on the number of primes we pre‑compute.  This covers n ≤ 10 000. */
static const idx_t PRIME_LIMIT = 1000000;

/* ------------------------------------------------------------------ */
/* Helper functions for the binary heap. */
/* ------------------------------------------------------------------ */
static void heap_swap(node_t *a, node_t *b)
{
    node_t tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_push(node_t node)
{
    /* Grow heap capacity if necessary. */
    if (heap_size >= heap_cap) {
        size_t new_cap = (heap_cap == 0) ? 16 : heap_cap << 1;
        node_t *new_heap = (node_t *)realloc(heap, new_cap * sizeof(node_t));
        if (!new_heap) abort();          /* allocation failure */
        heap = new_heap;
        heap_cap = new_cap;
    }

    /* Insert new node at the end. */
    size_t i = heap_size++;
    heap[i] = node;

    /* Sift up to maintain the min‑heap property. */
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val) break;
        heap_swap(&heap[parent], &heap[i]);
        i = parent;
    }
}

static void heap_pop_min(node_t *out)
{
    if (heap_size == 0) abort();
    *out = heap[0];
    if (--heap_size > 0) {
        heap[0] = heap[heap_size];
        /* Sift down to restore heap property. */
        size_t i = 0;
        while (1) {
            size_t left  = (i << 1) + 1;
            size_t right = left + 1;
            size_t smallest = i;

            if (left  < heap_size && heap[left].val  < heap[smallest].val)
                smallest = left;
            if (right < heap_size && heap[right].val < heap[smallest].val)
                smallest = right;
            if (smallest == i) break;
            heap_swap(&heap[i], &heap[smallest]);
            i = smallest;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Helpers for the cache. */
/* ------------------------------------------------------------------ */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* One‑time initialization – build the prime list and seed the heap. */
/* ------------------------------------------------------------------ */
static void init_once(void)
{
    /* Build list of primes up to PRIME_LIMIT. */
    build_primes_upto(PRIME_LIMIT, &prime_list, &prime_cnt);

    heap_cap = prime_cnt;
    heap = (node_t *)malloc(heap_cap * sizeof(node_t));
    if (!heap) abort();

    /* Insert the first power p² for each prime. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        node_t n = { .val = (seq_t)prime_list[i] * (seq_t)prime_list[i],
                     .prime = prime_list[i],
                     .exp   = 2 };
        heap_push(n);
    }
    heap_size = prime_cnt;            /* all nodes are now in the heap */

    cache      = NULL;
    cache_len  = 0;
    cache_cap  = 0;
}

/* ------------------------------------------------------------------ */
/* Main OEIS sequence function. */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined below offset – harmless */

    if (!initialized) {
        init_once();
        initialized = true;
    }

    /* Produce terms until the cache contains at least n entries. */
    ensure_cache_capacity(n);
    while (cache_len < (size_t)n) {
        node_t cur;
        heap_pop_min(&cur);
        cache[cache_len++] = cur.val;

        /* Push the next power of the same prime, if it fits. */
        seq_t next = cur.val * (seq_t)cur.prime;
        if (next > cur.val) {          /* avoid overflow (wrap) */
            node_t next_node = { .val = next,
                                 .prime = cur.prime,
                                 .exp   = (unsigned char)(cur.exp + 1) };
            heap_push(next_node);
        }
    }

    return cache[n - 1];
}
