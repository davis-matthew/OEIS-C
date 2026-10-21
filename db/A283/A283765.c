/*
 * A283765  Numbers k such that L(k) is even, where L = A000201 = lower
 *          Wythoff sequence.
 * https://oeis.org/A283765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 1299, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A283765 : k such that the lower Wythoff sequence L(k)=⌊k·φ⌋ is even
   (φ = (1+√5)/2).  The function generates the terms in increasing order
   and caches them for fast repeated access.  L(k) is computed with the
   integer identity

        L(k) = ⌊(k + ⌊√(5k²)⌋)/2⌋

   which is exact for all k ≥ 0.  The helper `isqrt_seq` performs an
   integer square‑root by binary search on a `seq_t` value. */

#include <stdlib.h>
#include <stddef.h>

/* integer square root of a seq_t value */
static seq_t isqrt_seq(seq_t x)
{
    seq_t low  = 0;
    seq_t high = x;
    seq_t ans  = 0;
    while (low <= high) {
        seq_t mid  = (low + high) / 2;          /* mid = floor((low+high)/2) */
        seq_t mid2 = mid * mid;
        if (mid2 <= x) {
            ans  = mid;
            low  = mid + 1;
        } else {
            if (mid == 0) break;                /* avoid underflow when mid==0 */
            high = mid - 1;
        }
    }
    return ans;
}

/* generated terms cache */
static seq_t *cache      = NULL;      /* array of stored k values   */
static size_t cache_cap  = 0;         /* allocated capacity        */
static size_t cache_sz   = 0;         /* number of cached elements */
static idx_t  last_k     = 0;         /* last k processed (≥ 0)    */

/* grow the cache as needed */
static void cache_grow(void)
{
    if (cache_sz == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* out of memory */
        cache      = tmp;
        cache_cap  = new_cap;
    }
}

/* public sequence function
   a(1) = 3, a(2) = 4, ...   (offset = 1) */
seq_t a(idx_t n)
{
    /* require n ≥ 1 by OEIS specification */
    if (n == 0) return (seq_t)0;           /* graceful handling of invalid 0 */

    /* generate additional terms until we have enough */
    while (cache_sz < (size_t)n) {
        last_k++;                          /* next k to test (starting from 1) */
        seq_t k = (seq_t)last_k;

        /* compute L(k) = floor((k + floor(sqrt(5k²)))/2) */
        seq_t kk      = k * k;
        seq_t five    = 5;
        seq_t sq      = kk * five;
        seq_t sqrt_f  = isqrt_seq(sq);
        seq_t Lk      = (k + sqrt_f) / 2;

        /* store k if Lk is even */
        if ((Lk & 1) == 0) {
            cache_grow();
            cache[cache_sz++] = k;
        }
    }

    return cache[n - 1];
}
