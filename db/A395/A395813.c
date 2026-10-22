/*
 * A395813  Number of perfect matchings in the complete 4-partite graph
 *          K_{n,n,n,n}.
 * https://oeis.org/A395813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n^2 - 10*n + 3)*a(n-1) - 9*(n-1)^4*a(n-2); a(0) = 1,
 *            a(1) = 3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)3;   /* a(1) */
    seq_t curr  = prev1;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;

        /* term1 = 10*i^2 - 10*i + 3 */
        seq_t term1 = ((seq_t)10 * i_seq * i_seq)
                    - ((seq_t)10 * i_seq)
                    + (seq_t)3;

        /* t = i-1; t4 = t^4 */
        seq_t t  = (seq_t)(i - 1);
        seq_t t2 = t * t;
        seq_t t4 = t2 * t2;

        /* term2 = 9*t4 * a(n-2) */
        seq_t term2 = (seq_t)9 * t4 * prev2;

        /* recurrence: a(n) = term1*a(n-1) - term2 */
        curr = term1 * prev1 - term2;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
