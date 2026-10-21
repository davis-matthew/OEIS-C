/*
 * A281813  a(0) = 3, a(n) = 8*n + 4 for n > 0.
 * https://oeis.org/A281813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 8*n + 4 for n>0, a(0) = 3
 * Range:     u8 n <= 31, u16 n <= 8191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;
    return (seq_t)8 * (seq_t)n + (seq_t)4;
}
