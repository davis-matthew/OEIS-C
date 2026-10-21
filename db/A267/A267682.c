/*
 * A267682  a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4) for n > 3, with initial terms
 *          1, 1, 4, 8.
 * https://oeis.org/A267682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1) + floor(n/2) + 1
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = nn * (nn - 1) + (nn >> 1) + 1;
    return term;
}
