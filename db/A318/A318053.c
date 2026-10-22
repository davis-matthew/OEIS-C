/*
 * A318053  a(n) = ceiling(sqrt(2*a(n-1)*a(n-2))), a(1) = a(2) = 1.
 * https://oeis.org/A318053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(sqrt(2*a(n-1)*a(n-2))), a(1) = a(2) = 1
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   ensure_cache, floor_sqrt, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                  /* for size_t                                   */
#include "../utils/ensure_cache.h"   /* ensure_cache(idx_t, seq_t**, size_t*)         */
#include "../utils/seq_mul.h"        /* seq_mul(seq_t, seq_t)                         */
#include "../utils/floor_sqrt.h"     /* floor_sqrt(seq_t)                             */

/* Static cache for memoisation                                           */
static seq_t *cache   = NULL;   /* array indexed from 0; ignore cache[0]        */
static size_t cap     = 0;      /* number of allocated slots                    */
static idx_t  max_en  = 0;      /* greatest index that has been computed        */

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)                                  /* offset is 1; undefined for n==0 */
        return 0;

    /* Ensure the cache has space for indices up to n (inclusive)       */
    ensure_cache((idx_t)(n + 1), &cache, &cap);

    /* Initialise base cases only once                               */
    if (cache[1] == 0) cache[1] = (seq_t)1;
    if (cache[2] == 0) cache[2] = (seq_t)1;

    /* If we have already computed up to n, return the cached value   */
    if (n <= max_en)
        return cache[n];

    /* Compute terms from the next missing index up to n              */
    idx_t start = (max_en > 2 ? max_en + 1 : 3);
    if (start > n) {                               /* nothing to compute          */
        max_en = n;
        return cache[n];
    }

    const seq_t two = (seq_t)2;                     /* 2, in type seq_t          */

    for (idx_t i = start; i <= n; ++i) {
        seq_t prod1 = seq_mul(cache[i - 1], cache[i - 2]); /* a[i-1]*a[i-2]      */
        seq_t prod2 = seq_mul(prod1, two);                 /* 2*a[i-1]*a[i-2]    */
        seq_t sq    = floor_sqrt(prod2);                   /* floor(sqrt(...))   */
        seq_t sqsq  = seq_mul(sq, sq);                     /* sq^2               */

        cache[i] = (sqsq == prod2 ? sq : sq + 1);          /* ceil(sqrt(...))    */
    }

    max_en = n;
    return cache[n];
}
