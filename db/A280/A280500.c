/*
 * A280500  Square array for division in ring GF(2)[X]: A(r,c) = r/c, or 0 if
 *          c is not a divisor of r, where the binary expansion of each number
 *          defines the corresponding (0,1)-polynomial.
 * https://oeis.org/A280500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(row,col) where (row,col) is the n‑th entry in the *
 *            antidiagonal enumeration of the GF(2)[X] division table. * For
 *            integers r,c the value is the quotient r/c computed in GF(2)[X]
 *            * (polynomial division), or 0 when c does not divide r
 * Range:     u8 n <= 9778, u16 n <= 9778, u32 n <= 9778, u64 n <= 9778,
 *            u128 n <= 9778, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"

/* polynomial division in GF(2)[X] of R by C.
 * Returns the quotient if divisible, otherwise 0. */
static seq_t poly_div_gf2(seq_t R, seq_t C)
{
    seq_t quotient = 0;
    seq_t remainder = R;

    /* C is always >=1 in this problem; guard just in case. */
    if (C == 0)
        return 0;

    while (remainder != 0)
    {
        idx_t deg_r = bitlen_seq_t(remainder) - 1;
        idx_t deg_c = bitlen_seq_t(C) - 1;

        if (deg_r < deg_c)
            break;

        idx_t shift = deg_r - deg_c;
        remainder ^= (C << shift);
        quotient ^= ((seq_t)1 << shift);
    }
    return (remainder == 0) ? quotient : 0;
}

seq_t a(idx_t n)
{
    /* Find the diagonal k such that k*(k+1)/2 >= n.
     * cum is the cumulative count up to diagonal k. */
    idx_t k = 1;
    idx_t cum = 1;
    while (cum < n)
    {
        ++k;
        cum += k;
    }

    idx_t prev = cum - k;             /* count up to diagonal k-1      */
    idx_t offset = n - prev - 1;      /* zero‑based position on diag  */
    idx_t r = offset + 1;
    idx_t c = k - offset;

    return poly_div_gf2((seq_t)r, (seq_t)c);
}
