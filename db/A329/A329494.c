/*
 * A329494  Numerator of 2*(2*n+1)/(n+2).
 * https://oeis.org/A329494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = (4*n + 2) / gcd(n + 2, 6) (i.e. the numerator of
 *            2*(2*n+1)/(n+2) reduced to lowest terms)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* g = gcd(n+2, 6) – only 1, 2, 3 or 6 can occur */
    idx_t d = n + 2;
    idx_t g;
    if (d % 6 == 0) {
        g = 6;
    } else if (d % 3 == 0) {
        g = 3;
    } else if (d % 2 == 0) {
        g = 2;
    } else {
        g = 1;
    }

    /* numerator = 4*n + 2 */
    seq_t num = (seq_t)n * 4 + 2;
    return num / (seq_t)g;
}
