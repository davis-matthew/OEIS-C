/*
 * A253707  Numbers M(n) which are the number of terms in the sums of
 *          consecutive cubed integers equaling a squared integer, b^3 +
 *          (b+1)^3 + ... + (b+M-1)^3 = c^2, for a first term b(n) being an
 *          odd squared integer (A016754).
 * https://oeis.org/A253707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(8*n*(n+1)+1)
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 812, u64 n <= 9407,
 *            u128 n <= 9407, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n >= 1 the formula applies; for n == 0 the value is irrelevant. */
    if (n == 0) return (seq_t)0;
    seq_t tn  = (seq_t)n;
    seq_t tn1 = tn + (seq_t)1;
    seq_t term = (seq_t)8 * tn * tn1 + (seq_t)1;
    return tn * term;
}
