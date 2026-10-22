/*
 * A343788  Number of ordered partitions of an n-set without blocks of size 5.
 * https://oeis.org/A343788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{s=1, s≠5}^{n} C(n, s) * a(n-s) Equivalent recurrence
 *            derived from the e.g.f
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_max = 0;   /* highest n for which cache[n] is valid */

/* Ensure cache has room for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    idx_t needed = n + 1;
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap == 0 ? 1 : cache_cap;
    while (new_cap < needed) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = (idx_t)new_cap;
}

seq_t a(idx_t n)
{
    /* initialise a(0) on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache_cap = 1;
        cache_max = 0;
        cache[0] = (seq_t)1;   /* a(0) = 1 */
    }
    if (n == 0) return cache[0];
    ensure_capacity(n);

    /* compute any missing values up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;          /* C(i, 0) */
        for (idx_t j = 0; j <= i - 1; ++j) {
            if (j > 0)
                binom = (binom * (seq_t)(i - j + 1)) / (seq_t)j;
            sum += binom * cache[j];
        }
        if (i >= 5) {
            seq_t binom5 = 1;
            for (idx_t k = 1; k <= 5; ++k)
                binom5 = (binom5 * (seq_t)(i - k + 1)) / (seq_t)k;   /* C(i,5) */
            sum -= binom5 * cache[i - 5];
        }
        cache[i] = sum;
    }
    cache_max = n;
    return cache[n];
}
