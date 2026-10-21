/*
 * A298125  The hex numbers (A003215) together with 3.
 * https://oeis.org/A298125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 - 9*n + 3*n^2 (for n > 2), a(1)=1, a(2)=3
 * Range:     u8 n <= 10, u16 n <= 149, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    idx_t nn = n;
    return (seq_t)(3ULL * nn * nn - 9ULL * nn + 7ULL);
}
