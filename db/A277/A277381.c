/*
 * A277381  a(n) = exp(-1) * Sum_{k>=0} H_n(k)/k!, where H_n(x) is n-th
 *          Hermite polynomial.
 * https://oeis.org/A277381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A277381
 * Formula: a(0)=1.
 * For n>=2:  a(n) = 2 * Σ_{i=0}^{n-1} C(n-1,i) * 2^i * a(n-1-i)
 *            - 2*(n-1)*a(n-2)
 *
 * Same recurrence is used for n=1 with a(-1)=0.
 * The implementation keeps a static cache so that subsequent
 * calls are O(n) overall.  All arithmetic is performed on type
 * seq_t and uses only integer operators, as required.
 */

#include <stdlib.h>
#include <stddef.h>

/* --- static cache for computed values --------------------------------- */
static seq_t *cache       = NULL;   /* array holding a(0) ... a(max)       */
static idx_t  cache_size  = 0;      /* allocated size (>= max+1)            */
static idx_t  max_computed = 0;     /* largest n for which a(n) is known    */

/* Ensure that the cache has room for indices up to new_size-1. */
static void ensure_capacity(idx_t new_size)
{
    if (cache_size < new_size) {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* In a production setting we would handle realloc failure. */
        cache = tmp;
        cache_size = new_size;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* First call: allocate cache, store a(0)=1 */
    if (cache == NULL) {
        ensure_capacity(1);             /* at least space for a(0) */
        cache[0] = 1;
        max_computed = 0;
    }

    /* Grow cache to accommodate a(n) if necessary */
    if (n >= cache_size) {
        ensure_capacity(n + 1);
    }

    /* Compute missing terms up to n */
    if (n > max_computed) {
        /* Compute a(m+1) for m = max_computed .. n-1 */
        for (idx_t m = max_computed; m < n; ++m) {
            /* Compute a(m+1) via the recurrence */
            seq_t sum   = 0;        /* Σ_{i=0}^{m} C(m,i) 2^i a(m-i)   */
            seq_t binom = 1;        /* C(m,0)                          */
            seq_t pow2  = 1;        /* 2^0                             */

            for (idx_t i = 0; i <= m; ++i) {
                /* term = binom * pow2 * a(m-i) */
                seq_t term = binom;
                term *= pow2;
                term *= cache[m - i];
                sum   += term;

                /* update binom for next i (if i<m) */
                if (i < m) {
                    binom = binom * (seq_t)(m - i) / (seq_t)(i + 1);
                }

                /* update pow2 for next i */
                pow2 *= 2;
            }

            /* multiply the sum by 2 */
            sum += sum;

            /* subtract 2*m*a(m-1) if m>=1 */
            if (m >= 1) {
                seq_t sub = cache[m - 1];
                sub *= (seq_t)2 * (seq_t)m;
                sum -= sub;
            }

            cache[m + 1] = sum;
        }
        max_computed = n;
    }

    return cache[n];
}
