/*
 * A337002  a(n) = n! * Sum_{k=0..n} k^4 / k!.
 * https://oeis.org/A337002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = n * (n^3 + a(n-1))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t seq_i = (seq_t)i;          /* cast index to seq_t */
        seq_t i3    = seq_i * seq_i * seq_i;   /* i^3 */
        result      = seq_i * (i3 + result);
    }
    return result;
}
