/*
 * A285741  a(0) = 1; a(2*n) = a(n), a(2*n+1) = a(n) + R(a(n)), where R() is
 *          the digit reversal.
 * https://oeis.org/A285741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1; a(2*n) = a(n); a(2*n+1) = a(n) + reverse_decimal(a(n))
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   ensure_capacity, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/reverse_decimal.h"
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Recursive helper that assumes cache is large enough for the requested index */
static seq_t rec(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                   /* the base value is never cached */

    /* use cached value if available */
    if (n < cache_cap && cache[n] != (seq_t)0)
        return cache[n];

    idx_t k = n >> 1;          /* floor(n/2) */
    seq_t val = rec(k);        /* a(k) */

    if (n & 1) {               /* odd index: add reverse of a(k) */
        seq_t rev = reverse_decimal(val);
        val = val + rev;
    }

    /* store computed value */
    cache[n] = val;
    return val;
}

/* public entry point */
seq_t a(idx_t n)
{
    /* ensure cache big enough for indices up to n */
    if (!cache) {
        ensure_capacity(n + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
        for (size_t i = 0; i < cache_cap; ++i)
            cache[i] = (seq_t)0;
    } else if (cache_cap <= n) {
        size_t old_cap = cache_cap;
        ensure_capacity(n + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
        for (size_t i = old_cap; i < cache_cap; ++i)
            cache[i] = (seq_t)0;
    }

    return rec(n);
}
