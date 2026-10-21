/*
 * A278910  Triangle of order m: C(n,k) = k*(n-k+1)^(k+m)+n-k, 0 <= k <= n, m
 *          = 0, read by rows.
 * https://oeis.org/A278910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = k * (row - k + 1)^k + (row - k), where (row, k) is
 *            obtained by flattening the triangle of order 0 (C(row, k)) row
 *            by row, starting from offset 0
 * Range:     u8 n <= 24, u16 n <= 60, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the row that contains the n‑th element. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Compute the offset of the first element of this row. */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;          /* column in the row */

    seq_t base = (seq_t)(r - k + 1);
    seq_t pow_val = big_pow(base, (seq_t)k);
    seq_t result = ((seq_t)k) * pow_val + (seq_t)(r - k);

    return result;
}
