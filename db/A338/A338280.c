/*
 * A338280  Triangle T read by rows: T(n, k) = k*n^(n-k-1) with 0 < k < n.
 * https://oeis.org/A338280
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = k * r^(r-k-1) where r and k are the row number and the
 *            column index of the triangle T(r,k)=k*r^(r-k-1) read by rows,
 *            offset 2 (so a(2)=T(2,1))
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /*  The sequence is defined for n >= 2.  For n < 2 we return 0. */
    if (n < 2) return 0;

    /*  Convert the input index to a zero‑based offset within the
        triangle (T(2,1) corresponds to offset 0).  */
    idx_t offset = n - 2;

    /*  Find the triangle row r that contains this offset.
        The total number of entries up to row r is (r-1)*r/2.  */
    idx_t r = 2;          /* first row of the triangle */
    idx_t cum = 0;        /* cumulative entries before the current row */

    while (offset >= cum + (r - 1)) {
        cum += r - 1;
        ++r;
    }

    /*  The column index k within this row.  Offsets start at 0, but
        the triangle indices start at 1.  */
    idx_t k = offset - cum + 1;

    /*  Compute the power r^(r-k-1) using the provided big_pow
        helper (handles arbitrary precision).  */
    seq_t base = (seq_t)r;
    seq_t exp  = (seq_t)(r - k - 1);
    seq_t pow_val = big_pow(base, exp);

    /*  Return the triangle value k * r^(r-k-1).  */
    return (seq_t)k * pow_val;
}
