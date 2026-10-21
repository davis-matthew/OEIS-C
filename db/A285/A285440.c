/*
 * A285440  Consider the sums of the numbers < n that share the same greatest
 *          common divisor with n. Sequence lists numbers that have only one
 *          of those sums equal to n.
 * https://oeis.org/A285440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-5) for n > 5 initial values:
 *            a(1)=3, a(2)=4, a(3)=8, a(4)=9, a(5)=15
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for index 0 */

    /* Handle the first five terms directly */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)8;
    if (n == 4) return (seq_t)9;
    if (n == 5) return (seq_t)15;

    /* Iterate using the recurrence */
    seq_t prev5 = (seq_t)3;   /* a(n-5)   */
    seq_t prev4 = (seq_t)4;   /* a(n-4)   */
    seq_t prev3 = (seq_t)8;   /* a(n-3)   */
    seq_t prev2 = (seq_t)9;   /* a(n-2)   */
    seq_t prev1 = (seq_t)15;  /* a(n-1)   */
    seq_t cur;

    for (idx_t i = 6; i <= n; ++i) {
        cur = prev1 + prev4 - prev5;
        /* Shift the window for next iteration */
        prev5 = prev4;
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
