/*
 * A305460  a(0) = 1, a(1) = 3, a(n) = 3*n*a(n-1) + 2*a(n-2).
 * https://oeis.org/A305460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 3, a(n) = 3*n*a(n-1) + 2*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)3;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)3;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2U; i <= n; ++i)
    {
        /* cur = 3*i*prev1 + 2*prev2 */
        cur = ((seq_t)3 * (seq_t)i * prev1) + ((seq_t)2 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
