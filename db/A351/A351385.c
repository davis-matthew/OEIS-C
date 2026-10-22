/*
 * A351385  Triangle read by rows: T(n,k) = Sum_{j=k..n} binomial(n + j,
 *          n)*binomial(n, j)/(j + 1).
 * https://oeis.org/A351385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 135, u64 n <= 434,
 *            u128 n <= 1325, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = T(i, j) where
 *  i  = floor((sqrt(8*n+1)-1)/2)   (row of the triangle)
 *  j  = n - i*(i+1)/2              (column in that row)
 *
 *  T(i, j) = Σ_{k=j..i} binom(i+k, i) * binom(i, k) /(k+1)
 *
 *  The sequence is the triangle read by rows.
 *
 *  The implementation uses the supplied binomial function.
 *  It runs in O(i) ≈ O(√n) time per call.
 */

#include <stddef.h>
#include "../utils/binom.h"          /* seq_t binom(idx_t a, idx_t b) */

static inline idx_t row_of(idx_t n, idx_t *col)
{
    idx_t i = 0;
    /* Find largest i such that i*(i+1)/2 <= n */
    while ((i + 1) * (i + 2) / 2 <= n)
        ++i;
    *col = n - i * (i + 1) / 2;
    return i;
}

static seq_t triangle_T(idx_t n, idx_t k)
{
    seq_t sum = 0;
    for (idx_t j = k; j <= n; ++j)
    {
        seq_t term = binom(n + j, n);
        term = term * binom(n, j);
        term = term / (j + 1);
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    idx_t k, row = row_of(n, &k);
    return triangle_T(row, k);
}
