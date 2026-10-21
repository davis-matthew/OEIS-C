/*
 * A264191  Number of (n+1)X(2+1) arrays of permutations of 0..n*3+2 with each
 *          element having index change +-(.,.) 0,0 0,2 or 2,1.
 * https://oeis.org/A264191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(n) = 4*a(n-1) + a(n-2) + 68*a(n-5) + 17*a(n-6) + 17*a(n-8) + a(n-12)
          - 17*a(n-4) - 68*a(n-9) - 17*a(n-10) - 4*a(n-13) - a(n-14)
   for n >= 15.
   Initial values (n = 1 … 14) are taken from the sequence definition. */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* 1‑indexed cache of computed terms */
static idx_t cache_cap = 0;       /* allocated capacity, including index 0 */
static idx_t cache_size = 0;      /* largest n that has been computed, 0 if none */

static const seq_t init_vals[15] = {
    0ULL,          /* dummy for index 0 */
    4ULL,          /* a(1) */
    18ULL,         /* a(2) */
    81ULL,         /* a(3) */
    360ULL,        /* a(4) */
    1600ULL,       /* a(5) */
    6760ULL,       /* a(6) */
    28561ULL,      /* a(7) */
    120666ULL,     /* a(8) */
    509796ULL,     /* a(9) */
    2159850ULL,    /* a(10) */
    9150625ULL,    /* a(11) */
    38768400ULL,   /* a(12) */
    164249856ULL,  /* a(13) */
    695767824ULL   /* a(14) */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; request for n==0 undefined */

    /* Ensure cache has space for indices 1..n. */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        if (new_cap < 100) new_cap = 100;          /* small default */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                         /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise newly allocated portion with zeros (except for index 0). */
    if (cache_size < n) {
        for (idx_t i = cache_size + 1; i <= n; ++i)
            cache[i] = 0;
    }

    /* Compute terms that are not yet cached. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 14) {
            cache[i] = init_vals[i];
            continue;
        }

        /* Positive part of the recurrence. */
        seq_t pos = 4 * cache[i-1]
                  + cache[i-2]
                  + 68 * cache[i-5]
                  + 17 * cache[i-6]
                  + 17 * cache[i-8]
                  + cache[i-12];

        /* Negative part of the recurrence. */
        seq_t neg = 17 * cache[i-4]
                  + 68 * cache[i-9]
                  + 17 * cache[i-10]
                  + 4  * cache[i-13]
                  +    cache[i-14];

        cache[i] = pos - neg;
    }

    cache_size = n;
    return cache[n];
}
