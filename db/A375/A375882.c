/*
 * A375882  a(n) = 4^n*cos(nC), where C is the angle opposite side AB in a
 *          triangle ABC having sidelengths |BC|=2, |CA| = 3, |AB| = 4; ABC is
 *          the smallest integer-sided scalene triangle.
 * https://oeis.org/A375882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) - 16*a(n-2), with a(0)=1, a(1)=–1
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 15, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)(-1);

    seq_t prev2 = (seq_t)1;          /* a(0) */
    seq_t prev1 = (seq_t)(-1);       /* a(1) */
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = -((seq_t)2 * prev1) - ((seq_t)16 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
