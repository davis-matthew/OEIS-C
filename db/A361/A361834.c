/*
 * A361834  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=0..n} (-1)^(n-j) *
 *          binomial(2*j,j) * binomial(k*j,n-j).
 * https://oeis.org/A361834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(i,j) where T(n,k) = Σ_{j=0..n} (-1)^(n-j) *
 *            binomial(2*j,j) * binomial(k*j,n-j), read by antidiagonals
 *            downwards; a(0) = 1, a(1) = 1,
 * Range:     i8 n <= 13, i16 n <= 43, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/binom.h"

/* triangular number: d*(d+1)/2, result fits in idx_t for d up to sqrt(2*n) */
static inline idx_t tri(idx_t d)
{
    __int128 prod = (__int128)d * (d + 1);
    prod >>= 1;                 /* divide by 2 */
    return (idx_t)prod;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* 1. find diagonal d such that tri(d) <= n < tri(d+1) */
    idx_t low = 0, high = 1;
    while (tri(high) <= n)         /* grow upper bound */
        high <<= 1;

    while (low + 1 < high) {       /* binary search for largest d with tri(d)<=n */
        idx_t mid = (low + high) >> 1;
        if (tri(mid) <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t d = low;

    /* 2. compute coordinates (i,j) on diagonal d */
    idx_t i = n - tri(d);          /* row index (0-based) along the diagonal */
    idx_t j = d - i;               /* column index */

    /* 3. compute T(i,j) by definition */
    seq_t sum = 0;
    for (idx_t t = 0; t <= i; ++t) {
        seq_t b1 = binom(2 * t, t);               /* binomial(2*t, t) */
        idx_t jt = j * t;                         /* j*t */
        seq_t b2 = binom(jt, i - t);              /* binomial(j*t, i-t) */

        /* product and sign */
        seq_t term = b1 * b2;
        if ((i - t) & 1U)                      /* odd sign */
            sum -= term;
        else
            sum += term;
    }
    return sum;
}
