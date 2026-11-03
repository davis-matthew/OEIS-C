/*
 * A338104  Number of spanning trees in the join of the disjoint union of two
 *          complete graphs each on n vertices with the empty graph on n+1
 *          vertices.
 * https://oeis.org/A338104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + 1)*(2*n)^n*(2*n + 1)^(2*(n - 1))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t N            = (seq_t)n;
    seq_t two_n        = N * 2;                     /* 2*n */
    seq_t p1           = ipow(two_n, N);          /* (2*n)^n */

    seq_t two_n_plus1  = two_n + 1;                  /* 2*n + 1 */
    seq_t exp2         = (N - 1) * 2;                /* 2*(n-1) */
    seq_t p2           = ipow(two_n_plus1, exp2);/* (2*n+1)^(2*(n-1)) */

    seq_t res = (N + 1);                            /* (n+1) */
    res *= p1;
    res *= p2;

    return res;
}
