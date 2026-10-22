/*
 * A342129  Square array T(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of g.f. 1/(1 - k*x + k*x^2).
 * https://oeis.org/A342129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(0,k)=1, T(1,k)=k, T(n,k)=k*(T(n-1,k)-T(n-2,k)) for n>1. a(n)
 *            is the array entry read by antidiagonals of the square array
 *            T(n,k)
 * Range:     i8 n <= 47, i16 n <= 96, i32 n <= 219, i64 n <= 484,
 *            i128 n <= 1256, big any n
 * Helpers:   sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sub.h"

/* Multiply a seq_t by a small unsigned integer. Works for both fixed‑size
   and arbitrary‑precision seq_t types. */
static inline seq_t mul_small(seq_t a, unsigned int m)
{
    return a * m;
}

seq_t a(idx_t n)
{
    /* Find the diagonal d (0‑based) such that
       d*(d+1)/2 <= n < (d+1)*(d+2)/2. */
    idx_t d = 0;
    idx_t tri = 0;          /* triangular number d*(d+1)/2 */
    while (tri + (d + 1) <= n) {
        tri += d + 1;
        ++d;
    }

    /* Offset inside the diagonal: row index i. */
    idx_t i = n - tri;          /* 0 <= i <= d */
    idx_t k = d - i;            /* column index.  k is small (≤ n). */

    /* Compute T(i,k) by recurrence. */
    if (i == 0) return (seq_t)1;
    if (i == 1) return (seq_t)k;

    seq_t prev2 = (seq_t)1;          /* T(0,k) */
    seq_t prev1 = (seq_t)k;          /* T(1,k) */
    seq_t curr  = 0;

    for (idx_t t = 2; t <= i; ++t) {
        curr = mul_small(sub(prev1, prev2), (unsigned int)k);
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
