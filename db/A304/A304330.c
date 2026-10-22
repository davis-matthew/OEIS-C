/*
 * A304330  T(n, k) = Sum_{j=0..k} (-1)^j*binomial(2*k, j)*(k - j)^(2*n),
 *          triangle read by rows, n >= 0 and 0 <= k <= n.
 * https://oeis.org/A304330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 33, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   _floor_isqrt_u64, big_pow, binomial_2k_j
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = T(row, col) where
 *   row = floor((sqrt(8*n+1)-1)/2)
 *   col = n - row*(row+1)/2
 * and
 *   T(row, col) = Σ_{j=0..col} (-1)^j * binomial(2*col, j) * (col-j)^(2*row)
 * The sequence is read row‑wise from the triangle T.
 */

#include <stdint.h>            /* for uint64_t, if needed */
#include "../utils/_floor_isqrt_u64.h"   /* for _floor_isqrt_u64 */
#include "../utils/big_pow.h"              /* for big_pow */
#include "../utils/binomial_2k_j.h"

/* declarations of external functions used */
extern idx_t _floor_isqrt_u64(idx_t n);
extern seq_t big_pow(seq_t base, seq_t exp);

seq_t a(idx_t n)
{
    /* Determine row and column from index n */
    idx_t row = (_floor_isqrt_u64(8*n + 1) - 1) / 2;
    /* correct possible rounding errors */
    while ((row + 1) * (row + 2) / 2 <= n)
        ++row;
    while (row * (row + 1) / 2 > n)
        --row;
    idx_t start = row * (row + 1) / 2;
    idx_t col   = n - start;

    seq_t sum = 0;
    seq_t bin = 1;                     /* C(2*col, 0) */
    seq_t exp = (seq_t)(2 * row);      /* exponent 2*n */

    for (idx_t j = 0; j <= col; ++j) {
        seq_t base = (seq_t)(col - j);
        seq_t p = big_pow(base, exp);
        seq_t term = bin * p;
        if ((j & 1) == 0)
            sum += term;
        else
            sum -= term;
        if (j < col)                  /* update binomial for next j */
            bin = (bin * (seq_t)(2*col - j)) / (seq_t)(j + 1);
    }
    return sum;
}
