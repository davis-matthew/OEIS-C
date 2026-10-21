/*
 * A289134  a(n) = 21*n^2 - 33*n + 13.
 * https://oeis.org/A289134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*n^2 - 33*n + 13
 * Range:     u8 n <= 4, u16 n <= 56, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;
    seq_t tn = (seq_t)n;
    return 21 * tn * tn - 33 * tn + 13;
}
