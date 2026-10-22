/*
 * A369801  Maximum number of segments between n points on a circle so that
 *          they can be colored in 2 colors so that each of them intersects
 *          (at an internal point) at most one other segment of the same
 *          color.
 * https://oeis.org/A369801
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)/2 for n <= 6; a(2*k+1) = 8*k-5 for k >= 3; a(2*k)
 *            = 8*k-8 for k >= 4 (n >= 7)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The OEIS offset is 2, so for n < 2 the value is irrelevant. */
    if (n < 2)
        return (seq_t)0;

    /* For n <= 6 the formula is triangular numbers. */
    if (n <= 6)
        return (seq_t)n * ((seq_t)n - (seq_t)1) / (seq_t)2;

    /* For larger n use the piecewise linear formulas. */
    if ((n & (idx_t)1) == (idx_t)1)  /* n is odd: n = 2*k+1 */
    {
        idx_t k = (n - (idx_t)1) / (idx_t)2;
        return (seq_t)8 * (seq_t)k - (seq_t)5;
    }
    else                            /* n is even: n = 2*k */
    {
        idx_t k = n / (idx_t)2;
        return (seq_t)8 * (seq_t)k - (seq_t)8;
    }
}
