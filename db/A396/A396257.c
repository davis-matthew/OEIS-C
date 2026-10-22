/*
 * A396257  Number of n X 3 arrays over {0,1,2} such that orthogonally
 *          adjacent entries are distinct.
 * https://oeis.org/A396257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=12, a(2)=54; a(n)=5*a(n-1)-2*a(n-2) for n>=3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three values directly */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)12;
    if (n == (idx_t)2) return (seq_t)54;

    /* Iteratively compute the recurrence */
    seq_t a_prev2 = (seq_t)12;   /* a(1) */
    seq_t a_prev1 = (seq_t)54;   /* a(2) */
    seq_t a_curr;

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        a_curr = (seq_t)5 * a_prev1 - (seq_t)2 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
