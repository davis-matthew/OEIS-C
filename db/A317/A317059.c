/*
 * A317059  a(n) is the number of time-dependent assembly trees satisfying the
 *          edge gluing rule for a complete graph on n vertices.
 * https://oeis.org/A317059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{j=1..floor(n/2)} (n!/(2^j j! (n-2j)!))*a(n-j),
 *            a(1)=a(2)=1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 22,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A317059.  Uses memoised recurrence with
   dynamic programming.  Works for arbitrary-precision builds
   as well as for fixed-width integer builds. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *A = NULL;                   /* memoisation array */
    static size_t A_cap = 0;                  /* allocated capacity */
    static idx_t A_max_computed = 0;          /* largest n with A[n] computed */

    if (n == 0)                /* not used, but harmless */
        return (seq_t)0;

    /* ensure we have room for indices up to n */
    ensure_cache(n, &A, &A_cap);

    /* initialise base cases once */
    if (A_max_computed == 0) {
        A[1] = (seq_t)1;
        if (A_cap > 1)        /* A_cap may be 1 when n==1 */
            A[2] = (seq_t)1;
        A_max_computed = 2;
    }

    if (n <= A_max_computed)   /* already computed value */
        return A[n];

    /* extend memo table up to n */
    for (idx_t m = A_max_computed + 1; m <= n; ++m) {
        /* base cases handled earlier; just skip two */
        if (m == 1 || m == 2) {
            A[m] = (seq_t)1;
            continue;
        }
        seq_t sum = 0;
        idx_t max_j = m / 2;                /* floor(m/2) */
        seq_t F = (seq_t)m * (seq_t)(m - 1) / 2;  /* j = 1 term of the multinomial */
        for (idx_t j = 1; j <= max_j; ++j) {
            seq_t term = F * A[m - j];
            sum += term;
            if (j == max_j) break;          /* last j processed */
            /* compute F for next j using multiplicative ratio */
            seq_t num   = (seq_t)(m - 2 * j) * (seq_t)(m - 2 * j - 1);
            seq_t denom = (seq_t)2 * (seq_t)(j + 1);
            F = (num * F) / denom;          /* exact division */
        }
        A[m] = sum;
    }

    A_max_computed = n;
    return A[n];
}
