/*
 * A398709  a(n) = floor(((5 + sqrt(17))/2)^n).
 * https://oeis.org/A398709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(((5 + sqrt(17))/2)^n) = A159289(n-1)-1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)                     /* for indices below the offset */
        return (seq_t)0;           /* undefined, but return 0 */

    /* s_0 = 2,  s_1 = 5  (s_n = alpha^n + beta^n) */
    seq_t s_prev2 = (seq_t)2;      /* s_{i-2} */
    seq_t s_prev1 = (seq_t)5;      /* s_{i-1} */

    if (n == 1)                    /* a(1) = s_1 - 1 = 4 */
        return s_prev1 - (seq_t)1;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = s_prev1 * (seq_t)5;   /* 5 * s_{i-1}          */
        cur -= s_prev2 * (seq_t)2;        /* - 2 * s_{i-2}        */
        s_prev2 = s_prev1;               /* shift for next step  */
        s_prev1 = cur;
    }
    return s_prev1 - (seq_t)1;           /* a(n) = s_n - 1       */
}
