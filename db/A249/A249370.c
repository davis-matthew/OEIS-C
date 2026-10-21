/*
 * A249370  Numbers p*m^2, where p is an odd prime and m >= 1, arranged in
 *          increasing order.
 * https://oeis.org/A249370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A249370: numbers of the form p*m^2 (p odd prime, m≥1) 
   in increasing order, using a min‑heap of the prime‑squared sequences. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Node in the heap: current value for a prime sequence */
struct Node {
    idx_t prime_idx;   /* index into primes[] */
    idx_t k;           /* current multiplier (k ≥ 1) */
    seq_t val;         /* current value prime * k^2 */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, n=0 is unused */

    /* static state */
    static bool initialized = false;
    static uint64_t *primes = NULL;
    static size_t  primes_cnt = 0;
    static struct Node *heap  = NULL;
    static size_t       heap_size = 0;
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    if (!initialized) {
        /* --- prime generation --------------------------- */
        const idx_t LIMIT = 200000;   /* enough for n ≈ 10000 */
        bool *comp = (bool*)calloc((size_t)(LIMIT + 1), sizeof(bool));
        if (!comp) abort();

        for (idx_t i = 2; i * i <= LIMIT; ++i)
            if (!comp[i])
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    comp[j] = true;

        uint64_t *tmp = (uint64_t*)malloc((LIMIT / 2) * sizeof(uint64_t));
        if (!tmp) abort();
        primes_cnt = 0;
        for (idx_t i = 3; i <= LIMIT; i += 2)
            if (!comp[i])  /* odd primes only */
                tmp[primes_cnt++] = (uint64_t)i;
        free(comp);

        primes = (uint64_t*)malloc(primes_cnt * sizeof(uint64_t));
        if (!primes) abort();
        memcpy(primes, tmp, primes_cnt * sizeof(uint64_t));
        free(tmp);

        /* --- heap initialization ----------------------- */
        heap = (struct Node*)malloc(primes_cnt * sizeof(struct Node));
        if (!heap) abort();
        for (size_t i = 0; i < primes_cnt; ++i) {
            heap[i].prime_idx = i;
            heap[i].k = 1;
            heap[i].val = (seq_t)primes[i] * (seq_t)1;
        }
        heap_size = primes_cnt;

        /* heapify (min‑heap) */
        for (size_t i = heap_size / 2; i-- > 0;) {
            size_t hole = i;
            while (true) {
                size_t left  = 2 * hole + 1;
                size_t right = 2 * hole + 2;
                size_t best = hole;
                if (left < heap_size && heap[left].val < heap[best].val)
                    best = left;
                if (right < heap_size && heap[right].val < heap[best].val)
                    best = right;
                if (best == hole)
                    break;
                struct Node tmpN = heap[hole];
                heap[hole] = heap[best];
                heap[best] = tmpN;
                hole = best;
            }
        }

        /* cache for previously computed terms */
        cache_cap = 0;
        cache_len = 0;
        cache = NULL;

        initialized = true;
    }

    /* Return cached value if already computed */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Generate terms until we reach n */
    while (cache_len < (size_t)n) {
        /* Extract minimum from heap */
        struct Node top = heap[0];
        heap[0] = heap[heap_size - 1];
        --heap_size;

        /* restore heap property */
        if (heap_size > 0) {
            size_t hole = 0;
            while (true) {
                size_t left  = 2 * hole + 1;
                size_t right = 2 * hole + 2;
                size_t best = hole;
                if (left < heap_size && heap[left].val < heap[best].val)
                    best = left;
                if (right < heap_size && heap[right].val < heap[best].val)
                    best = right;
                if (best == hole)
                    break;
                struct Node tmpN = heap[hole];
                heap[hole] = heap[best];
                heap[best] = tmpN;
                hole = best;
            }
        }

        /* Store extracted value */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) abort();
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[cache_len++] = top.val;

        /* Advance the sequence for this prime */
        top.k++;
        top.val = (seq_t)primes[top.prime_idx] *
                  ((seq_t)top.k * (seq_t)top.k);

        /* Push the updated node back into the heap */
        heap[heap_size++] = top;
        size_t hole = heap_size - 1;
        while (hole > 0) {
            size_t parent = (hole - 1) / 2;
            if (heap[hole].val < heap[parent].val) {
                struct Node tmpN = heap[hole];
                heap[hole] = heap[parent];
                heap[parent] = tmpN;
                hole = parent;
            } else
                break;
        }
    }

    return cache[(size_t)n - 1];
}
