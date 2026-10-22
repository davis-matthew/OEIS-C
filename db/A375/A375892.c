/*
 * A375892  a(n) = (32)^n*cos (nC - nA), where A, B, C are, respectively, the
 *          angles opposite sides BC, CA, AB in a triangle ABC having
 *          sidelengths |BC| = 2, |CA| = 3, |AB| = 4; ABC is the smallest
 *          integer-sided scalene triangle.
 * https://oeis.org/A375892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 16 a(n-1) - 1024 a(n-2), a(0) = 1, a(1) = 8
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 13, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t a_prevprev = (seq_t)1;  /* a(0) */
    seq_t a_prev     = (seq_t)8;  /* a(1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i) {
        a_curr = (seq_t)16 * a_prev - (seq_t)1024 * a_prevprev;
        a_prevprev = a_prev;
        a_prev = a_curr;
    }

    return a_prev;
}
