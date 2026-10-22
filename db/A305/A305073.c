/*
 * A305073  a(n) = 288*n^2 - 96*n (n>=1).
 * https://oeis.org/A305073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 288*n^2 - 96*n
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 3861, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return (seq_t)288 * x * x - (seq_t)96 * x;
}
