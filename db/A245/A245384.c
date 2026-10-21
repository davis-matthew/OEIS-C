/*
 * A245384  a(n) = (1 + a(n-1)) * a(n-2) * a(n-3) with a(1) = 1, a(2) = 2,
 *          a(3) = 3.
 * https://oeis.org/A245384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + a(n-1)) * a(n-2) * a(n-3) initial values: a(1)=1,
 *            a(2)=2, a(3)=3 (computed as a recurrence; memoised for arbitrary
 *            access order)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t */
#include "../utils/seq_alloc.h"    /* seq_alloc(seq_t *, size_t) */
#include "../utils/seq_realloc.h"  /* seq_realloc(seq_t *, size_t, size_t) */

seq_t a(idx_t n)
{
    /* static cache: indices are 1‑based; cache[0] unused          */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t len   = 0;   /* largest n with cache[n] valid */

    /* initialise cache with first three terms */
    if (len == 0) {
        cap   = 4;                      /* indices 0..3 */
        cache = seq_alloc(cap);
        if (!cache) abort();            /* construction failure */

        cache[0] = 0;   /* unused (no a(0) in OEIS definition) */
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 3;
        len = 3;
    }

    /* return cached value if already computed */
    if ((size_t)n <= len)
        return cache[n];

    /* grow cache to at least n+1 elements (indices 0..n) */
    if ((size_t)n >= cap) {
        size_t newcap = cap * 2;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = seq_realloc(cache, cap, newcap);
        if (!tmp) abort();            /* reallocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms iteratively */
    for (idx_t i = (idx_t)(len + 1); i <= n; ++i) {
        seq_t val = (1 + cache[(size_t)i - 1]) * cache[(size_t)i - 2] * cache[(size_t)i - 3];
        cache[(size_t)i] = val;
    }
    len = n;
    return cache[n];
}
