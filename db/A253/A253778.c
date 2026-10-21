/*
 * A253778  Numbers b that are the first of m^3 consecutive cubes whose sum is
 *          a cube c^3, where m^3 is not divisible by 3 (A118719).
 * https://oeis.org/A253778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (m-1)*(m^2*(m-2)-4*(m+1))/6 where m = n + floor((n-1)/2 )
 *            (A001651 is the sequence of integers not divisible by 3)
 * Range:     i8 n <= 4, i16 n <= 14, i32 n <= 203, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle index below the offset (n < 1) simply. */
    if (n == 0)
        return 0;

    /* Compute m = n + floor((n-1)/2).  Use seq_t conversions to avoid
       unwanted 32‑bit overflow when the compiler treats the literal 2
       as an `int`. */
    seq_t m = (seq_t)n + ((seq_t)(n - 1) / 2);

    /* Compute the numerator: (m-1) * (m^2 * (m-2) - 4*(m+1)) */
    seq_t t1 = m - 1;
    seq_t t2 = m * m * (m - 2) - 4 * (m + 1);

    /* Final result */
    return t1 * t2 / 6;
}
