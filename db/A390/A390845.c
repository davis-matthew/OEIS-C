/*
 * A390845  Expansion of e.g.f. exp( x^3/6 + x^4/24 ).
 * https://oeis.org/A390845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A390845 – expansion of exp( x^3/6 + x^4/24 )
   Recurrence:
     a(0)=1, a(1)=0, a(2)=0, a(3)=1
     a(n) = (n‑1)(n‑2)/2 * a(n‑3) + (n‑1)(n‑2)(n‑3)/6 * a(n‑4)   (n≥4) */
#include <stddef.h>
#include <stdlib.h>

/* external integer types:
   idx_t – unsigned integer for indices (uint64_t)
   seq_t – unsigned integer for values, may be 8,16,32,64 or 128 bits,
           or arbitrary‑precision (compiled as C++17). */
extern idx_t _unused;  /* dummy to suppress unused warnings */

/* global cached array of previously computed terms */
static seq_t *cache = NULL;      /* pointer to dynamic array */
static idx_t cache_len = 0;      /* number of entries currently stored */
static size_t cache_cap = 0;     /* allocated capacity (in seq_t elements) */

static void ensure_cache_cap(idx_t n)
{
    /* Ensure capacity for indices up to n (inclusive). */
    if (cache_cap <= (size_t)n)
    {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *newptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newptr) abort();  /* out of memory */
        cache = newptr;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* ---------- 1. Build/extend the cache -------------------------------- */
    if (cache_len == 0)
    {
        /* First allocation large enough for the required index. */
        size_t sz = (n > 3) ? (size_t)n + 1 : 4;
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = sz;

        /* Base values. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache_len = 4;
        if (cache_len > n+1) cache_len = n+1;   /* keep track of filled part */
    }

    /* If the requested index is already covered, return it. */
    if (n < cache_len) return cache[n];

    /* Expand capacity to cover new indices. */
    ensure_cache_cap(n);

    /* ---------- 2. Compute missing terms --------------------------------- */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        /* Compute the two parts of the recurrence. */
        seq_t term = (seq_t)0;

        /* part1 = (i-1)(i-2)/2 * a(i-3) */
        seq_t m1 = (seq_t)(i - 1) * (i - 2);
        m1 /= (seq_t)2;
        term += m1 * cache[i - 3];

        /* part2 = (i-1)(i-2)(i-3)/6 * a(i-4) */
        seq_t m2 = (seq_t)(i - 1) * (i - 2) * (i - 3);
        m2 /= (seq_t)6;
        term += m2 * cache[i - 4];

        cache[i] = term;
    }

    /* Update logical size. */
    cache_len = n + 1;

    return cache[n];
}
