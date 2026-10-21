/*
 * A289789  p-INVERT of A016777, where p(S) = 1 - S - S^2.
 * https://oeis.org/A289789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=6, a(2)=26, a(3)=111; for n≥4: a(n)=5*a(n‑1) –
 *            5*a(n‑2) + 5*a(n‑3) + 5*a(n‑4)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute term a(n) of OEIS A289789, with memoisation. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holding computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with the first four terms on first call */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* defensive, should not happen */
        cache[0] = 1;
        cache[1] = 6;
        cache[2] = 26;
        cache[3] = 111;
    }

    /* return cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* grow cache to hold index n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        free(cache);
        cache = NULL;
        cache_size = 0;
        return (seq_t)0;                      /* defensive */
    }
    cache = tmp;

    /* zero initialise new slots (not strictly needed but safe) */
    for (idx_t i = cache_size; i < new_size; ++i)
        cache[i] = (seq_t)0;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_size; i < new_size; ++i) {
        cache[i] =
            (seq_t)5 * cache[i - 1]
          - (seq_t)5 * cache[i - 2]
          + (seq_t)5 * cache[i - 3]
          + (seq_t)5 * cache[i - 4];
    }

    cache_size = new_size;
    return cache[n];
}
