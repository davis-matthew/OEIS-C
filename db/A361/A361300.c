/*
 * A361300  Numbers of the form m^2 + p^2 for p prime and m > 0.
 * https://oeis.org/A361300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2605, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of the form m^2 + p^2 with p prime, m > 0,
   sorted in increasing order.  This function returns the n-th
   element (a(1) is the smallest such integer). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/*  Node for the min‑heap: value of m^2 + p^2, prime index, current m */
/* ------------------------------------------------------------------ */
struct Node {
    seq_t val;          /* m^2 + p^2 */
    size_t p_idx;       /* index into primes array */
    idx_t m;            /* current m for this prime */
};
typedef struct Node Node;

/* ------------------------------------------------------------------ */
/*  Static storage for the sequence and its generator              */
/* ------------------------------------------------------------------ */
static seq_t *cache      = NULL; /* stored sequence values                */
static size_t cache_cap  = 0;    /* allocated capacity of cache           */
static idx_t  cache_size = 0;    /* number of values currently stored     */

static Node *heap       = NULL; /* min‑heap of candidate values           */
static size_t heap_sz   = 0;    /* number of elements in heap            */
static size_t heap_cap  = 0;    /* allocated capacity of heap            */

static uint32_t *primes   = NULL;          /* list of primes                      */
static size_t    primes_cnt = 0;           /* number of primes                    */

#define PRIME_LIMIT 200000 /* sufficient for n≈10000 */

/* ------------------------------------------------------------------ */
/*  Helper: grow a re‑sizable array                                */
/* ------------------------------------------------------------------ */
static inline void grow_seq_cache(size_t needed)
{
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }
}

static inline void grow_heap(size_t needed)
{
    if (needed > heap_cap) {
        size_t new_cap = heap_cap ? heap_cap * 2 : 8;
        Node *tmp = (Node*)realloc(heap, new_cap * sizeof(Node));
        if (!tmp) exit(1);
        heap = tmp;
        heap_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/*  Heap operations                                                 */
/* ------------------------------------------------------------------ */
static inline void heap_swap(Node *a, Node *b)
{
    Node tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_percolate_up(size_t idx)
{
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].val <= heap[idx].val) break;
        heap_swap(&heap[parent], &heap[idx]);
        idx = parent;
    }
}

static void heap_percolate_down(size_t idx)
{
    size_t child;
    while ((child = (idx << 1) + 1) < heap_sz) {
        size_t right = child + 1;
        if (right < heap_sz && heap[right].val < heap[child].val)
            child = right;
        if (heap[idx].val <= heap[child].val) break;
        heap_swap(&heap[idx], &heap[child]);
        idx = child;
    }
}

static void heap_push(Node nd)
{
    grow_heap(heap_sz + 1);
    heap[heap_sz] = nd;
    heap_percolate_up(heap_sz++);
}

static Node heap_pop(void)
{
    Node top = heap[0];
    heap[0] = heap[--heap_sz];
    heap_percolate_down(0);
    return top;
}

/* ------------------------------------------------------------------ */
/*  Prime table initialization                                      */
/* ------------------------------------------------------------------ */
static void init_primes(void)
{
    if (primes) return;           /* already built */

    const size_t lim = PRIME_LIMIT;
    bool *is_comp = (bool*)calloc(lim + 1, sizeof(bool));
    if (!is_comp) exit(1);

    /* Rough estimate of number of primes up to lim */
    size_t est = lim / 2;
    primes = (uint32_t*)malloc(est * sizeof(uint32_t));
    if (!primes) exit(1);

    for (size_t i = 2; i <= lim; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            if (i * i <= lim)
                for (size_t j = i * i; j <= lim; j += i)
                    is_comp[j] = true;
        }
    }
    free(is_comp);
}

/* ------------------------------------------------------------------ */
/*  Sequence function: a(idx_t n)                                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not defined, but harmless */

    init_primes();                       /* ensure primes exist */

    /* If we already have the requested term, return it */
    if (cache_size >= n)
        return cache[n - 1];

    /* Initialise the heap once with the first value for each prime */
    if (heap_sz == 0) {
        for (size_t i = 0; i < primes_cnt; ++i) {
            seq_t p2 = (seq_t)primes[i] * (seq_t)primes[i];
            heap_push((Node){p2 + (seq_t)1, i, (idx_t)1});
        }
    }

    /* Generate terms until we reach the desired index */
    while (cache_size < n) {
        Node cur = heap_pop();

        /* Store only if different from the previous value */
        if (cache_size == 0 || cur.val != cache[cache_size - 1]) {
            grow_seq_cache(cache_size + 1);
            cache[cache_size++] = cur.val;
        }

        /* Push the next value for the same prime */
        idx_t next_m = cur.m + 1;
        seq_t m2 = (seq_t)next_m * (seq_t)next_m;
        seq_t p2 = (seq_t)primes[cur.p_idx] * (seq_t)primes[cur.p_idx];
        heap_push((Node){m2 + p2, cur.p_idx, next_m});
    }

    return cache[n - 1];
}
