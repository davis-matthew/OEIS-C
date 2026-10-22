/*
 * A352756  Positive numbers k such that the centered cube number k^3 +
 *          (k+1)^3 is equal to the difference of two positive cubes and to
 *          A352755(n).
 * https://oeis.org/A352756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2*n - 1)*(3*(2*n - 1)^2 + 4) - 1)/2
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 709, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute t = 2*n - 1 */
    seq_t t = (seq_t)(((__int128)n * 2) - 1);
    seq_t sq = t * t;            /* t^2 */
    seq_t temp = 3 * sq + 4;     /* 3*t^2 + 4 */
    seq_t result = (t * temp - 1) / 2;
    return result;
}
