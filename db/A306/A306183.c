/*
 * A306183  The coefficients of x in the reduction of x^2 -> x + 1 for the
 *          polynomial p(n,x) = Product_{k=1..n} (x+k).
 * https://oeis.org/A306183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*a(n-1) - (n^2 - n - 1)*a(n-2); a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t a_prev2 = (seq_t)0; /* a(0) */
    seq_t a_prev1 = (seq_t)1; /* a(1) */
    seq_t an = 0;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t term1 = (seq_t)2 * ii * a_prev1;
        seq_t term2 = (ii * ii - ii - (seq_t)1) * a_prev2;
        an = term1 - term2;
        a_prev2 = a_prev1;
        a_prev1 = an;
    }
    return an;
}
