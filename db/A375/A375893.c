/*
 * A375893  a(n) = (64)^n*cos (nA - nB), where A, B, C are, respectively, the
 *          angles opposite sides BC, CA, AB in a triangle ABC having
 *          sidelengths |BC| = 2, |CA| = 3, |AB| = 4; ABC is the smallest
 *          integer-sided scalene triangle.
 * https://oeis.org/A375893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 122*a(n-1) - 4096*a(n-2), a(0) = 1, a(1) = 61
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial two terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)61;

    /* Iteratively compute up to n using the recurrence. */
    seq_t prevprev = (seq_t)1;    /* a(0) */
    seq_t prev     = (seq_t)61;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (seq_t)122 * prev - (seq_t)4096 * prevprev;
        prevprev = prev;
        prev     = cur;
    }
    return cur;
}
