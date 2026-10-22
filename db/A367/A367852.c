/*
 * A367852  Expansion of e.g.f. 1/(1 - x + log(1 - 3*x)/3).
 * https://oeis.org/A367852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A367852
   Formula: a(0) = 1;
   a(n) = n*a(n-1) + ∑_{k=1..n} 3^(k-1)*(k-1)!*C(n,k)*a(n-k)   (n ≥ 1)
*/
#include <stdlib.h>

/* Computes the n-th term of A367852.  Results are memoised in a static cache
   so that later calls with larger n are fast and earlier calls are still
   answered correctly.  The algorithm uses an O(n²) recurrence but each call
   recomputes only up to the largest n requested so far.  This stays well
   within the time limits for n up to about 10 000 even on 64‑bit builds. */
seq_t a(idx_t n)
{
    /* Static cache for previously computed a(k). */
    static seq_t *cache      = NULL;
    static idx_t cache_cap  = 0;
    static idx_t cache_len  = 0;      /* number of values currently stored */

    /* Static cache for B(k) = 3^(k-1)*(k-1)!.  We store indices from 1. */
    static seq_t *B          = NULL;
    static idx_t B_cap      = 0;
    static idx_t B_len      = 0;      /* next free index (i.e. number of
                                       elements already computed + 1 ) */

    /* Allocate / grow the cache if the requested n is outside the current
       cached range. */
    if (n >= cache_len) {
        idx_t need = n + 1;               /* need indices 0..n */

        /* Grow the a(n) cache. */
        if (need > cache_cap) {
            size_t newcap = cache_cap ? cache_cap * 2 : 64;
            while (newcap < need) newcap <<= 1;
            seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cache = newptr;
            cache_cap = (idx_t)newcap;
        }

        /* Grow the B(k) cache. */
        if (need > B_cap) {
            size_t newcap = B_cap ? B_cap * 2 : 64;
            while (newcap < need) newcap <<= 1;
            seq_t *newptr = (seq_t *)realloc(B, newcap * sizeof(seq_t));
            B = newptr;
            B_cap = (idx_t)newcap;
        }

        /* Initialise the memoised sequences if this is the first call. */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;          /* a(0) = 1 */
            cache_len = 1;
        }
        if (B_len == 0) {
            B[1] = (seq_t)1;              /* B(1) = 1 (=3^0*0!) */
            B_len = 2;                    /* indices 0..1 are now valid */
        }

        /* Compute a(k) for all k from cache_len up to n. */
        for (idx_t cur = cache_len; cur <= n; ++cur) {
            /* Ensure B(k) is available for all k ≤ cur. */
            while (B_len <= cur) {
                idx_t k = B_len;
                /* B(k) = B(k-1) * 3 * (k-1)  */
                seq_t mult = (seq_t)3 * (seq_t)(k - 1);
                B[k] = B[k - 1] * mult;
                ++B_len;
            }

            /* Sum over k = 1..cur.  We iterate binomial coefficients
               multiplicatively to keep intermediate sizes small. */
            seq_t sum   = (seq_t)0;
            seq_t binom = (seq_t)cur;      /* C(cur,1) */

            for (idx_t k = 1; k <= cur; ++k) {
                seq_t term = B[k] * binom * cache[cur - k];
                sum += term;
                if (k < cur) {
                    /* binom(C(cur,k+1)) = binom * (cur-k)/(k+1) */
                    binom = binom * (seq_t)(cur - k) / (seq_t)(k + 1);
                }
            }

            /* a(cur) = cur * a(cur-1) + sum.  Cur must be cast to seq_t. */
            seq_t val = (seq_t)cur * cache[cur - 1] + sum;
            cache[cur] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
