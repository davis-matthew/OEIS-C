/*
 * A349118  Row sums of a triangle based on A261327.
 * https://oeis.org/A349118
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = * (n^3 - 3*n^2 + 14*n) / 24 if n is even, * ((n-1)^3 +
 *            11*(n-1)) / 6 if n is odd. * (Sequence defined for n >= 2; a(n)
 *            = 0 for n < 2.)
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    if ((n & 1) == 0) {                 /* even n */
        seq_t m  = (seq_t)n;
        seq_t m2 = m * m;
        seq_t m3 = m2 * m;
        seq_t num = m3 - 3 * m2 + 14 * m;
        return num / 24;
    } else {                            /* odd n */
        seq_t m  = (seq_t)(n - 1);
        seq_t m2 = m * m;
        seq_t m3 = m2 * m;
        seq_t num = m3 + 11 * m;
        return num / 6;
    }
}
