/*
 * A295270  Number of n X 3 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 1 or 4 1s.
 * https://oeis.org/A295270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 10*a(n-2) + 20*a(n-3) + 17*a(n-4) * - a(n-5) -
 *            9*a(n-6) - 12*a(n-7) - a(n-8) - a(n-9) + a(n-10) * with a(1)=7,
 *            a(2)=33, a(3)=164, a(4)=811, a(5)=4035, * a(6)=19997,
 *            a(7)=99245, a(8)=492401, a(9)=2443097, * a(10)=12121712
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   free_seq, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"   /* provides seq_alloc_new_array() */
#include "../utils/free_seq.h"              /* provides free_seq() */

/* Cache for memoisation – index 0 is unused. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;          /* current allocated size (≥ 1) */
static idx_t  cache_max  = 0;          /* largest n already computed */

/* Base values a(1)..a(10) */
static const seq_t base[11] = {
    0,          /* dummy for index 0 */
    7,          /* a(1) */
    33,         /* a(2) */
    164,        /* a(3) */
    811,        /* a(4) */
    4035,       /* a(5) */
    19997,      /* a(6) */
    99245,      /* a(7) */
    492401,     /* a(8) */
    2443097,    /* a(9) */
    12121712    /* a(10) */
};

/* Ensure cache contains values up to n (inclusive). */
static void compute_to(idx_t n)
{
    if (n == 0)
        return;                 /* nothing to do for index 0 */

    if ((idx_t)cache_max >= n)
        return;                 /* already computed */

    /* Allocate or grow the cache array.  Use the helper that
       constructs objects correctly for arbitrary‑precision types. */
    if ((size_t)(n + 1) > cache_cap) {
        seq_t *tmp = seq_alloc_new_array((size_t)(n + 1));
        if (!tmp)
            exit(EXIT_FAILURE);
        /* copy old values (if any) */
        size_t i;
        for (i = 0; i < cache_cap; ++i)
            tmp[i] = cache[i];
        if (cache)                /* free old buffer */
            free_seq(cache);
        cache = tmp;
        cache_cap = (size_t)(n + 1);
    }

    /* Compute missing terms. */
    idx_t k;
    for (k = cache_max + 1; k <= n; ++k) {
        if (k <= 10) {
            cache[k] = base[(size_t)k];
        } else {
            seq_t pos = cache[k - 1];
            pos += cache[k - 1];                     /* 2 * a(k-1) */
            pos += cache[k - 2] * (seq_t)10;         /* 10 * a(k-2) */
            pos += cache[k - 3] * (seq_t)20;         /* 20 * a(k-3) */
            pos += cache[k - 4] * (seq_t)17;         /* 17 * a(k-4) */
            pos += cache[k - 10];                    /* + a(k-10) */

            seq_t neg = cache[k - 5];
            neg += cache[k - 6] * (seq_t)9;          /* 9 * a(k-6) */
            neg += cache[k - 7] * (seq_t)12;         /* 12 * a(k-7) */
            neg += cache[k - 8];                    /* + a(k-8) */
            neg += cache[k - 9];                    /* + a(k-9) */

            cache[k] = pos - neg;
        }
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* outside OEIS offset – return 0 */
    compute_to(n);
    return cache[(size_t)n];
}
