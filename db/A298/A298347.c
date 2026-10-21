/*
 * A298347  a(n) = a(n-1) + a(n-2) + 2 a([n/2]), where a(0) = 1, a(1) = 2,
 *          a(2) = 3.
 * https://oeis.org/A298347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) + 2*a([n/2]), a(0)=1, a(1)=2, a(2)=3 */
#include <stdlib.h>

static seq_t *cache = NULL;     /* cache of computed values */
static size_t cache_cap = 0;    /* allocated capacity of cache */
static size_t cache_len = 0;    /* highest index computed + 1 (cache_len==0 means none computed) */

/* Extend the cache so that it can hold indices up to n (inclusive). */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;                /* already large enough */

    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;          /* double until sufficient */

    seq_t *tmp = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);    /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure the cache is large enough for n. */
    ensure_cache_capacity(n);

    /* Compute any missing entries up to index n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)2;
        } else if (i == 2) {
            cache[i] = (seq_t)3;
        } else {
            /* a(i) = a(i-1) + a(i-2) + 2*a([i/2]) */
            cache[i] = cache[i - 1] + cache[i - 2] + ((seq_t)2) * cache[i / 2];
        }
    }
    cache_len = (size_t)n + 1;   /* next free position is n+1 */

    return cache[(size_t)n];
}
