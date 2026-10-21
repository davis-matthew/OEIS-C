/*
 * A289711  Triangle T(n,p) read by rows: the order of the semigroup of
 *          orientation-preserving partial transformations of n elements with
 *          height p.
 * https://oeis.org/A289711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   let T(n,p) be the triangle read by rows. * - For p = 0: T(n,0) =
 *            1 * - For p = 1: T(n,1) = n * (2ⁿ – 1) * - For p ≥ 2: T(n,p) =
 *            C(n,p)² * p * 2ⁿ⁻ᵖ * The sequence a(n) is obtained by flattening
 *            the triangle * row‑by‑row (0‑based indexing)
 * Range:     u8 n <= 11, u16 n <= 37, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   binom, int_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_sqrt.h"
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find (row, col) such that a(n) = T(row,col). */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Row index: largest r with r*(r+1)/2 ≤ n */
    idx_t row = (int_sqrt(8 * n + 1) - 1) / 2;

    /* First index of this row */
    idx_t first_in_row = row * (row + 1) / 2;

    idx_t col = n - first_in_row;   /* 0‑based column */

    /* Compute T(row,col) according to the cases above */
    if (col == 0) {
        return (seq_t)1;
    }

    if (col == 1) {
        seq_t two_pow_row = pow2(row);          /* 2^row   */
        return (seq_t)row * (two_pow_row - (seq_t)1);
    }

    /* col ≥ 2 :  T = C(row,col)^2 * col * 2^(row-col) */
    seq_t comb = binom(row, col);                /* C(row,col)   */
    seq_t result = comb * comb * (seq_t)col * pow2(row - col);
    return result;
}
