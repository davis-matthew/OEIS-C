/*
 * A260414  Expansion of psi(x^3) * psi(x^6) / f(-x^4) in powers of x where
 *          phi(), f() are Ramanujan theta functions.
 * https://oeis.org/A260414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 156, u32 n <= 466, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   compute_series
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// /* OEIS A260414: expansion of psi(x^3)*psi(x^6)/f(-x^4) */
#include <stdlib.h>
#include "../utils/compute_series.h"

static seq_t *cache = NULL;    /* array holding computed coefficients */
static idx_t cache_max = 0;    /* largest index that has been computed */

seq_t a(idx_t n)
{
    if (!cache || n > cache_max) {
        /* allocate enough space for indices [0 .. n] */
        seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* out of memory */

        compute_series(n, new_cache);

        free(cache);
        cache = new_cache;
        cache_max = n;
    }
    return cache[n];
}
