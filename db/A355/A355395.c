/*
 * A355395  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=0..n} k^(j*(n-j)) *
 *          binomial(n,j).
 * https://oeis.org/A355395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,col) where T(n,k)=∑_{j=0..n} k^(j*(n-j))*C(n,j) and
 *            (row,col) are the indices of the entry on the n-th antidiagonal
 *            (row increasing downwards)
 * Range:     u8 n <= 31, u16 n <= 49, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

static inline idx_t int_sqrt_u128(__uint128_t val)
{
    /* initial approximation using double */
    double approx = sqrt((double)val);
    idx_t r = (idx_t)approx;
    /* correct towards the nearest integer root */
    while ((__uint128_t)(r + 1) * (r + 1) <= val) ++r;
    while ((__uint128_t)r * r > val) --r;
    return r;
}

seq_t a(idx_t n)
{
    /* find the diagonal d such that the first d+1 diagonals contain n+1 terms */
    __uint128_t tmp = (__uint128_t)8 * n + 1;
    idx_t w = int_sqrt_u128(tmp);          /* floor(sqrt(8n+1))   */
    idx_t m = (w + 1) / 2;                 /* number of terms to exceed n */
    idx_t d = m - 1;                       /* current diagonal index */

    /* offset of n within diagonal d (0 ≤ offset ≤ d) */
    idx_t offset = n - d * (d + 1) / 2;
    idx_t i = offset;                      /* row index n in T(n,k) */
    idx_t col = d - offset;                /* column index k in T(n,k) */

    seq_t sum = 0;
    for (idx_t s = 0; s <= i; ++s)
    {
        idx_t exp = s * (i - s);           /* j*(n-j) in the formula */
        seq_t term = big_pow((seq_t)col, (seq_t)exp) * binomial(i, s);
        sum += term;                       /* seq_t addition is exact */
    }
    return sum;
}
