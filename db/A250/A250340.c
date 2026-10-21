/*
 * A250340  Number of length 4+5 0..n arrays with every six consecutive terms
 *          having the maximum of some three terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A250340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9); Initial
 *            values: a(1) = 172 a(2) = 4409 a(3) = 37824 a(4) = 193437 a(5) =
 *            725596 a(6) = 2210213 a(7) = 5794496 a(8) = 13561449 a(9) =
 *            29037004 (subsequent values are computed with the abo...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 18, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The sequence values are stored in a static cache that expands on demand. */
static seq_t *seq_cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Precomputed first nine terms of the sequence. */
    static const seq_t init[9] = {
        (seq_t)172,
        (seq_t)4409,
        (seq_t)37824,
        (seq_t)193437,
        (seq_t)725596,
        (seq_t)2210213,
        (seq_t)5794496,
        (seq_t)13561449,
        (seq_t)29037004
    };

    /* Initialise cache with the first nine terms if not yet done. */
    if (cache_size == 0) {
        cache_size = 9;
        seq_cache  = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!seq_cache) abort();               /* out‑of‑memory */
        for (idx_t i = 0; i < cache_size; ++i)
            seq_cache[i] = init[i];
    }

    /* If the requested index is within the cached range, just return it. */
    if (n <= cache_size)
        return seq_cache[n - 1];

    /* Extend the cache up to the requested index using the recurrence. */
    seq_t *new_cache = (seq_t *)realloc(seq_cache, n * sizeof(seq_t));
    if (!new_cache) abort();                   /* out‑of‑memory */
    seq_cache = new_cache;

    /* Compute consecutive terms from cache_size+1 up to n. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* Indices of the terms needed from the cache (zero‑based). */
        idx_t a1 = i - 2;   /* a(i-1) */
        idx_t a2 = i - 3;   /* a(i-2) */
        idx_t a3 = i - 4;   /* a(i-3) */
        idx_t a4 = i - 5;   /* a(i-4) */
        idx_t a5 = i - 6;   /* a(i-5) */
        idx_t a6 = i - 7;   /* a(i-6) */
        idx_t a7 = i - 8;   /* a(i-7) */
        idx_t a8 = i - 9;   /* a(i-8) */
        idx_t a9 = i -10;   /* a(i-9) */

        /* Positive part of the recurrence. */
        seq_t pos = (seq_t)9 * seq_cache[a1] +
                    (seq_t)84 * seq_cache[a3] +
                    (seq_t)126 * seq_cache[a5] +
                    (seq_t)36 * seq_cache[a7] +
                    seq_cache[a9];

        /* Negative part of the recurrence. */
        seq_t neg = (seq_t)36 * seq_cache[a2] +
                    (seq_t)126 * seq_cache[a4] +
                    (seq_t)84 * seq_cache[a6] +
                    (seq_t)9 * seq_cache[a8];

        /* The new term: difference of the two parts (pos >= neg). */
        seq_t val = pos - neg;

        seq_cache[i - 1] = val;
    }

    cache_size = n;
    return seq_cache[n - 1];
}
