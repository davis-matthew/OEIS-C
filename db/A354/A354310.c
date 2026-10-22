/*
 * A354310  Expansion of e.g.f. 1/(1 - 3*x)^(x/3).
 * https://oeis.org/A354310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A354310: Expansion of e.g.f. 1/(1 - 3*x)^(x/3)
   a(0) = 1
   For n >= 2:
   a(n) = (n-1)! * sum_{k=2..n} k * 3^(k-2)/(k-1) * a(n-k)/(n-k)!.
   Recurrence is implemented via integer arithmetic so that each
   intermediate value fits seq_t.  The sequence is memoised in a
   static array so that arbitrary calls to a(idx_t n) are O(n) in total. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Static cache for a(n) values */
static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;
static size_t a_cache_len = 0;

/* Ensure the cache contains values up to index n (inclusive). */
static void ensure_cache_up_to(idx_t n)
{
    if (n < a_cache_len)
        return; /* already computed */

    /* Resize cache if necessary */
    if ((size_t)(n + 1) > a_cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            exit(EXIT_FAILURE); /* out of memory */
        a_cache = new_cache;
        a_cache_cap = new_cap;
    }

    /* Compute missing values */
    for (idx_t m = a_cache_len; m <= n; ++m) {
        if (m == 0) {
            a_cache[0] = 1;
            continue;
        }
        if (m == 1) {
            a_cache[1] = 0;
            continue;
        }

        /* a(m) = sum_{k=2..m} C(m-1,k-1) * k * 3^(k-2) * (k-2)! * a(m-k) */
        seq_t acc = 0;

        /* Initialize for k = 2 */
        seq_t binom = (seq_t)(m - 1);            /* C(m-1,1) */
        seq_t pow3 = 1;                          /* 3^(k-2) with k=2 => 3^0 = 1 */
        seq_t fact_k_minus2 = 1;                 /* (k-2)! with k=2 => 0! = 1 */

        for (idx_t k = 2; k <= m; ++k) {
            /* term = binom * k * pow3 * fact_k_minus2 * a(m-k) */
            seq_t term = binom;
            term *= (seq_t)k;
            term *= pow3;
            term *= fact_k_minus2;
            term *= a_cache[(size_t)(m - k)];
            acc += term;

            if (k == m)
                break; /* last iteration, no need to update */

            /* Update binomial coefficient: C(m-1,k) = C(m-1,k-1) * (m-k)/k */
            binom = binom * (seq_t)(m - k);
            binom /= (seq_t)k;

            /* Update 3^(k-2) and (k-2)! for next k */
            pow3 *= 3;
            fact_k_minus2 *= (seq_t)(k - 1);
        }

        a_cache[(size_t)m] = acc;
    }

    a_cache_len = (size_t)(n + 1);
}

seq_t a(idx_t n)
{
    if (n >= a_cache_len)
        ensure_cache_up_to(n);
    return a_cache[(size_t)n];
}
