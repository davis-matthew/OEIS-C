/*
 * A372025  Maximum second Zagreb index of maximal 3-degenerate graphs with n
 *          vertices.
 * https://oeis.org/A372025
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(n-1)*(2n-5) (valid for n >= 3)
 * Range:     u8 n <= 6, u16 n <= 75, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute using the closed form 6*(n-1)*(2n-5).  No overflow for
       typical n values; intermediate results are in seq_t to match the
       required arbitrary‑precision type. */
    seq_t base = (seq_t)(n - 1);
    seq_t factor = (seq_t)(2 * n - 5);
    return 6 * base * factor;
}
