/*
 * A271389  a(n) = 2*a(n-1) + a(n-2) + n^2 for n > 1, with a(0) = 0, a(1) = 1.
 * https://oeis.org/A271389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + n^2 (a(0)=0, a(1)=1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t prev2 = 0;            /* a(n-2) */
    seq_t prev1 = 1;            /* a(n-1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t i2 = (seq_t)i * (seq_t)i;   /* i^2 */
        seq_t cur = prev1 * 2 + prev2 + i2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
