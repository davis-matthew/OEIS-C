/*
 * A391679  Number of integer compositions of n that (1) have all parts > 1
 *          and (2) are not the first sums of any finite nonnegative sequence.
 * https://oeis.org/A391679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 2*a(n-3) - 4*a(n-4) - 3*a(n-5) -
 *            2*a(n-6) + 5*a(n-7) + 5*a(n-8) + 2*a(n-9) - 2*a(n-11) - a(n-12)
 *            Initial values:
 *            a(1)=0,a(2)=0,a(3)=0,a(4)=0,a(5)=0,a(6)=0,a(7)=0,a(8)=0,
 *            a(9)=1,a(10)=1,a(11)=5,a(12)=7
 * Range:     u8 n <= 16, u16 n <= 26, u32 n <= 49, u64 n <= 94, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* array indexed from 1 */
static idx_t  cache_cap = 0;       /* allocated size (including index 0) */
static idx_t  max_computed = 0;    /* largest n for which a(n) is stored */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_cap < n + 1) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache = new_cache;
        cache_cap = n + 1;
    }
}

/* Initialise the first 12 values of the sequence. */
static void init_cache(void)
{
    ensure_cache(12);
    cache[1] = cache[2] = cache[3] = cache[4] = cache[5] = 0;
    cache[6] = cache[7] = cache[8] = 0;
    cache[9] = 1;
    cache[10] = 1;
    cache[11] = 5;
    cache[12] = 7;
    max_computed = 12;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* out of offset range, undefined behaviour allowed */

    /* Initialise cache on first call. */
    if (max_computed == 0)
        init_cache();

    if (n <= max_computed)
        return cache[n];

    /* Extend cache and compute missing values up to n. */
    ensure_cache(n);
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        /* k >= 13 because we already have values up to 12. */
        seq_t val = cache[k - 1]                     /* a(k-1) */
                + 4 * cache[k - 2]                   /* +4*a(k-2) */
                - 2 * cache[k - 3]                   /* -2*a(k-3) */
                - 4 * cache[k - 4]                   /* -4*a(k-4) */
                - 3 * cache[k - 5]                   /* -3*a(k-5) */
                - 2 * cache[k - 6]                   /* -2*a(k-6) */
                + 5 * cache[k - 7]                   /* +5*a(k-7) */
                + 5 * cache[k - 8]                   /* +5*a(k-8) */
                + 2 * cache[k - 9]                   /* +2*a(k-9) */
                - 2 * cache[k - 11]                  /* -2*a(k-11) */
                -   cache[k - 12];                  /* -a(k-12) */
        cache[k] = val;
    }
    max_computed = n;
    return cache[n];
}
