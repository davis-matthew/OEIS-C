/*
 * A254463  a(n) = 15*2^n + 6*4^n + 10*3^n + 3*5^n + 6^n + 21.
 * https://oeis.org/A254463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*2^n + 6*4^n + 10*3^n + 3*5^n + 6^n + 21
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t p2 = 1;   /* 2^n   */
    seq_t p3 = 1;   /* 3^n   */
    seq_t p4 = 1;   /* 4^n   */
    seq_t p5 = 1;   /* 5^n   */
    seq_t p6 = 1;   /* 6^n   */

    for (idx_t i = 0; i < n; ++i) {
        p2 *= 2;
        p3 *= 3;
        p4 *= 4;
        p5 *= 5;
        p6 *= 6;
    }

    seq_t res = 15 * p2 + 6 * p4 + 10 * p3 + 3 * p5 + p6 + 21;
    return res;
}
