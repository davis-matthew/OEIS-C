/*
 * A363269  Positive squares (A000290) alternating with positive square
 *          pyramidal numbers (A000330).
 * https://oeis.org/A363269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { m^2 if n is odd, * m(m+1)(2m+1)/6 if n is even } *
 *            where m = (n+1)/2 (odd n) or m = n/2 (even n)
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 */
    if ((n & 1) == 1) {            /* odd n : square */
        seq_t m = ((seq_t)n + 1) / 2;
        return m * m;
    } else {                       /* even n : square pyramidal */
        seq_t m = (seq_t)n / 2;
        return m * (m + 1) * (2 * m + 1) / 6;
    }
}
