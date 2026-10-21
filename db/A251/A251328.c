/*
 * A251328  Number of (n+1) X (1+1) 0..2 arrays with every 2 X 2 subblock
 *          summing to 2 4 or 6.
 * https://oeis.org/A251328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 13*a(n-2) - 12*a(n-3), a(1) = 39, a(2) = 171,
 *            a(3) = 753
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined below offset, safe value */
    if (n == 1)
        return (seq_t)39;
    if (n == 2)
        return (seq_t)171;
    if (n == 3)
        return (seq_t)753;

    /* Compute by recurrence. */
    seq_t a1 = (seq_t)39;   /* a(1)   */
    seq_t a2 = (seq_t)171;  /* a(2)   */
    seq_t a3 = (seq_t)753;  /* a(3)   */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)8 * a3 - (seq_t)13 * a2 - (seq_t)12 * a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
