/*
 * A354319  Expansion of e.g.f. 1/(1 - 2*x)^(x/4).
 * https://oeis.org/A354319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A354319
    Formula: a(n) = (n-1)! * Sum_{k=2..n} k * 2^(k-3)/(k-1) * a(n-k)/(n-k)!
    Initial terms: a(0)=1, a(1)=0
    The function returns a(n) for any idx_t n ≥ 0.  Results are memoized
    so that calls in arbitrary order are efficient.  Only
    integer arithmetic is used (no floating point).  The code works
    for the unsigned integer types used by the build system. */

#include <stddef.h>
#include <stdlib.h>

/* Power‑of‑two helper supplied by the build system
   (seq_t pow2(idx_t exp) would also work, but we precompute
   explicit values to avoid repeated calls). */
seq_t pow2(idx_t exp);

seq_t a(idx_t n)
{
    /* static memoization tables */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;          /* number of valid entries in cache */

    static seq_t *pow2s = NULL;         /* 2^e table */
    static size_t pow2_cap = 0;
    static idx_t pow2_len = 0;          /* number of filled entries in pow2s */

    /* make sure cache is large enough */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (n + 1) * 2;
        if (new_cap < 16) new_cap = 16;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* make sure pow2 table is large enough */
    idx_t max_exp = (n >= 3) ? (n - 3) : 0;
    if ((size_t)max_exp >= pow2_cap) {
        size_t new_cap = (max_exp + 1) * 2;
        if (new_cap < 16) new_cap = 16;
        pow2s = (seq_t *)realloc(pow2s, new_cap * sizeof(seq_t));
        pow2_cap = new_cap;
    }

    /* initialise pow2 table entries that are not yet filled */
    for (idx_t i = pow2_len; i <= max_exp; ++i) {
        if (i == 0)
            pow2s[0] = (seq_t)1;
        else
            pow2s[i] = pow2s[i - 1] * (seq_t)2;
    }
    pow2_len = max_exp + 1;

    /* base case */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* compute new terms up to n */
    for (idx_t cur = cache_len; cur <= n; ++cur) {
        seq_t sum = 0;
        seq_t mult = 1;              /* (cur-1)!/(cur-k)!  part */

        for (idx_t k = 2; k <= cur; ++k) {
            /* update mult for this k: multiply by (cur-k+1) */
            mult *= (seq_t)(cur - k + 1);

            if (k == 2) {
                /* for k=2 the factor k*2^(k-3)/(k-1) equals 1 */
                seq_t term = cache[cur - 2] * mult;
                sum += term;
            } else {
                seq_t term = cache[cur - k];
                term *= mult;
                term *= (seq_t)k;
                term *= pow2s[k - 3];          /* 2^(k-3) */
                term /= (seq_t)(k - 1);        /* division is exact */
                sum += term;
            }
        }
        cache[cur] = sum;
    }
    cache_len = n + 1;
    return cache[n];
}
