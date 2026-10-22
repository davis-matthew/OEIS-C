/*
 * A305155  a(n) = 28*2^n - 15.
 * https://oeis.org/A305155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*2^n - 15
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow2n = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2n *= 2;
    return pow2n * 28 - 15;
}
