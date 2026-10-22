/*
 * A389371  Number of distinct branching structures with up to 3 branches per
 *          node after n generations.
 * https://oeis.org/A389371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n>=1, a(n)=∑_{k=0..3} binomial(a(n-1),k) */

seq_t a(idx_t n)
{
    /* a(0) is the empty structure */
    if (n == 0u)
        return (seq_t)1;

    seq_t cur = (seq_t)1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* binomial terms for k = 0..3 */
        seq_t binom0 = (seq_t)1;          /* C(cur,0) */
        seq_t binom1 = cur;               /* C(cur,1) */
        seq_t binom2 = (seq_t)0;          /* C(cur,2) */
        seq_t binom3 = (seq_t)0;          /* C(cur,3) */

        if (cur >= (seq_t)2)
            binom2 = cur * (cur - (seq_t)1) / (seq_t)2;

        if (cur >= (seq_t)3)
            binom3 = cur * (cur - (seq_t)1) * (cur - (seq_t)2) / (seq_t)6;

        cur = binom0 + binom1 + binom2 + binom3;
    }
    return cur;
}
