/*
 * A360099  To get A(n,k), replace 0's in the binary expansion of n with (-1)
 *          and interpret the result in base k; square array A(n,k), n>=0,
 *          k>=0, read by antidiagonals.
 * https://oeis.org/A360099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, look, base
 * Formula:   a(n) = A(i, j) where i = n - d(d+1)/2, j = d - i, and A(0, j) =
 *            0, A(i, j) = j*A(i/2, j) + 2*(i%2) - 1 for i > 0. (i,k) run over
 *            non‑negative integers, d is the diagonal number: d = max { d |
 *            d(d+1)/2 <= n }
 * Range:     i8 n <= 112, i16 n <= 480, i32 n <= 5214, i64 n <= 9221,
 *            i128 n <= 9221, big any n
 * Helpers:   A_val, diagonal_from_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/A_val.h"
#include "../utils/diagonal_from_n.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t d = diagonal_from_n(n);
    idx_t offset = n - d * (d + 1) / 2;   /* offset within the diagonal */
    idx_t i = offset;
    idx_t j = d - offset;
    return A_val(i, (seq_t)j);
}
