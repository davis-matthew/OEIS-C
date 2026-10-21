/*
 * A261317  Number of permutations sigma of [n] without fixed points such that
 *          sigma^6 = Id.
 * https://oeis.org/A261317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*a(n-2) + (n-1)*(n-2)*a(n-3) +
 *            (n-5)*(n-1)*(n-2)*(n-3)*(n-4)*a(n-6) for n ≥ 6 Initial values:
 *            a(0)=1, a(1)=0, a(2)=1, a(3)=2, a(4)=3, a(5)=20
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, exit */

static seq_t *cache = NULL;   /* dynamic array holding computed terms */
static idx_t cache_len = 0;   /* number of entries currently stored */

/* Initialise the cache with the first six terms. */
static void init_cache(void)
{
    cache = (seq_t *)malloc(6 * sizeof(seq_t));
    if (!cache) exit(1);
    cache[0] = (seq_t)1;   /* a(0) */
    cache[1] = (seq_t)0;   /* a(1) */
    cache[2] = (seq_t)1;   /* a(2) */
    cache[3] = (seq_t)2;   /* a(3) */
    cache[4] = (seq_t)3;   /* a(4) */
    cache[5] = (seq_t)20;  /* a(5) */
    cache_len = 6;
}

/* Ensure that the cache can hold values up to index n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    if (!cache) init_cache();

    if (n + 1 <= cache_len) return;   /* already large enough */

    idx_t old_len = cache_len;
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) exit(1);
    cache = new_cache;
    cache_len = new_len;

    /* Compute missing terms using the recurrence. */
    for (idx_t t = old_len; t <= n; ++t) {
        /* t ≥ 6 because old_len ≥ 6 after initialization. */
        seq_t t1 = (seq_t)(t - 1);
        seq_t term1 = cache[t - 2] * t1;

        seq_t term2 = cache[t - 3];
        term2 *= t1;
        term2 *= (seq_t)(t - 2);

        seq_t term3 = cache[t - 6];
        term3 *= (seq_t)(t - 5);
        term3 *= t1;
        term3 *= (seq_t)(t - 2);
        term3 *= (seq_t)(t - 3);
        term3 *= (seq_t)(t - 4);

        cache[t] = term1 + term2 + term3;
    }
}

/* Public function computing a(n). */
seq_t a(idx_t n)
{
    ensure_cache_size(n);
    return cache[n];
}
