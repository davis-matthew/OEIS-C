/*
 * A375884  a(n) = 2^(2n-1) sin(nB)/(3 sqrt(15)), where B is the angle
 *          opposite side CA in a triangle ABC having sidelengths |BC|=2, |CA|
 *          = 3, |AB| = 4; ABC is the smallest integer-sided scalene triangle.
 * https://oeis.org/A375884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 11*a(n-1) - 64*a(n-2), a(0) = 0, a(1) = 1
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 11, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)1;
    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;
    seq_t an;
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        an = (seq_t)11 * a1 - (seq_t)64 * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
