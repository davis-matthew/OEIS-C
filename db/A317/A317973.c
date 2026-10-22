/*
 * A317973  a(n) = 2*(a(n-1)+a(n-2)+a(n-3))-a(n-4) for n >= 4, with initial
 *          terms -1, 2, 3, 6.
 * https://oeis.org/A317973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*(a(n-1)+a(n-2)+a(n-3)) - a(n-4) for n>=4 with initial
 *            terms: a(0) = -1, a(1) = 2, a(2) = 3, a(3) = 6
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 21, i64 n <= 42, i128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* dynamic array of computed terms */
static size_t seq_cache_cap = 0;  /* allocated capacity of seq_cache   */
static idx_t  seq_cache_last = (idx_t)-1; /* highest n for which seq_cache[n] is valid */

/* Ensure the cache has room for at least n+1 entries */
static void ensure_cache(idx_t n)
{
    if (seq_cache_cap > n)
        return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 16;
    while (new_cap <= n)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    seq_cache = tmp;
    seq_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* First call – initialize cache with the initial four terms */
    if (seq_cache == NULL) {
        ensure_cache(3);
        seq_cache[0] = (seq_t)(-1);
        seq_cache[1] = (seq_t)2;
        seq_cache[2] = (seq_t)3;
        seq_cache[3] = (seq_t)6;
        seq_cache_last = 3;
    }

    /* If requested value already computed, return it */
    if (n <= seq_cache_last)
        return seq_cache[n];

    /* Compute missing terms up to n */
    ensure_cache(n);
    for (idx_t i = seq_cache_last + 1; i <= n; ++i) {
        seq_cache[i] = 2 * (seq_cache[i - 1] + seq_cache[i - 2] + seq_cache[i - 3])
                       - seq_cache[i - 4];
    }
    seq_cache_last = n;
    return seq_cache[n];
}
