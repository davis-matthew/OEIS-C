/*
 * A330002  Beatty sequence for x, where 1/x + 1/(x+1)^2 = 1.
 * https://oeis.org/A330002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((n-1)/4)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is at least 1, so (n-1)/4 is integer division */
    return n + (n - 1) / 4;
}
