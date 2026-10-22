/*
 * A384329  Table read by rows: row n is the unique primitive Pythagorean
 *          triple (a,b,c) such that (a-b+c)/2 = A000217(n) and its long leg
 *          and hypotenuse are consecutive natural numbers, n >= 0.
 * https://oeis.org/A384329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabf
 * Formula:   a(n) = the nth element of the flattened list of primitive
 *            Pythagorean triples given by T(m) = m*(m+1)/2 triple(m) =
 *            (2*T(m)-1, 2*T(m)*(T(m)-1), 2*T(m)*(T(m)-1)+1) where m =
 *            floor(n/3), n % 3 selects the first, second, or third element
 * Range:     i8 n <= 12, i16 n <= 48, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine which triple block and which component */
    idx_t block = n / 3;
    idx_t rem   = n % 3;

    /* Compute T = m*(m+1)/2 */
    seq_t m = (seq_t)block;
    seq_t T = m * (m + 1) / 2;

    /* Compute answer depending on remainder */
    if (rem == 0) {
        return (seq_t)2 * T - 1;
    } else if (rem == 1) {
        return (seq_t)2 * T * (T - 1);
    } else /* rem == 2 */ {
        return (seq_t)2 * T * (T - 1) + 1;
    }
}
