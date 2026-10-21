/*
 * A249015  A binomial convolution.
 * https://oeis.org/A249015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*n*a(n-2) + (n-1)*n*a(n-3); Initial values:
 *            a(0) = 1, a(1) = 1, a(2) = 5
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)5;

    /* Iterate the recurrence, keeping only the last three values. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)5;   /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t term1 = a2;
        seq_t term2 = (seq_t)2 * (seq_t)i * a1;
        seq_t term3 = (seq_t)(i - 1) * (seq_t)i * a0;
        seq_t next  = term1 + term2 + term3;

        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
