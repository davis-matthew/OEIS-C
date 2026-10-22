/*
 * A377340  E.g.f. satisfies A(x) = ( 1 + (exp(x*A(x)) - 1)/A(x) )^3.
 * https://oeis.org/A377340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A377340
   Formula (recurrence form):
           a(n) = 3 * Σ_{k=1..⌊(3n+3)/4⌋} (3n-3k+2)! / (3n-4k+3)! * S(n,k)
   where S(n,k) are Stirling numbers of the second kind.
   a(0) = 1. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Special case */
    if (n == 0) return (seq_t)1;

    /* Max k needed */
    idx_t Kmax = (3 * n + 3) / 4;

    /* Allocate arrays for Stirling numbers (S(n,k)) */
    seq_t *prev = (seq_t*)malloc((size_t)(Kmax + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t*)malloc((size_t)(Kmax + 1) * sizeof(seq_t));
    if (!prev || !curr) {
        free(prev); free(curr);
        return (seq_t)0;            /* allocation failed */
    }

    /* Initialise S(0,0)=1, others 0 */
    for (idx_t i = 0; i <= Kmax; ++i) prev[i] = (seq_t)0;
    prev[0] = (seq_t)1;

    /* Build Stirling numbers up to n */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t limit = (i < Kmax) ? i : Kmax;   /* max k to compute for this row */

        curr[0] = (seq_t)0;                    /* S(i,0)=0 for i>0 */

        for (idx_t k = 1; k <= limit; ++k) {
            seq_t part = mul_small(prev[k], (unsigned int)k);   /* k * S(i-1,k) */
            curr[k] = part + prev[k-1];                       /* + S(i-1,k-1) */
        }
        /* Zero remaining entries beyond 'limit' */
        for (idx_t k = limit + 1; k <= Kmax; ++k) curr[k] = (seq_t)0;

        /* Swap rows */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Compute the sum Σ ... */
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= Kmax; ++k) {
        /* Compute factorial ratio R = (3n-3k+2)!/(3n-4k+3)! */
        seq_t R = (seq_t)1;
        if (k > 1) {
            idx_t start = 3 * n - 4 * k + 4;   /* first factor in product */
            for (idx_t i = 0; i < k - 1; ++i) {
                R = mul_small(R, (unsigned int)(start + i));
            }
        }
        seq_t term = prev[k] * R;               /* S(n,k)*R  (big*big) */
        sum += term;
    }

    seq_t result = mul_small(sum, (unsigned int)3);   /* multiply final sum by 3 */

    free(prev);
    free(curr);
    return result;
}
