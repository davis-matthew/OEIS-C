/*
 * A247859  The product of the first n Catalan numbers and 2^(n^2).
 * https://oeis.org/A247859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n²) * ∏_{k=0..n} Catalan(k) (OEIS A247859)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   catalan, ensure_capacity, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/catalan.h"
#include "../utils/pow2.h"
#include "../utils/seq_mul.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* static cache for computed values and cumulative product of Catalan numbers */
    static seq_t *cache = NULL;      /* a(0..cached_n) */
    static size_t cap   = 0;         /* current capacity of cache array */
    static idx_t  cached_n = (idx_t)-1; /* maximum n for which cache is filled */
    static seq_t prod_cur = 0;       /* product of Catalan(k) for k=0..cached_n */

    /* First‑time initialisation: compute a(0) */
    if (cached_n == (idx_t)-1) {
        ensure_capacity(1, &cap, (void **)&cache, sizeof(seq_t));
        prod_cur = catalan(0);                     /* = 1 */
        cache[0]  = seq_mul(pow2(0), prod_cur);    /* a(0) = 1 */
        cached_n  = 0;
    }

    /* If the requested index is already computed, return it. */
    if (n <= cached_n) {
        return cache[n];
    }

    /* Extend the cache up to the requested index */
    for (idx_t k = cached_n + 1; k <= n; ++k) {
        seq_t cat = catalan(k);                    /* Catalan(k) */
        prod_cur = seq_mul(prod_cur, cat);         /* update cumulative product */
        seq_t a_k   = seq_mul(pow2(k * k), prod_cur);
        if (k >= cap) {
            ensure_capacity((size_t)k + 1, &cap, (void **)&cache, sizeof(seq_t));
        }
        cache[k] = a_k;
    }
    cached_n = n;
    return cache[n];
}
