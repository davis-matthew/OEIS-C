/*
 * A387552  a(n) = (1/2) * Sum_{k=0..floor(n/3)} 2^k *
 *          binomial(2*k+2,2*n-6*k+1).
 * https://oeis.org/A387552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 39, u64 n <= 75, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A387552:
 * a(0) = 1, a(1) = 0, a(2) = 0, a(3) = 4, a(4) = 4, a(5) = 0,
 * a(6) = 12, a(7) = 40, a(n) = 4*a(n-3) + 4*a(n-4) - 4*a(n-6) +
 *                8*a(n-7) - 4*a(n-8)   for n ≥ 8
 *
 * The function is memoised and works for arbitrary‑precision or
 * limited‑width seq_t implementations.
 */

#include <stdlib.h>

/* Hard‑coded first eight terms */
static const seq_t _base[8] = {
    1, 0, 0, 4, 4, 0, 12, 40
};

/* Cached values, the size of the allocation, and the last
   successfully computed index (must be ≥ 7 after initialisation). */
static seq_t *cache       = NULL;
static idx_t  cache_size  = 0;
static idx_t  last_computed = (idx_t)-1;   /* sentinel */

/* Initialise the cache with the initial eight terms. */
static void init_cache(void)
{
    cache_size = 8;
    cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    if (!cache) return;          /* allocation failure – silent */
    for (idx_t i = 0; i < 8; ++i)
        cache[i] = _base[i];
    last_computed = 7;           /* indices 0..7 are now known */
}

/* Main function – computes a(n). */
seq_t a(idx_t n)
{
    /* Initialise on first use. */
    if (!cache)
        init_cache();

    /* If we already have the requested value, return it. */
    if (n <= last_computed)
        return cache[n];

    /* Increase the allocation if necessary. */
    if (n + (idx_t)1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)(n + (idx_t)1) * sizeof(seq_t));
        if (!tmp)               /* allocation failure – return 0 */
            return 0;
        cache = tmp;
        cache_size = n + (idx_t)1;
    }

    /* Compute the missing terms using the recurrence. */
    for (idx_t i = last_computed + (idx_t)1; i <= n; ++i) {
        cache[i] = 4 * cache[i - 3]
                 + 4 * cache[i - 4]
                 - 4 * cache[i - 6]
                 + 8 * cache[i - 7]
                 - 4 * cache[i - 8];
    }

    /* Update the last computed index and return the result. */
    last_computed = n;
    return cache[n];
}
