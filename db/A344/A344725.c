/*
 * A344725  Square array T(n,k), n >= 1, k >= 1, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=1..n} floor(n/j)^k.
 * https://oeis.org/A344725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{j=1..row} ⌊row/j⌋^col, where (row,col) are obtained
 *            from n by reading the square array T(r,c) by downward
 *            antidiagonals
 * Range:     u8 n <= 24, u16 n <= 58, u32 n <= 157, u64 n <= 412,
 *            u128 n <= 1092, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"
 
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* not part of the sequence, but safe guard */
 
    /* Find diagonal number s such that s*(s-1)/2 ≥ n */
    idx_t s = 2;
    while (s * (s - 1) / 2 < n)
        ++s;
 
    /* Number of elements before this diagonal */
    idx_t prev = (s - 1) * (s - 2) / 2;
 
    /* Position within the diagonal (0‑based) */
    idx_t offset = n - prev - 1;
 
    /* Row (i) starts at 1; column (j) = s - i */
    idx_t row = offset + 1;
    idx_t col = s - row;
 
    /* Compute T(row,col) = Σ_{j=1..row} floor(row/j)^col */
    seq_t result = 0;
    for (idx_t j = 1; j <= row; ++j) {
        idx_t base = row / j;               /* floor division */
        seq_t term = pow_uint((uint64_t)base, col);
        result += term;
    }
    return result;
}
