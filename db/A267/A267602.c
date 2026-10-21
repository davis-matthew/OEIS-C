/*
 * A267602  Number of unlabeled, connected graphs on n vertices that are prime
 *          and have no induced subgraph isomorphic to a bull, a P5 or a
 *          P5-bar.
 * https://oeis.org/A267602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n < 4 a(n) = 1 if n = 4 or n = 5 a(n) = 2 if n is
 *            even and n ≥ 6 a(n) = 0 if n is odd and n ≥ 7
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)          return 0;
    if (n == 4 || n == 5) return 1;
    if ((n & 1ULL) == 0) return 2;
    return 0;
}
