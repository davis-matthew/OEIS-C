/*
 * A326658  a(n) = 6*floor(n/2) + ceiling((n-1)^2/2).
 * https://oeis.org/A326658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-3 + 7*(-1)^n + 8*n + 2*n^2)/4
 * Range:     u8 n <= 9, u16 n <= 305, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute numerator: (-3 + 7*(-1)^n + 8*n + 2*n^2) */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t val = (seq_t)8 * nn + (seq_t)2 * n2;
    if ((n & 1u) == 0u) {
        /* even n: -3 + 7 = 4  */
        val += 4;
    } else {
        /* odd n: -3 - 7 = -10  (val is always >= 10 here) */
        val -= 10;
    }
    return val / 4;
}
