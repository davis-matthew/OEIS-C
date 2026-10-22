/*
 * A375891  a(n) = (32)^n*cos (nB - nC), where A, B, C are, respectively, the
 *          angles opposite sides BC, CA, AB in a triangle ABC having
 *          sidelengths |BC| = 2, |CA| = 3, |AB| = 4; ABC is the smallest
 *          integer-sided scalene triangle.
 * https://oeis.org/A375891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 34*a(n-1) - 1024*a(n-2), with a(0) = 1, a(1) = 17
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)17;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)17;  /* a(1) */
    seq_t curr = 0;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)34 * prev1 - (seq_t)1024 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
