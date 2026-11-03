/*
 * A328748  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          T(n,k) is Sum_{i=0..n} (-2)^(n-i)*binomial(n,i)*Sum_{j=0..i}
 *          binomial(i,j)^k.
 * https://oeis.org/A328748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = Sum_{i=0..row} (-2)^(row-i) * binomial(row,i) * *
 *            Sum_{j=0..i} binomial(i,j)^col * where (row,col) are the
 *            antidiagonal coordinates of n
 * Range:     i8 n <= 38, i16 n <= 58, i32 n <= 97, i64 n <= 179,
 *            i128 n <= 336, big any n
 * Validated: standard, pass, 82 terms through n = 4707 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    /* Find diagonal d such that d(d+1)/2 <= n < (d+1)(d+2)/2. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;

    /* Position t on diagonal d. */
    idx_t t = n - d * (d + 1) / 2;

    /* Antidiagonal coordinates (row,k). */
    idx_t row = t;          /* n value in T(n,k) */
    idx_t col = d - t;      /* k value in T(n,k) */

    seq_t result = 0;

    for (idx_t i = 0; i <= row; ++i) {
        /* Inner sum Σ_{j=0..i} binomial(i,j)^col. */
        seq_t inner = 0;
        for (idx_t j = 0; j <= i; ++j) {
            seq_t b = binomial(i, j);
            seq_t p = ipow_i32(b, (int)col);
            inner += p;
        }

        seq_t coeff = binomial(row, i);

        /* Compute (-2)^(row-i) as ((-1)^(row-i))*2^(row-i). */
        seq_t two_pow = ipow_i32((seq_t)2, (int)(row - i));
        if ((row - i) & 1)
            two_pow = -two_pow;  /* multiply by -1 if exponent is odd */

        seq_t term = coeff * inner * two_pow;
        result += term;
    }

    return result;
}
