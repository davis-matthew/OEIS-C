/*
 * A367576  Semiprimes p*q such that 2*p divides q-1.
 * https://oeis.org/A367576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth smallest semiprime p*q with p and q primes such
 *            that 2*p divides (q-1). The code enumerates primes p, then
 *            generates the arithmetic progression q = 1 (mod 2*p) using a
 *            min‑heap to keep the next candidate p*q for each p sorted. It
 *            caches results so repeated calls are O(1) after the...
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"

/* Node kept in the min‑heap */
typedef struct {
    seq_t   prod;   /* current product p*q         */
    uint64_t p;     /* prime p                      */
    uint64_t base;  /* 2*p                          */
    uint64_t q;     /* current prime q              */
} Node;

/* Static state kept between calls */
static uint64_t *prime_array   = NULL;   /* array of primes */
static size_t     prime_cnt    = 0;
static Node       *heap     = NULL;       /* min‑heap of Nodes */
static size_t      heap_size = 0, heap_cap = 0;
static seq_t      *cached   = NULL;       /* cache of terms   */
static size_t      cache_size = 0, cache_cap = 0;
static bool        initialized = false;

/* Swap two heap nodes */
static inline void swap_node(Node *a, Node *b)
{
    Node tmp = *a; *a = *b; *b = tmp;
}

/* Sift a node up the heap */
static void sift_up(size_t idx)
{
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].prod <= heap[idx].prod) break;
        swap_node(&heap[parent], &heap[idx]);
        idx = parent;
    }
}

/* Sift a node down the heap */
static void sift_down(size_t idx)
{
    while (true) {
        size_t left = (idx << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_size && heap[right].prod < heap[left].prod)
            smallest = right;
        if (heap[idx].prod <= heap[smallest].prod) break;
        swap_node(&heap[idx], &heap[smallest]);
        idx = smallest;
    }
}

/* Ensure that the primes, heap and cache have been created. */
static void ensure_initialized(idx_t n)
{
    if (initialized) return;

    /* Choose a safe upper limit for the prime sieve.
       n * 200 gives sufficient primes for the typical test ranges. */
    uint64_t limit = n * 200 + 1000;
    if (limit < 200000) limit = 200000;

    build_primes_upto(limit, &prime_array, &prime_cnt);

    heap_cap = prime_cnt;
    heap = (Node*)malloc(heap_cap * sizeof(Node));
    heap_size = 0;

    /* Initialise the heap with the first eligible q for each prime p. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_array[i];
        uint64_t base  = 2 * p;
        uint64_t q = base + 1;                /* 2p + 1  */
        while (!is_prime_big((seq_t)q)) q += base;
        heap[heap_size++] = (Node){ (seq_t)p * (seq_t)q, p, base, q };
        sift_up(heap_size - 1);
    }

    cache_cap = 8;
    cached = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    cache_size = 0;

    initialized = true;
}

/* Generate terms until we have at least n cached. */
static void generate_up_to(idx_t n)
{
    while (cache_size < (size_t)n) {
        /* Extract the smallest product. */
        Node top = heap[0];
        heap[0] = heap[heap_size - 1];
        heap_size--;
        sift_down(0);

        /* Store it in the cache. */
        if (cache_size == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 8;
            cached = (seq_t*)realloc(cached, cache_cap * sizeof(seq_t));
        }
        cached[cache_size++] = top.prod;

        /* Compute the next prime q for this prime p and insert it back. */
        uint64_t next_q = top.q + top.base;
        while (!is_prime_big((seq_t)next_q)) next_q += top.base;
        Node newnode = { (seq_t)top.p * (seq_t)next_q, top.p, top.base, next_q };
        heap[heap_size++] = newnode;
        sift_up(heap_size - 1);
    }
}

/*-------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* The sequence is 1‑based. */
    ensure_initialized(n);
    generate_up_to(n);
    return cached[n - 1];
}
