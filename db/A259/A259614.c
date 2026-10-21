/*
 * A259614  Numbers congruent to {17,29} mod 36.
 * https://oeis.org/A259614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36 * floor((n-1)/2) + (17 if n is odd, 29 if n is even)
 * Range:     u8 n <= 14, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = ((seq_t)(n - 1)) / 2;
    seq_t add = (n & 1) ? 17 : 29;
    return 36 * k + add;
}
