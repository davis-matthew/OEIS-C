/*
 * A369182  Numbers k such that d(k) is greater than d(k-2), d(k-1), d(k+1)
 *          and d(k+2) where d(k) is the number of divisors of k.
 * https://oeis.org/A369182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that d(k) > d(k-2), d(k-1), d(k+1), d(k+2)
   Computation: linear scan of k, testing the divisor‑count condition.
   Memoization keeps all values found so far so later calls need only
   extend the search. */

#include <stdlib.h>
#include "../utils/divisor_count_u64.h"   /* for divisor_count_u64(uint64_t) */

seq_t a(idx_t n)
{
    /* static data to cache previously computed terms */
    static seq_t *terms      = NULL;   /* array of terms found so far   */
    static size_t cap        = 0;      /* capacity of that array         */
    static size_t len        = 0;      /* number of terms stored         */
    static uint64_t checked  = 2;      /* largest k that has been examined */

    /* return a already computed term */
    if (n <= len && n > 0)
        return terms[n-1];

    /* grow capacity helper */
    if (terms == NULL) {
        cap = 32;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* extend the search until we have at least n terms */
    for (uint64_t k = checked + 1; len < (size_t)n; ++k) {
        /* divisor counts of k-2 .. k+2 */
        uint64_t d_km2 = divisor_count_u64(k-2);
        uint64_t d_km1 = divisor_count_u64(k-1);
        uint64_t d_k   = divisor_count_u64(k);
        uint64_t d_kp1 = divisor_count_u64(k+1);
        uint64_t d_kp2 = divisor_count_u64(k+2);

        if (d_k > d_km2 && d_k > d_km1 && d_k > d_kp1 && d_k > d_kp2) {
            if (len == cap) {
                cap *= 2;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            }
            terms[len++] = (seq_t)k;
        }
    }

    checked = (uint64_t)terms[len-1];    /* largest checked k now */
    return terms[n-1];
}
