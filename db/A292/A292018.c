/*
 * A292018  Wiener index for the n-Andrásfai graph.
 * https://oeis.org/A292018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n - 4)*(3*n - 1)/2
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = n;
    seq_t t1 = 5 * sn - 4;
    seq_t t2 = 3 * sn - 1;
    return (t1 * t2) / 2;
}
