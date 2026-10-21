/*
 * A241957  Rectangular array A read by upward antidiagonals in which the
 *          entry in row n and column k is defined by A(n,k) = 2^n*(2*k - 1) -
 *          1, n,k >= 1.
 * https://oeis.org/A241957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2^row * (2*col-1) - 1, where row and col come from the
 *            upward anti‑diagonal enumeration of the array A(n,k) =
 *            2^n*(2k-1)-1
 * Range:     u8 n <= 29, u16 n <= 121, u32 n <= 497, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the anti‑diagonal sum s such that (s-1)*s/2 >= n. */
    idx_t s = 2;
    while (((s - 1) * s) / 2 < n)
        ++s;

    idx_t pre    = ((s - 2) * (s - 1)) / 2;      /* terms before this diagonal */
    idx_t offset = n - pre - 1;                  /* 0‑based position within diagonal */

    idx_t row = (s - 1) - offset;                /* n in the formula */
    idx_t col = offset + 1;                      /* k in the formula */

    seq_t val = pow2(row);                       /* 2^row  */
    val = val * (((seq_t)2 * col) - 1) - 1;
    return val;
}
