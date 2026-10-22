/*
 * A374659  a(n) = (1/5) A374658(n).
 * https://oeis.org/A374659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1, a(n)=a(n-1)*r(n) where r(1)=4, r(2)=6, r(3)=7
 *            and for n>=4 r(n)=2*r(n-1)-r(n-3). This recurrence guarantees
 *            that a(n+1)/a(n) is an integer for all n>=0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    seq_t r_prev3 = 0; /* r(n-3) for current n   */
    seq_t r_prev2 = 0; /* r(n-2) for current n   */
    seq_t r_prev1 = 0; /* r(n-1) for current n   */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t r;
        if (i == 1)
            r = (seq_t)4;
        else if (i == 2)
            r = (seq_t)6;
        else if (i == 3)
            r = (seq_t)7;
        else
            r = 2 * r_prev1 - r_prev3;   /* r(n) = 2*r(n-1) - r(n-3) */

        result *= r;

        /* shift the stored r values for the next iteration */
        r_prev3 = r_prev2;
        r_prev2 = r_prev1;
        r_prev1 = r;
    }

    return result;
}
