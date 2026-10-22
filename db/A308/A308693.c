/*
 * A308693  a(n) = Sum_{d|n} d^(3*(n/d - 1)).
 * https://oeis.org/A308693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(3*(n/d - 1))
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"

/* Cache for already computed values */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;

/* Re‑compute the cache up to at least n */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    /* Reset the cache */
    free(cache);
    cache = (seq_t*)calloc((size_t)n, sizeof(seq_t));
    if (!cache) {
        cache_size = 0;          /* keep size 0 so future calls try again */
        return;
    }

    /* Enumerate all pairs (k , m) with n = k * m <= n
       and add m^(3*(k-1)) to cache[n-1] */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t exp = (seq_t)(3 * (k - 1));
        idx_t max_m = n / k;
        for (idx_t m = 1; m <= max_m; ++m) {
            idx_t pos = k * m;           /* 1‑based index of n = k*m */
            seq_t term = big_pow((seq_t)m, exp);
            cache[pos - 1] += term;
        }
    }
    cache_size = n;
}

/* Public interface */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset starts at 1 – unused case */

    ensure_cache(n);
    return cache[n - 1];
}
