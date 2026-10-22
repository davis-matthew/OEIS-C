/*
 * A367877  Expansion of e.g.f. exp(x * (3 - 2*exp(x))).
 * https://oeis.org/A367877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..n} (-2)^k * (k+3)^(n-k) * binomial(n,k)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 13, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the e.g.f. representation:   a(0)=1,   */
/* a(n+1)=Σ_{k=0}^n binomial(n,k) * a(n-k) * h(k) with  h(0)=1, h(k)=-(2+2k) (k>0). */
#include <stdlib.h>
#include <stdint.h>

/* Allocate or enlarge the cache that holds computed values
   up to the highest index requested so far. */
static seq_t *a_cache = NULL;
static idx_t a_cache_len = 0;   /* number of entries stored (cache_len = highest + 1) */
static idx_t a_cache_cap = 0;   /* current capacity of cache */

static void ensure_cache_capacity(idx_t needed)
{
    if (needed <= a_cache_cap) return;
    idx_t new_cap = a_cache_cap ? a_cache_cap << 1 : 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache = new_ptr;
    a_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise cache if this is the first call. */
    if (a_cache == NULL) {
        a_cache_len = 0;
        a_cache_cap = 0;
        a_cache = NULL;
    }

    if (n >= a_cache_len) {
        /* Expand cache to cover all indices up to n. */
        ensure_cache_capacity(n + 1);

        /* Compute new values using the recurrence. */
        for (idx_t i = a_cache_len; i <= n; ++i) {
            if (i == 0) {                      /* a(0) = 1 */
                a_cache[0] = 1;
                continue;
            }
            idx_t m = i - 1;                    /* compute a(i)   */
            seq_t sum   = 0;
            seq_t binom = 1;                    /* binomial(m,0)  */

            for (idx_t k = 0; k <= m; ++k) {
                idx_t j = m - k;                /* index of a(j)   */
                seq_t hk;
                if (k == 0) hk = 1;
                else        hk = -(seq_t)(2 + 2 * k); /* h(k)          */

                seq_t term = a_cache[j] * hk;   /* a(j) * h(k)    */
                term = term * binom;            /* multiply by C(m,k) */
                sum += term;                    /* accumulate       */

                if (k < m) {
                    /* update binomial coefficient for next k:
                       binom(new) = binom(old) * (m-k) / (k+1)   */
                    binom = binom * (m - k) / (k + 1);
                }
            }
            a_cache[i] = sum;
        }
        a_cache_len = n + 1;
    }
    return a_cache[n];
}
