/*
 * A376087  a(0) = 1; a(n) = Sum_{k=0..n-1} (4*k+1) * a(k) * a(n-k-1).
 * https://oeis.org/A376087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..n-1} (4*k+1) * a(k) * a(n-k-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache   = NULL;   /* cached a(n) values */
static size_t cap    = 0;       /* allocated capacity of cache   */
static size_t len    = 0;       /* number of computed terms (len==0 means none computed) */

/* Ensure cache has room for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;            /* include index n */
    if (need > cap) {
        size_t newcap = (cap > 0) ? cap * 2 : 32;
        while (newcap < need)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort (no error handling per spec) */
            abort();
        }
        /* Initialise new entries to zero */
        for (size_t i = cap; i < newcap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cap = newcap;
    }
}

/* Compute a(n) with memoisation. */
seq_t a(idx_t n)
{
    if (len == 0) {          /* initialise a(0) */
        ensure_cache(0);
        cache[0] = 1;
        len = 1;
    }

    if (n < len)
        return cache[n];

    ensure_cache(n);

    for (idx_t m = len; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 0; k < m; ++k) {
            seq_t w = (seq_t)(4 * k + 1);
            seq_t prod = w * cache[k] * cache[m - 1 - k];
            sum += prod;
        }
        cache[m] = sum;
    }

    len = (size_t)n + 1;
    return cache[n];
}
