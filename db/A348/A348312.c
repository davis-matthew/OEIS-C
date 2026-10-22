/*
 * A348312  a(n) = n! * Sum_{k=0..n-1} 3^k / k!.
 * https://oeis.org/A348312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = n * (a(n-1) + 3^(n-1))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is defined as 0. */
    if (n == 0ULL)
        return (seq_t)0;

    seq_t a_val = (seq_t)0;   /* a(0) */
    seq_t pow3  = (seq_t)1;   /* 3^0 */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* recurrence: a(i) = i * (a(i-1) + 3^(i-1)) */
        a_val = (seq_t)i * (a_val + pow3);
        /* advance power for next iteration: pow3 = 3^i */
        pow3 *= (seq_t)3;
    }

    return a_val;
}
