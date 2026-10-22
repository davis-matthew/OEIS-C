/*
 * A334277  Perimeters of almost-equilateral Heronian triangles.
 * https://oeis.org/A334277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2); a(1) = 12, a(2) = 42
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of computed terms, 1‑based indexing */
static seq_t *cache = NULL;          /* array of terms */
static idx_t  cap   = 0;             /* allocated size of cache (indexable up to cap-1) */
static idx_t  max_n = 0;             /* largest n for which cache[n] is valid */

/* ensure that cache contains terms up to at least n (inclusive) */
static void ensure(idx_t n)
{
    /* Allocate when first needed */
    if (max_n == 0) {
        /* always allocate at least two entries to hold a(1) and a(2) */
        idx_t new_cap = (n + 1 >= 3) ? (n + 1) : 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (!cache) return;          /* out of memory – caller will still receive garbage */
        cap = new_cap;
        /* seed initial values */
        cache[1] = (seq_t)12;
        cache[2] = (seq_t)42;
        max_n = (n >= 2) ? 2 : 1;   /* if n==1 we already have term 1 */
    }

    /* Grow the cache if necessary */
    if (n + 1 > cap) {
        idx_t new_cap = (cap * 2 > n + 1) ? cap * 2 : n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return;          /* out of memory – ignore growth */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)12;
        } else if (i == 2) {
            cache[2] = (seq_t)42;
        } else {
            cache[i] = (seq_t)(4 * cache[i-1] - cache[i-2]);
        }
    }
    if (n > max_n) max_n = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* undefined, but avoid crash */
    ensure(n);
    return cache[n];
}
