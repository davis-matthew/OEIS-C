/*
 * A354902  a(n) = 2*n^2 - 6*n + 11 for n > 1 with a(0)=1 and a(1)=3.
 * https://oeis.org/A354902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 - 6*n + 11 for n ≥ 2, a(0)=1, a(1)=3
 * Range:     u8 n <= 12, u16 n <= 182, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    seq_t ns = (seq_t)n;
    return (seq_t)2 * ns * ns - (seq_t)6 * ns + (seq_t)11;
}
