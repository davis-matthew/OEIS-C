/*
 * A304381  a(n) = 54*n^2 - 26*n + 4 (n>=1).
 * https://oeis.org/A304381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 54*n^2 - 26*n + 4
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t result = (seq_t)54 * x * x - (seq_t)26 * x + 4;
    return result;
}
