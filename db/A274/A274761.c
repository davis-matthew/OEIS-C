/*
 * A274761  a(n) = a(n-1) + a(n-2) + a(n-3), with a(0) = 3, a(1) = 2, a(2) =
 *          5.
 * https://oeis.org/A274761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3); a(0) = 3, a(1) = 2, a(2) = 5
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 72, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;

    /* Iteratively compute the recurrence */
    seq_t a0 = (seq_t)3;   /* a(n-3)   */
    seq_t a1 = (seq_t)2;   /* a(n-2)   */
    seq_t a2 = (seq_t)5;   /* a(n-1)   */
    seq_t res;

    for (idx_t i = 3; i <= n; ++i)
    {
        res = a2 + a1 + a0;  /* a(i)   */
        a0 = a1;             /* shift window */
        a1 = a2;
        a2 = res;
    }
    return a2;
}
