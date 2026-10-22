/*
 * A395827  a(n) is the number of possible outcomes having a run of 2 or more
 *          sixes when a die is rolled n times.
 * https://oeis.org/A395827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - A180033(n) where A180033(n) counts length‑n
 *            sequences of dice rolls with no two consecutive sixes. The
 *            recurrence for A180033 is f(0)=1, f(1)=6, f(n)=5*f(n-1)+5*f(n-2)
 *            (n≥2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 6^n */
    seq_t pow6 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow6 = pow6 * (seq_t)6;

    /* A180033(n) */
    if (n == 0)
        return pow6 - (seq_t)1;          /* a(0)=0 */
    if (n == 1)
        return pow6 - (seq_t)6;          /* a(1)=0 */

    seq_t f_prev2 = (seq_t)1;           /* f(0) */
    seq_t f_prev1 = (seq_t)6;           /* f(1) */
    seq_t f_cur   = (seq_t)0;
    for (idx_t i = 2; i <= n; ++i)
    {
        f_cur   = f_prev1 * (seq_t)5 + f_prev2 * (seq_t)5;
        f_prev2 = f_prev1;
        f_prev1 = f_cur;
    }

    return pow6 - f_cur;                /* a(n) */
}
