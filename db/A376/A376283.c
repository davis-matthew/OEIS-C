/*
 * A376283  a(n) = (40)^n * cos(nB), where B is the angle opposite side CA in
 *          a triangle ABC having sidelengths |BC|=3, |CA|=4, |AB|=5; ABC is
 *          the smallest integer-sided right triangle.
 * https://oeis.org/A376283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 48 a(n-1) - 1600 a(n-2), a(0) = 1, a(1) = 24
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 12, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)24;
    }

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)24;  /* a(1) */
    seq_t current;

    for (idx_t i = 2; i <= n; ++i) {
        current = (seq_t)48 * prev1 - (seq_t)1600 * prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return current;
}
