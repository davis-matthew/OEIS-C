/*
 * A260637  Sums of seven consecutive squares: a(n) = n^2 + (n+1)^2 + (n+2)^2
 *          + (n+3)^2 + (n+4)^2 + (n+5)^2 + (n+6)^2.
 * https://oeis.org/A260637
 *
 * Offset:    -3    (a(-3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n^2 + 42*n + 91
 * Range:     u8 n <= 2, u16 n <= 93, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t nn = n;
    idx_t val = 7 * nn * nn + 42 * nn + 91;
    return (seq_t)val;
}
