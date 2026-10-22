/*
 * A305861  a(n) = 32*3^n - 2^(n+5) + 5.
 * https://oeis.org/A305861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*3^n - 2^(n+5) + 5
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow3 = 1;   /* 3^0 */
    seq_t pow2 = 1;   /* 2^0 */
    /* Compute 3^n and 2^n */
    for (idx_t i = 0; i < n; ++i) {
        pow3 *= 3;
        pow2 *= 2;
    }
    /* 2^(n+5) = 32 * 2^n */
    /* a(n) = 32*(3^n - 2^n) + 5 */
    return (seq_t)32 * (pow3 - pow2) + 5;
}
