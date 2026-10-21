/*
 * A248963  Prime powers p^m for which sigma(p^2m) is not prime.
 * https://oeis.org/A248963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 896, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, big_pow, build_primes_upto, cmp_seq_t,
 *            isPrimeGeneric, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: prime powers p^m for which σ(p^(2m)) is not prime,
   with a(1)=1.  The algorithm generates prime powers in increasing
   order by a min‑heap; for each candidate p^m it checks
   σ(p^(2m))=(p^(2m+1)−1)/(p−1) for primality.  */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/append_candidate.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/cmp_seq_t.h"
#include "../utils/build_primes_upto.h"

/* Node representing a prime power p^e */
typedef struct {
    seq_t value;      /* current value of p^e */
    uint64_t prime;   /* prime p */
    uint64_t exp;     /* exponent e */
} Node;

/* ---------- Min‑heap of Nodes ---------- */
static Node *heap = NULL;
static size_t heap_size = 0;
static size_t heap_cap   = 0;

/* Compare two nodes by their value */
static inline int node_cmp(const Node *a, const Node *b)
{
    return cmp_seq_t((const void *)&a->value, (const void *)&b->value);
}

/* Push a node onto the heap */
static void heap_push(Node v)
{
    if (heap_size == heap_cap) {
        heap_cap = heap_cap ? heap_cap * 2 : 8;
        heap = (Node *)realloc(heap, heap_cap * sizeof(Node));
    }
    size_t i = heap_size++;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (node_cmp(&heap[parent], &v) <= 0) break;
        heap[i] = heap[parent];
        i = parent;
    }
    heap[i] = v;
}

/* Pop the smallest node from the heap */
static Node heap_pop(void)
{
    Node root = heap[0];
    Node last = heap[--heap_size];
    size_t i = 0;
    while (true) {
        size_t left = (i << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_size && node_cmp(&heap[right], &heap[left]) < 0)
            smallest = right;
        if (node_cmp(&last, &heap[smallest]) <= 0) break;
        heap[i] = heap[smallest];
        i = smallest;
    }
    heap[i] = last;
    return root;
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    static bool              initialized = false;
    static seq_t            *cache       = NULL;
    static size_t            cache_len   = 0;
    static size_t            cache_cap   = 0;
    static uint64_t         *primes      = NULL;
    static size_t            prime_cnt   = 0;

    if (!initialized) {
        /* Sufficiently large bound to generate the required prime powers. */
        const idx_t PRIME_LIMIT = 2000000; /* ~148933 primes */
        build_primes_upto(PRIME_LIMIT, &primes, &prime_cnt);

        /* Allocate and fill the heap with the first powers (p^1). */
        heap_cap  = prime_cnt;
        heap      = (Node *)malloc(heap_cap * sizeof(Node));
        heap_size = 0;
        for (size_t i = 0; i < prime_cnt; ++i)
            heap_push((Node){(seq_t)primes[i], primes[i], 1});

        /* Cache starts with the special term 1. */
        append_candidate(&cache, &cache_len, &cache_cap, (seq_t)1);

        initialized = true;
    }

    /* Generate terms until we've produced at least n of them. */
    while (cache_len < (size_t)n) {
        Node cur = heap_pop();

        uint64_t p = cur.prime;
        uint64_t e = cur.exp;

        /* σ(p^(2e)) = (p^(2e+1)−1)/(p−1) */
        seq_t exp   = (seq_t)(2ULL * e + 1);
        seq_t pow   = big_pow((seq_t)p, exp);
        seq_t numer = pow - (seq_t)1;
        seq_t denom = (seq_t)(p - 1);
        seq_t sigma = numer / denom;

        if (!isPrimeGeneric(sigma))
            append_candidate(&cache, &cache_len, &cache_cap, cur.value);

        /* Insert the next power of the same prime. */
        seq_t next_val = seq_mul(cur.value, (seq_t)p);
        heap_push((Node){next_val, p, e + 1});
    }

    return cache[(size_t)n - 1];
}
