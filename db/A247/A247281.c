/*
 * A247281  a(n) = 4^n -(-1)^n.
 * https://oeis.org/A247281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 4^n - (-1)^n, Offset: 0
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow4 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow4 *= (seq_t)4;   /* compute 4^n */
    }
    if ((n & 1) == 0) {   /* even n : subtract 1 */
        return pow4 - (seq_t)1;
    } else {              /* odd n : add 1 */
        return pow4 + (seq_t)1;
    }
}
