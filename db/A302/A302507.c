/*
 * A302507  a(n) = 4*(3^n-1).
 * https://oeis.org/A302507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(3^n - 1)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        result *= (seq_t)3;
    }
    result -= (seq_t)1;
    result *= (seq_t)4;
    return result;
}
