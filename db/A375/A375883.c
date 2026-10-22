/*
 * A375883  a(n) = 2^(2n-1)*sin(nA)/sqrt(15), where A is the angle opposite
 *          side BC in a triangle ABC having sidelengths |BC|=2, |CA| = 3,
 *          |AB| = 4; ABC is the smallest integer-sided scalene triangle.
 * https://oeis.org/A375883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 7*a(n-1) - 16*a(n-2); a(0) = 0, a(1) = 1
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 15, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Use recurrence to compute up to n */
    seq_t prev2 = (seq_t)0;          /* a(0) */
    seq_t prev1 = (seq_t)1;          /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)7 * prev1 - (seq_t)16 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
